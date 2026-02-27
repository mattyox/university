#include "energy_meter.h"

#include <zephyr/logging/log.h>

#include "em_ot.h"
#include "em_eeprom.h"
#include "em_rtc.h"
#include "em_ioexpander.h"


#define ENERGY_METER_LOG_PROJECT_NAME Energy_Meter
#define ENERGY_METER_LOG_LEVEL LOG_LEVEL_WRN

#define GPIO1_PORT DT_NODELABEL(gpio1)
// #define TXB0108_VLT_OE_PIN 9
#define TXB0108_VLT_OE_PIN 0

#define SPI1_NODE DT_NODELABEL(spi1)

#define FREQUENCY 400000U

#define ENERGYMETER_0_NODE DT_ALIAS(energymeter0)
#define ENERGYMETER_1_NODE DT_ALIAS(energymeter1)
#define ENERGYMETER_2_NODE DT_ALIAS(energymeter2)

#define NODE_0_RESET_PIN 0
#define NODE_1_RESET_PIN 5

#define NODE_2_RESET_PIN 6


static struct ade7753_device s_energy_meter_0;
static struct ade7753_device s_energy_meter_1;
static struct ade7753_device s_energy_meter_2;

static int64_t s_boot_time;
int64_t s_reset_time;
static struct readings s_readings;

K_SEM_DEFINE(em_lock_0, 1, 1);
K_SEM_DEFINE(em_lock_1, 1, 1);
K_SEM_DEFINE(em_lock_2, 1, 1);


static int initialize_voltage_level_translator(void)
{
    LOG_MODULE_DECLARE(ENERGY_METER_LOG_PROJECT_NAME, ENERGY_METER_LOG_LEVEL);

    static const struct gpio_dt_spec output_enable = {
        .port = DEVICE_DT_GET(GPIO1_PORT),
        .pin = TXB0108_VLT_OE_PIN
    };
    
    if (!device_is_ready(output_enable.port))
    {
        LOG_ERR("GPIO port %s for output enable line is not ready!", output_enable.port->name);
        return -EIO;
    }

    int ret = gpio_pin_configure_dt(&output_enable, (GPIO_ACTIVE_HIGH | GPIO_OUTPUT_ACTIVE));
    if (ret != 0)
    {
        LOG_ERR("Could not set output enable line pin %d as output with high state!", output_enable.pin);

        return ret;
    }

    LOG_INF("Voltage level translator enabled.");

    return ret;
}

static int get_spi_bus(void)
{
    LOG_MODULE_DECLARE(ENERGY_METER_LOG_PROJECT_NAME, ENERGY_METER_LOG_LEVEL);
    
    static struct spi_dt_spec spi_bus = {
        .bus = DEVICE_DT_GET(SPI1_NODE),
        .config = {
            .frequency = FREQUENCY,
            .operation = SPI_WORD_SET(8) | SPI_MODE_CPHA,
            .slave = 0,
        }
    };

    if (!device_is_ready(spi_bus.bus))
    {
        LOG_ERR("SPI bus %s for Energy Meter is not ready!", spi_bus.bus->name);
        return -EIO;
    }

    s_energy_meter_0.spi_spec = &spi_bus;
    s_energy_meter_1.spi_spec = &spi_bus;
    s_energy_meter_2.spi_spec = &spi_bus;

    LOG_INF("SPI bus %s for Energy Meter is ready.", spi_bus.bus->name);

    return 0;
}

static int get_chip_select_0_pin(void)
{
    LOG_MODULE_DECLARE(ENERGY_METER_LOG_PROJECT_NAME, ENERGY_METER_LOG_LEVEL);
    
    static struct gpio_dt_spec cs = SPI_CS_GPIOS_DT_SPEC_GET(DT_ALIAS(energymeter0));

    if (!device_is_ready(cs.port))
    {
            LOG_ERR("CS port %s for Energy Meter Node 0 is not ready!", cs.port->name);
            return -EIO;
    }

    s_energy_meter_0.cs = &cs;

    LOG_INF("Found Energy Meter Node 0 Chip Select pin.");

    return 0;
}

static int get_chip_select_1_pin(void)
{
    LOG_MODULE_DECLARE(ENERGY_METER_LOG_PROJECT_NAME, ENERGY_METER_LOG_LEVEL);
    
    static struct gpio_dt_spec cs = SPI_CS_GPIOS_DT_SPEC_GET(DT_ALIAS(energymeter1));

    if (!device_is_ready(cs.port))
    {
            LOG_ERR("CS port %s for Energy Meter Node 1 is not ready!", cs.port->name);
            return -EIO;
    }

    s_energy_meter_1.cs = &cs;

    LOG_INF("Found Energy Meter Node 1 Chip Select pin.");

    return 0;
}

static int get_chip_select_2_pin(void)
{
    LOG_MODULE_DECLARE(ENERGY_METER_LOG_PROJECT_NAME, ENERGY_METER_LOG_LEVEL);

    static struct gpio_dt_spec cs = SPI_CS_GPIOS_DT_SPEC_GET(DT_ALIAS(energymeter2));

    if (!device_is_ready(cs.port))
    {
            LOG_ERR("CS port %s for Energy Meter Node 2 is not ready!", cs.port->name);
            return -EIO;
    }

    s_energy_meter_2.cs = &cs;

    LOG_INF("Found Energy Meter Node 2 Chip Select pin.");

    return 0;
}

static int get_chip_select_pins(void)
{
    LOG_MODULE_DECLARE(ENERGY_METER_LOG_PROJECT_NAME, ENERGY_METER_LOG_LEVEL);

    int ret = 0;

    ret = get_chip_select_0_pin();
    if (ret != 0)
	{
		LOG_ERR("Could not get Chip Select pin for Energy Meter Node 0: %i!", ret);
		return ret;
	}
	else
	{
		LOG_INF("Got Chip Select pin for Energy Meter Node 0.");
	}

    ret = get_chip_select_1_pin();
    if (ret != 0)
	{
		LOG_ERR("Could not get Chip Select pin for Energy Meter Node 1: %i!", ret);
		return ret;
	}
	else
	{
		LOG_INF("Got Chip Select pin for Energy Meter Node 1.");
	}

    ret = get_chip_select_2_pin();
    if (ret != 0)
    {
		LOG_ERR("Could not get Chip Select pin for Energy Meter Node 2: %i!", ret);
        return ret;
    }
	else
    {
		LOG_INF("Got Chip Select pin for Energy Meter Node 2.");
    }

    LOG_INF("Found Energy Meter Chip Select pins.");

    return ret;
}

static int initialize_nodes(void)
{
    LOG_MODULE_DECLARE(ENERGY_METER_LOG_PROJECT_NAME, ENERGY_METER_LOG_LEVEL);

    int ret = 0;

    ret = ade7753_initialize(&s_energy_meter_0);
    if (ret != 0)
    {
		LOG_ERR("Could not initialize Energy Meter Node 0: %i!", ret);
        return ret;
    }
	else
    {
		LOG_INF("Initialized Energy Meter Node 0.");
    }

    ret = em_ioxpander_0_pin_configure(NODE_0_RESET_PIN, GPIO_OUTPUT_LOW);
    if (ret != 0)
    {
		LOG_ERR("Could not configure reset pin for Energy Meter Node 0: %i!", ret);
        return ret;
    }
	else
    {
		LOG_INF("Configured reset pin for Energy Meter Node 0.");
    }

    // Time to reset device
	k_msleep(1);

    ret = em_ioxpander_0_pin_set(NODE_0_RESET_PIN, 1);
	if (ret != 0)
	{
		LOG_ERR("Could not deactivate Energy Meter Node 0 reset pin: %i!", ret);
		return ret;
	}
	else
	{
		LOG_INF("Deactivated Energy Meter Node 0 reset pin.");
	}

	// Time to recover from reset state
	k_msleep(1);

    s_energy_meter_0.lock = &em_lock_0;

    ret = ade7753_initialize(&s_energy_meter_1);
    if (ret != 0)
    {
		LOG_ERR("Could not initialize Energy Meter Node 1: %i!", ret);
        return ret;
    }
	else
    {
		LOG_INF("Initialized Energy Meter Node 1.");
    }

    ret = em_ioxpander_0_pin_configure(NODE_1_RESET_PIN, GPIO_OUTPUT_LOW);
    if (ret != 0)
    {
		LOG_ERR("Could not configure reset pin for Energy Meter Node 1: %i!", ret);
        return ret;
    }
	else
    {
		LOG_INF("Configured reset pin for Energy Meter Node 1.");
    }

    // Time to reset device
	k_msleep(1);

    ret = em_ioxpander_0_pin_set(NODE_1_RESET_PIN, 1);
	if (ret != 0)
	{
		LOG_ERR("Could not deactivate Energy Meter Node 1 reset pin: %i!", ret);
		return ret;
	}
	else
	{
		LOG_INF("Deactivated Energy Meter Node 1 reset pin.");
	}

	// Time to recover from reset state
	k_msleep(1);

    s_energy_meter_1.lock = &em_lock_1;

    ret = ade7753_initialize(&s_energy_meter_2);
    if (ret != 0)
    {
		LOG_ERR("Could not initialize Energy Meter Node 2: %i!", ret);
        return ret;
    }
	else
    {
		LOG_INF("Initialized Energy Meter Node 2.");
    }

    ret = em_ioxpander_1_pin_configure(NODE_2_RESET_PIN, GPIO_OUTPUT_LOW);
    if (ret != 0)
    {
		LOG_ERR("Could not configure reset pin for Energy Meter Node 2: %i!", ret);
        return ret;
    }
	else
    {
		LOG_INF("Configured reset pin for Energy Meter Node 2.");
    }

    // Time to reset device
	k_msleep(1);

    ret = em_ioxpander_1_pin_set(NODE_2_RESET_PIN, 1);
	if (ret != 0)
	{
		LOG_ERR("Could not deactivate Energy Meter Node 2 reset pin: %i!", ret);
		return ret;
	}
	else
	{
		LOG_INF("Deactivated Energy Meter Node 2 reset pin.");
	}

	// Time to recover from reset state
	k_msleep(1);

    s_energy_meter_2.lock = &em_lock_2;

    LOG_INF("Initialized Energy Meter nodes.");

    return ret;
}

static int energy_meter_get_device(void)
{
    LOG_MODULE_DECLARE(ENERGY_METER_LOG_PROJECT_NAME, ENERGY_METER_LOG_LEVEL);

    int ret = 0;

    memset(&s_energy_meter_0, 0, sizeof(s_energy_meter_0));
    memset(&s_energy_meter_1, 0, sizeof(s_energy_meter_1));
    memset(&s_energy_meter_2, 0, sizeof(s_energy_meter_2));

    ret = get_spi_bus();
	if (ret != 0)
	{
		LOG_ERR("Could not get SPI bus for Energy Meter: %i!", ret);
		return ret;
	}
	else
	{
		LOG_INF("Got SPI bus for Energy Meter.");
	}

    ret = get_chip_select_pins();
    if (ret != 0)
    {
		LOG_ERR("Could not get Chip Select pins for Energy Meter: %i!", ret);
        return ret;
    }
	else
    {
		LOG_INF("Got Chip Select pins for Energy Meter.");
    }

    ret = initialize_nodes();
    if (ret != 0)
    {
		LOG_ERR("Could not initialize nodes of Energy Meter: %i!", ret);
        return ret;
    }
	else
    {
		LOG_INF("Initialized nodes of Energy Meter.");
    }

    return ret;
}

int get_components(void)
{
    LOG_MODULE_DECLARE(ENERGY_METER_LOG_PROJECT_NAME, ENERGY_METER_LOG_LEVEL);

    int ret = 0;

    em_ot_set_context(openthread_get_default_context());

    em_ot_set_instance(openthread_get_default_instance());
    
    ret = em_ot_initialize();
    if (ret != 0)
	{
		LOG_ERR("Could not initialize OpenThread: %i!", ret);
		return ret;
	}
	else
	{
		LOG_INF("Initialized OpenThread.");
	}


    ret = em_eeprom_get_device();
	if (ret != 0)
	{
		LOG_ERR("Could not get EEPROM device: %i!", ret);
		return ret;
	}
	else
	{
		LOG_INF("Got EEPROM device.");
	}


    ret = em_rtc_get_device();
	if (ret != 0)
	{
		LOG_ERR("Could not get RTC device: %i!", ret);
		return ret;
	}
	else
	{
		LOG_INF("Got RTC device.");
	}


    ret = em_ioexpander_get_devices();
	if (ret != 0)
	{
		LOG_ERR("Could not get I/O Expander devices: %i!", ret);
		return ret;
	}
	else
	{
		LOG_INF("Got I/O Expander devices.");
	}

    ret = energy_meter_get_device();
    if (ret != 0)
        LOG_ERR("Could not get Energy Meter devices: %i!", ret);
    else
        LOG_INF("Got Energy Meter devices.");

    return ret;
}

int check_device_data(void)
{
    LOG_MODULE_DECLARE(ENERGY_METER_LOG_PROJECT_NAME, ENERGY_METER_LOG_LEVEL);

    size_t eeprom_size = em_eeprom_get_size();
    if (eeprom_size == 0)
    {
        LOG_ERR("EEPROM with size 0!");
        return -ENODEV;
    }
    else
    {
        LOG_INF("EEPROM with size %i.", eeprom_size);
    }

    int ret = 0;

    uint64_t read_eui64 = 0;

	ret = em_eeprom_read(EUI_OFFSET, &read_eui64, sizeof(read_eui64));
    if (ret != 0)
    {
        LOG_ERR("Could not read EUI64 from EEPROM: %i!", ret);
        return ret;
    }
    else
    {
        LOG_INF("Read EUI64 from EEPROM.");
    }

    uint64_t current_eui64 = 0;
    current_eui64 = em_ot_get_eui64();

	if (read_eui64 != current_eui64)
	{
		LOG_WRN("EUI64 has changed from %llX to %llX since last device boot!", read_eui64, current_eui64);

        ret = em_eeprom_write(EUI_OFFSET, &current_eui64, sizeof(current_eui64));
        if (ret != 0)
        {
            LOG_ERR("Could not write new EUI64 to EEPROM: %i!", ret);
            return ret;
        }
        else
        {
            LOG_INF("Wrote new EUI64 to EEPROM.");
        }
	}

    int64_t last_time = 0;

    ret = em_eeprom_read(BOOT_TIME_OFFSET, &last_time, sizeof(last_time));
    if (ret != 0)
    {
        LOG_ERR("Could not read last boot time from EEPROM: %i!", ret);
        return ret;
    }
    else
    {
        LOG_INF("Read last boot time from EEPROM.");
    }

    LOG_DBG("Last boot time:");
    em_rtc_log_time(last_time);

    ret = em_rtc_get_time(&s_boot_time);
    if (ret != 0)
    {
        LOG_ERR("Could not get current time from RTC: %i!", ret);
        return ret;
    }
    else
    {
        LOG_INF("Got current time from RTC.");
    }

	ret = em_eeprom_write(BOOT_TIME_OFFSET, &s_boot_time, sizeof(s_boot_time));
    if (ret != 0)
    {
        LOG_ERR("Could not write new boot time to EEPROM: %i!", ret);
        return ret;
    }
    else
    {
        LOG_INF("Wrote new boot time to EEPROM.");
    }

    ret = em_eeprom_read(RESET_TIME_OFFSET, &s_reset_time, sizeof(s_reset_time));
    if (ret != 0)
    {
        LOG_ERR("Could not read last reset time from EEPROM: %i!", ret);
        return ret;
    }
    else
    {
        LOG_INF("Read last reset time from EEPROM.");
    }

    LOG_DBG("Last reset time:");
    em_rtc_log_time(s_reset_time);

    ret = em_eeprom_read(READINGS_OFFSET, &s_readings, sizeof(s_readings));
    if (ret != 0)
        LOG_ERR("Could not get previous readings from EEPROM: %i!", ret);
    else
        LOG_INF("Got previous readings from EEPROM.");

    LOG_DBG("Restoring readings.\n"
            "Active energy: %lf [Wh].\n"
            "Reactive energy: %lf [VARh].\n"
            "Apparent energy: %lf [VAh].",
            s_readings.active_energy,
            s_readings.reactive_energy,
            s_readings.apparent_energy);

    return ret;
}

bool is_readings_update_required(void)
{
    LOG_MODULE_DECLARE(ENERGY_METER_LOG_PROJECT_NAME, LOG_LEVEL_NONE);

    LOG_INF("Returning readings update status.");
    LOG_DBG("Readings update status: %i.",  em_rtc_get_interrupt_status());

    return  em_rtc_get_interrupt_status();
}

int update_readings(void)
{
    LOG_MODULE_DECLARE(ENERGY_METER_LOG_PROJECT_NAME, ENERGY_METER_LOG_LEVEL);

    int ret = 0;

    ret += ade7753_get_readings(&s_energy_meter_0);
    if (ret != 0)
        LOG_WRN("Could not get readings from ADE7753 Node 0: %i!", ret);
    else
        LOG_INF("Got readings from ADE7753 Node 0.");

    ret += ade7753_get_readings(&s_energy_meter_1);
    if (ret != 0)
        LOG_WRN("Could not get readings from ADE7753 Node 1: %i!", ret);
    else
        LOG_INF("Got readings from ADE7753 Node 1.");

    ret += ade7753_get_readings(&s_energy_meter_2);
    if (ret != 0)
        LOG_WRN("Could not get readings from ADE7753 Node 2: %i!", ret);
    else
        LOG_INF("Got readings from ADE7753 Node 2.");

    s_readings.active_energy += (s_energy_meter_0.readings.active_energy
                             + s_energy_meter_1.readings.active_energy
                             + s_energy_meter_2.readings.active_energy);

    LOG_DBG("Total active energy value: %lf.", s_readings.active_energy);

    s_readings.apparent_energy += (s_energy_meter_0.readings.apparent_energy
                               + s_energy_meter_1.readings.apparent_energy
                               + s_energy_meter_2.readings.apparent_energy);
    
    LOG_DBG("Total apparent energy value: %lf.", s_readings.apparent_energy);
    
    s_readings.reactive_energy += (s_energy_meter_0.readings.reactive_energy
                               + s_energy_meter_1.readings.reactive_energy
                               + s_energy_meter_2.readings.reactive_energy);

    LOG_DBG("Total reactive energy value: %lf.", s_readings.reactive_energy);

    ret += em_eeprom_write(READINGS_OFFSET, &s_readings, sizeof(s_readings));
    if (ret != 0)
        LOG_ERR("Could not write new readings to EEPROM: %i!", ret);
    else
        LOG_INF("Wrote new readings to EEPROM.");

    return ret;
}

float get_active_energy(void)
{
    LOG_MODULE_DECLARE(ENERGY_METER_LOG_PROJECT_NAME, ENERGY_METER_LOG_LEVEL);

    LOG_INF("Returning active energy value.");
    LOG_DBG("Active energy value: %lf.", s_readings.active_energy);

    return s_readings.active_energy;
}

float get_apparent_energy(void)
{
    LOG_MODULE_DECLARE(ENERGY_METER_LOG_PROJECT_NAME, ENERGY_METER_LOG_LEVEL);

    LOG_INF("Returning apparent energy value.");
    LOG_DBG("Apparent energy value: %lf.", s_readings.apparent_energy);

    return s_readings.apparent_energy;
}

float get_reactive_energy(void)
{
    LOG_MODULE_DECLARE(ENERGY_METER_LOG_PROJECT_NAME, ENERGY_METER_LOG_LEVEL);

    LOG_INF("Returning reactive energy value.");
    LOG_DBG("Reactive energy value: %lf.", s_readings.reactive_energy);

    return s_readings.reactive_energy;
}

float get_node_0_current(void)
{
    LOG_MODULE_DECLARE(ENERGY_METER_LOG_PROJECT_NAME, ENERGY_METER_LOG_LEVEL);

    LOG_INF("Returning Node 0 current value.");
    LOG_DBG("Node 0 current value: %lf.", s_energy_meter_0.readings.current_rms);

    return s_energy_meter_0.readings.current_rms;
}

float get_node_0_voltage(void)
{
    LOG_MODULE_DECLARE(ENERGY_METER_LOG_PROJECT_NAME, ENERGY_METER_LOG_LEVEL);

    LOG_INF("Returning Node 0 voltage value.");
    LOG_DBG("Node 0 voltage value: %lf.", s_energy_meter_0.readings.voltage_rms);

    return s_energy_meter_0.readings.voltage_rms;
}

float get_node_1_current(void)
{
    LOG_MODULE_DECLARE(ENERGY_METER_LOG_PROJECT_NAME, ENERGY_METER_LOG_LEVEL);

    LOG_INF("Returning Node 1 current value.");
    LOG_DBG("Node 1 current value: %lf.", s_energy_meter_1.readings.current_rms);

    return s_energy_meter_1.readings.current_rms;
}

float get_node_1_voltage(void)
{
    LOG_MODULE_DECLARE(ENERGY_METER_LOG_PROJECT_NAME, ENERGY_METER_LOG_LEVEL);

    LOG_INF("Returning Node 1 voltage value.");
    LOG_DBG("Node 1 voltage value: %lf.", s_energy_meter_1.readings.voltage_rms);

    return s_energy_meter_1.readings.voltage_rms;
}

float get_node_2_current(void)
{
    LOG_MODULE_DECLARE(ENERGY_METER_LOG_PROJECT_NAME, ENERGY_METER_LOG_LEVEL);

    LOG_INF("Returning Node 2 current value.");
    LOG_DBG("Node 2 current value: %lf.", s_energy_meter_2.readings.current_rms);

    return s_energy_meter_2.readings.current_rms;
}

float get_node_2_voltage(void)
{
    LOG_MODULE_DECLARE(ENERGY_METER_LOG_PROJECT_NAME, ENERGY_METER_LOG_LEVEL);

    LOG_INF("Returning Node 2 voltage value.");
    LOG_DBG("Node 2 voltage value: %lf.", s_energy_meter_2.readings.voltage_rms);

    return s_energy_meter_2.readings.voltage_rms;
}

int64_t get_boot_time(void)
{
    LOG_MODULE_DECLARE(ENERGY_METER_LOG_PROJECT_NAME, ENERGY_METER_LOG_LEVEL);

    LOG_INF("Returning last boot time.");
    em_rtc_log_time(s_boot_time);

    return s_boot_time;
}

int64_t get_reset_time(void)
{
    LOG_MODULE_DECLARE(ENERGY_METER_LOG_PROJECT_NAME, ENERGY_METER_LOG_LEVEL);

    LOG_INF("Returning last reset time.");
    em_rtc_log_time(s_reset_time);

    return s_reset_time;
}

void reset_readings(void)
{
    LOG_MODULE_DECLARE(ENERGY_METER_LOG_PROJECT_NAME, ENERGY_METER_LOG_LEVEL);

    LOG_INF("Reseting readings.");

    s_readings.active_energy = 0.0;
    s_readings.apparent_energy = 0.0;
    s_readings.reactive_energy = 0.0;

    int ret = 0;

    ret = em_rtc_get_time(&s_reset_time);
    if (ret != 0)
        LOG_ERR("Could not get current time from RTC: %i!", ret);
    else
        LOG_INF("Got current time from RTC.");

    ret = em_eeprom_write(RESET_TIME_OFFSET, &s_reset_time, sizeof(s_reset_time));
    if (ret != 0)
        LOG_ERR("Could not write new reset time to EEPROM: %i!", ret);
    else
        LOG_INF("Wrote new reset time to EEPROM.");
}
