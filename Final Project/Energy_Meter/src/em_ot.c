#include "em_ot.h"

#include <zephyr/drivers/gpio.h>

#include <openthread/thread.h>
#include <openthread/srp_client.h>
#include <openthread/udp.h>
#include <openthread/dns_client.h>
#include <openthread/sntp.h>

#include "energy_meter.h"


#define OT_LOG_PROJECT_NAME Energy_Meter
#define OT_LOG_LEVEL LOG_LEVEL_DBG

// #define STATUS_LED_NODE DT_ALIAS(led0)
#define STATUS_LED_NODE DT_ALIAS(led3)

#define SRP_CLIENT_HOST_NAME "ot-host"

#define UDP_SRP_CLIENT_SERVICE_INSTANCE_NAME "ot-service"
#define UDP_SRP_CLIENT_SERVICE_NAME "_energy_meter._udp"
#define UDP_SRP_CLIENT_SERVICE_PORT 51200

#define SNTP_SERVER_NAME "time.google.com"


static struct gpio_dt_spec s_state_led;

static struct openthread_context *s_context = NULL;
static otInstance *s_instance = NULL;
static uint8_t s_eui64[8];

static otUdpSocket s_udp_socket;

static otIp6Address s_sntp_address;
static uint64_t s_sntp_time;

K_SEM_DEFINE(dns_lock, 0, 1);
K_SEM_DEFINE(sntp_lock, 0, 1);


static int configure_led0(void)
{
    LOG_MODULE_DECLARE(OT_LOG_PROJECT_NAME, OT_LOG_LEVEL);

    static struct gpio_dt_spec led = GPIO_DT_SPEC_GET(STATUS_LED_NODE, gpios);

    s_state_led = led;

    int ret = 0;

    if (!device_is_ready(s_state_led.port))
    {
        LOG_ERR("GPIO port %s for LED 0 is not ready!", s_state_led.port->name);
        return -ENODEV;
    }

    ret = gpio_pin_configure_dt(&s_state_led, GPIO_OUTPUT_INACTIVE);
    if (ret != 0)
        LOG_ERR("Could not configure LED 0 pin: %i!", ret);
    else
        LOG_INF("Configured LED 0 pin.");

    return ret;
}

static void state_changed_callback(otChangedFlags a_flags, void *a_context)
{
    LOG_MODULE_DECLARE(OT_LOG_PROJECT_NAME, OT_LOG_LEVEL);

    OT_UNUSED_VARIABLE(a_context);

    if ((a_flags & OT_CHANGED_THREAD_ROLE) != 0)
    {
        otDeviceRole current_role = otThreadGetDeviceRole(s_instance);

        switch (current_role)
        {
            case OT_DEVICE_ROLE_LEADER:
            case OT_DEVICE_ROLE_ROUTER:
            case OT_DEVICE_ROLE_CHILD:
                LOG_INF("Device connected to OpenThread network.");
                gpio_pin_set_dt(&s_state_led, 1);
                break;

            case OT_DEVICE_ROLE_DETACHED:
            case OT_DEVICE_ROLE_DISABLED:
                LOG_INF("Device not connected to OpenThread network.");
                gpio_pin_set_dt(&s_state_led, 0);
                break;
        }
    }
}

static void srp_client_auto_start_callback(const otSockAddr *a_server_sock_addr, void *a_context)
{
    LOG_MODULE_DECLARE(OT_LOG_PROJECT_NAME, OT_LOG_LEVEL);

    LOG_INF("Called SRP client auto start callback.");

    if (a_server_sock_addr != NULL)
    {
        LOG_INF("SRP client auto start.");

        char srp_address_string[OT_IP6_ADDRESS_STRING_SIZE];
        otIp6AddressToString(&a_server_sock_addr->mAddress, srp_address_string, OT_IP6_ADDRESS_STRING_SIZE);
        LOG_DBG("SRP server address: %s", srp_address_string);

        LOG_DBG("SRP server port: %i", a_server_sock_addr->mPort);
    }
    else
    {
        LOG_INF("SRP client auto stop.");
    }
}

static otError initialize_srp_client(void)
{
    LOG_MODULE_DECLARE(OT_LOG_PROJECT_NAME, OT_LOG_LEVEL);

    otError error = OT_ERROR_NONE;

    error = otSrpClientEnableAutoHostAddress(s_instance);
    if (error == OT_ERROR_NONE)
    {
        LOG_INF("Enabled SRP client auto host address.");
    }
    else
    {
        LOG_ERR("Could not enable SRP client auto host address: %s!", otThreadErrorToString(error));
        return error;
    }

    otSrpClientEnableAutoStartMode(s_instance, srp_client_auto_start_callback, s_context);
    LOG_INF("Enabled SRP client auto start mode.");

    otPlatRadioGetIeeeEui64(s_instance, s_eui64);
    
    static char host_name[sizeof(SRP_CLIENT_HOST_NAME) + 17];
    strcpy(host_name, SRP_CLIENT_HOST_NAME);
    strcat(host_name, "-");
    char eui64[17];
    em_ot_get_eui64_string(eui64);
    strcat(host_name, eui64);
    
    error = otSrpClientSetHostName(s_instance, host_name);
    if (error == OT_ERROR_NONE)
        LOG_INF("Set SRP client host name.");
    else
        LOG_ERR("Could not set SRP client host name: %s!", otThreadErrorToString(error));
    
    return error;
}

static otError add_srp_client_service(void)
{
    LOG_MODULE_DECLARE(OT_LOG_PROJECT_NAME, OT_LOG_LEVEL);

    static otSrpClientService srp_client_service;
    memset(&srp_client_service, 0, sizeof(srp_client_service));
    srp_client_service.mInstanceName = UDP_SRP_CLIENT_SERVICE_INSTANCE_NAME;
    srp_client_service.mName = UDP_SRP_CLIENT_SERVICE_NAME;
    srp_client_service.mPort = UDP_SRP_CLIENT_SERVICE_PORT;

    otError error = OT_ERROR_NONE;

    error = otSrpClientAddService(s_instance, &srp_client_service);
    if (error == OT_ERROR_NONE)
        LOG_INF("Added SRP client service.");
    else
        LOG_ERR("Could not add SRP client service: %s!", otThreadErrorToString(error));

    return error;
}

static void udp_receive_callback(void *a_context, otMessage *a_message, const otMessageInfo *a_message_info)
{
    LOG_MODULE_DECLARE(OT_LOG_PROJECT_NAME, OT_LOG_LEVEL);

    OT_UNUSED_VARIABLE(a_context);

    uint16_t message_length = otMessageGetLength(a_message);
    if (message_length != 0)
    {
        LOG_INF("Received UDP message of length: %i [B].", message_length);
    }
    else
    {
        LOG_ERR("Received UDP message of length 0!");
        return;
    }

    uint16_t read_bytes = 0;

    uint8_t buffer[message_length];

    read_bytes = otMessageRead(a_message, otMessageGetOffset(a_message), buffer, message_length);
    if (read_bytes != 0)
    {
        LOG_INF("Read %i [B] from UDP message.", read_bytes);
    }
    else
    {
        LOG_ERR("Could not read UDP message!");
        return;
    }

    otMessage *message = otUdpNewMessage(s_instance, NULL);
    if (message != NULL)
    {
        LOG_INF("Created new UDP message.");
    }
    else
    {
        LOG_ERR("Could not create new UDP message!");
        return;
    }

    uint8_t command = 0xFF;
    uint8_t *response_message;
    size_t response_size = 0;

    switch (buffer[0])
    {
    case 0x00:
        LOG_INF("0x00 command.");
        command = 0x00;
        const double active_energy = get_active_energy();
        response_size = sizeof(active_energy);
        response_message = (uint8_t*)malloc(response_size);
        memcpy(response_message, &active_energy, response_size);
        break;

    case 0x01:
        LOG_INF("0x01 command.");
        command = 0x01;
        const double reactive_energy = get_reactive_energy();
        response_size = sizeof(reactive_energy);
        response_message = (uint8_t*)malloc(response_size);
        memcpy(response_message, &reactive_energy, response_size);
        break;

    case 0x02:
        LOG_INF("0x02 command.");
        command = 0x02;
        const double apparent_energy = get_apparent_energy();
        response_size = sizeof(apparent_energy);
        response_message = (uint8_t*)malloc(response_size);
        memcpy(response_message, &apparent_energy, response_size);
        break;

    case 0x03:
        LOG_INF("0x03 command.");
        command = 0x03;
        const double current_rms_0 = get_node_0_current();
        response_size = sizeof(current_rms_0);
        response_message = (uint8_t*)malloc(response_size);
        memcpy(response_message, &current_rms_0, response_size);
        break;

    case 0x04:
        LOG_INF("0x04 command.");
        command = 0x04;
        const double voltage_rms_0 = get_node_0_voltage();
        response_size = sizeof(voltage_rms_0);
        response_message = (uint8_t*)malloc(response_size);
        memcpy(response_message, &voltage_rms_0, response_size);
        break;

    case 0x05:
        LOG_INF("0x05 command.");
        command = 0x05;
        const double current_rms_1 = get_node_1_current();
        response_size = sizeof(current_rms_1);
        response_message = (uint8_t*)malloc(response_size);
        memcpy(response_message, &current_rms_1, response_size);
        break;

    case 0x06:
        LOG_INF("0x06 command.");
        command = 0x06;
        const double voltage_rms_1 = get_node_1_voltage();
        response_size = sizeof(voltage_rms_1);
        response_message = (uint8_t*)malloc(response_size);
        memcpy(response_message, &voltage_rms_1, response_size);
        break;

    case 0x07:
        LOG_INF("0x07 command.");
        command = 0x07;
        const double current_rms_2 = get_node_2_current();
        response_size = sizeof(current_rms_2);
        response_message = (uint8_t*)malloc(response_size);
        memcpy(response_message, &current_rms_2, response_size);
        break;

    case 0x08:
        LOG_INF("0x08 command.");
        command = 0x08;
        const double voltage_rms_2 = get_node_2_voltage();
        response_size = sizeof(voltage_rms_2);
        response_message = (uint8_t*)malloc(response_size);
        memcpy(response_message, &voltage_rms_2, response_size);
        break;

    case 0x09:
        LOG_INF("0x09 command.");
        command = 0x09;
        const int64_t boot_time = get_boot_time();
        response_size = sizeof(boot_time);
        response_message = (uint8_t*)malloc(response_size);
        memcpy(response_message, &boot_time, response_size);
        break;

    case 0x0A:
        LOG_INF("0x0A command.");
        command = 0x0A;
        const int64_t reset_time = get_reset_time();
        response_size = sizeof(reset_time);
        response_message = (uint8_t*)malloc(response_size);
        memcpy(response_message, &reset_time, response_size);
        break;

    case 0x0B:
        LOG_INF("0x0B command.");
        command = 0x0B;
        reset_readings();
        break;
    
    default:
        LOG_WRN("Unknown command!");
        response_size = sizeof("Warning. Unknown command!");
        response_message = (char*)malloc(response_size);
        strcpy(response_message, "Warning. Unknown command!");
        break;
    }

    otError error = OT_ERROR_NONE;

    error = otMessageAppend(message, &command, sizeof(command));
    if (error == OT_ERROR_NONE)
    {
        LOG_INF("Appended message.");
    }
    else
    {
        LOG_ERR("Could not append message: %s!", otThreadErrorToString(error));
        otMessageFree(message);
        return;
    }

    error = otMessageAppend(message, response_message, response_size);
    if (error == OT_ERROR_NONE)
    {
        LOG_INF("Appended message.");
    }
    else
    {
        LOG_ERR("Could not append message: %s!", otThreadErrorToString(error));
        otMessageFree(message);
        return;
    }

    free(response_message);

    error = otUdpSend(s_instance, &s_udp_socket, message, a_message_info);
    if (error == OT_ERROR_NONE)
    {
        LOG_INF("Sent UDP message.");
    }
    else
    {
        LOG_ERR("Could not send UDP message: %s!", otThreadErrorToString(error));
        otMessageFree(message);
    }
}

static otError initialize_udp_socket(void)
{
    LOG_MODULE_DECLARE(OT_LOG_PROJECT_NAME, OT_LOG_LEVEL);

    otSockAddr udp_socket_address;

    memset(&s_udp_socket, 0, sizeof(s_udp_socket));
    memset(&udp_socket_address, 0, sizeof(udp_socket_address));

    udp_socket_address.mPort = UDP_SRP_CLIENT_SERVICE_PORT;

    otError error = OT_ERROR_NONE;

    error = otUdpOpen(s_instance, &s_udp_socket, udp_receive_callback, s_context);
    if (error == OT_ERROR_NONE)
    {
        LOG_INF("Opened UDP socket.");
    }
    else
    {
        LOG_ERR("Could not open UDP socket: %s!", otThreadErrorToString(error));
        return error;
    }

    error = otUdpBind(s_instance, &s_udp_socket, &udp_socket_address, OT_NETIF_THREAD);
    if (error == OT_ERROR_NONE)
        LOG_INF("Binded UDP socket.");
    else
        LOG_ERR("Could not bind UDP: %s!", otThreadErrorToString(error));

    return error;
}

static void dns_address_callback(otError a_error, const otDnsAddressResponse *a_response, void *a_context)
{
    LOG_MODULE_DECLARE(OT_LOG_PROJECT_NAME, OT_LOG_LEVEL);

    OT_UNUSED_VARIABLE(a_context);

    if (a_error == OT_ERROR_NONE)
    {
        LOG_INF("Got DNS address response.");

        a_error = otDnsAddressResponseGetAddress(a_response, 0, &s_sntp_address, NULL);

        if (a_error == OT_ERROR_NONE)
        {
            otIp6Address dns_ip6_address = s_sntp_address;
            char dns_string_address[OT_IP6_ADDRESS_STRING_SIZE];
            otIp6AddressToString(&dns_ip6_address, dns_string_address, OT_IP6_ADDRESS_STRING_SIZE);

            LOG_DBG("Got address from DNS response: %s.", dns_string_address);
        }
        else
        {
            LOG_ERR("Could not get address from DNS reponse: %s!", otThreadErrorToString(a_error));
        }
    }
    else
    {
        LOG_ERR("Could not get DNS address response: %s!", otThreadErrorToString(a_error));
    }

    k_sem_give(&dns_lock);
}

static void sntp_response_handler(void *a_context, uint64_t a_time, otError a_result)
{
    LOG_MODULE_DECLARE(OT_LOG_PROJECT_NAME, OT_LOG_LEVEL);

    OT_UNUSED_VARIABLE(a_context);

    if (a_result == OT_ERROR_NONE)
    {
        LOG_DBG("Got Unix epoch: %llu.", a_time);

        s_sntp_time = a_time;
    }
    else
    {
        LOG_ERR("Could not get unix epoch: %s!", otThreadErrorToString(a_result));
    }

    k_sem_give(&sntp_lock);
}

void em_ot_set_context(struct openthread_context *a_context)
{
    LOG_MODULE_DECLARE(OT_LOG_PROJECT_NAME, OT_LOG_LEVEL);

    LOG_INF("Set OpenThead context.");

    s_context = a_context;
}

void em_ot_set_instance(struct otInstance *a_instance)
{
    LOG_MODULE_DECLARE(OT_LOG_PROJECT_NAME, OT_LOG_LEVEL);

    LOG_INF("Set OpenThead instance.");

    s_instance = a_instance;
}

otError em_ot_initialize(void)
{
    LOG_MODULE_DECLARE(OT_LOG_PROJECT_NAME, OT_LOG_LEVEL);

    int ret = 0;

    ret = configure_led0();
    if (ret != 0)
    {
		LOG_ERR("Could configure LED 0: %i!", ret);
        return ret;
    }
	else
    {
		LOG_INF("Configured LED 0.");
    }

    otError error = OT_ERROR_NONE;

    error = otSetStateChangedCallback(s_instance, state_changed_callback, s_context);
    if (error == OT_ERROR_NONE)
    {
        LOG_INF("Set OpenThread state changed callback.");
    }
    else
    {
        LOG_ERR("Could not set OpenThread state changed callback: %s!", otThreadErrorToString(error));
        return error;
    }

    error = openthread_start(s_context);
    if (error == OT_ERROR_NONE)
    {
        LOG_INF("Started OpenThread.");
    }
    else
    {
        LOG_ERR("Could not start OpenThread: %s!", otThreadErrorToString(error));
        return error;
    }

    error = initialize_srp_client();
    if (error == OT_ERROR_NONE)
    {
        LOG_INF("Initialized SRP client.");
    }
    else
    {
        LOG_ERR("Could not initialize SRP client: %s!", otThreadErrorToString(error));
        return error;
    }

    error = add_srp_client_service();
    if (error == OT_ERROR_NONE)
    {
        LOG_INF("Added SRP client service.");
    }
    else
    {
        LOG_ERR("Could not add SRP client service: %s!", otThreadErrorToString(error));
        return error;
    }

    error = initialize_udp_socket();
    if (error == OT_ERROR_NONE)
    {
        LOG_INF("Initialized UDP socket.");
    }
    else
    {
        LOG_ERR("Could not initialize UDP socket: %s!", otThreadErrorToString(error));
        return error;
    }

    // At least 1 second is needed to start the network interface
    k_sleep(K_SECONDS(5));
        
    error = otDnsClientResolveIp4Address(s_instance, SNTP_SERVER_NAME, dns_address_callback, s_context, NULL);
    if (error == OT_ERROR_NONE)
    {
        LOG_INF("DNS client resolved IPv4 address.");
    }
    else
    {
        LOG_ERR("DNS client could not resolve IPv4 address: %s!", otThreadErrorToString(error));
        return error;
    }

    // Wait for DNS address callback
	ret = k_sem_take(&dns_lock, K_FOREVER);
	if (ret != 0)
		LOG_ERR("Could not take semaphore for dns_address_callback: %i!", ret);
	else
		LOG_INF("Took semaphore for dns_address_callback.");

    return ret;
}

int em_ot_get_sntp_time(int64_t *a_unix_epoch)
{
    LOG_MODULE_DECLARE(OT_LOG_PROJECT_NAME, OT_LOG_LEVEL);

    otMessageInfo message_info;
    memset(&message_info, 0, sizeof(message_info));
    message_info.mPeerPort = OT_SNTP_DEFAULT_SERVER_PORT;
    message_info.mPeerAddr = s_sntp_address;

    otSntpQuery sntp_query;
    sntp_query.mMessageInfo = &message_info;

    otError error = OT_ERROR_NONE;

    int ret = 0;

    error = otSntpClientQuery(s_instance, &sntp_query, sntp_response_handler, s_context);
    if (error == OT_ERROR_NONE)
    {
        LOG_INF("Queried SNTP client.");
    }
    else
    {
        LOG_ERR("Could not could not query SNTP client: %s!", otThreadErrorToString(error));
        return error;
    }

    // Wait for SNTP response
	ret = k_sem_take(&sntp_lock, K_FOREVER);
	if (ret != 0)
    {
		LOG_ERR("Could not take semaphore for sntp_response_handler: %i!", ret);

        *a_unix_epoch = 0;
    }
	else
    {
		LOG_INF("Took semaphore for sntp_response_handler.");

        *a_unix_epoch = s_sntp_time;
    }

    return ret;
}

uint64_t em_ot_get_eui64(void)
{
    LOG_MODULE_DECLARE(OT_LOG_PROJECT_NAME, OT_LOG_LEVEL);

    LOG_INF("Got OpenThead EUI64.");

    uint64_t eui64 = 0;
    uint64_t shift = 0;

    for (int i = 7; i >= 0; i--)
    {
        uint64_t temp_eui64 = s_eui64[i];
        temp_eui64 <<= shift;
        eui64 |= temp_eui64;
        shift += 8;
    }

    LOG_DBG("EUI64: %llX.", eui64);

    return eui64;
}

static char uint8_t_to_char(const uint8_t value)
{
    LOG_MODULE_DECLARE(OT_LOG_PROJECT_NAME, LOG_LEVEL_NONE);

    LOG_DBG("uint8_t: %X.", value);

    char c;

    if (value < 0xA)
        c = (value + 48);
    else
        c = (value + 55);

    LOG_DBG("char: %c.", c);

    return c;
}

char *em_ot_get_eui64_string(char eui64_string[])
{
    LOG_MODULE_DECLARE(OT_LOG_PROJECT_NAME, LOG_LEVEL_NONE);

    for (int i = 0; i < 8; i++)
    {
        eui64_string[2 * i] = uint8_t_to_char((s_eui64[i] & 0xF0) >> 4);
        eui64_string[(2 * i) + 1] = uint8_t_to_char(s_eui64[i] & 0x0F);
    }

    eui64_string[16] = '\0';

    LOG_DBG("EUI64: %s.", eui64_string);

    return eui64_string;
}
