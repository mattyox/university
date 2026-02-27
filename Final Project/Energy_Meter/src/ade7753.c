#include "ade7753.h"

#include <zephyr/logging/log.h>

#include <math.h>


#define ADE7753_LOG_PROJECT_NAME Energy_Meter
#define ADE7753_LOG_LEVEL LOG_LEVEL_WRN


static int write_register(const struct ade7753_device *a_device, uint8_t a_tx_buffer[], const size_t a_tx_buffer_length)
{
    LOG_MODULE_DECLARE(ADE7753_LOG_PROJECT_NAME, ADE7753_LOG_LEVEL);

    a_tx_buffer[0] |= BIT(7);

    const struct spi_buf tx_spi_buf[] = {
        { .buf = a_tx_buffer, .len = a_tx_buffer_length }
    };

    const struct spi_buf_set tx_spi_buf_set = {
        .buffers = tx_spi_buf,
        .count = 1
    };

    int ret = 0;

    ret = gpio_pin_set_dt(a_device->cs, 1);
    if (ret != 0)
    {
        LOG_ERR("Could not set CS pin to active state: %i", ret);
        return ret;
    }
    else
    {
        LOG_INF("Set CS pin to active state.");
    }

    ret = spi_write_dt(a_device->spi_spec, &tx_spi_buf_set);
    if (ret != 0)
    {
        LOG_ERR("Could not write to ADE7753: %i", ret);
        return ret;
    }
    else
    {
        LOG_INF("Wrote to ADE7753.");
    }

    ret = gpio_pin_set_dt(a_device->cs, 0);
    if (ret != 0)
        LOG_ERR("Could not set CS pin to inactive state: %i", ret);
    else
        LOG_INF("Set CS pin to inactive state.");

    return ret;
}

static int read_register(struct ade7753_device *a_device, uint8_t a_tx_buffer[], const size_t a_tx_buffer_length, uint8_t a_rx_buffer[], const size_t a_rx_buffer_length)
{
    LOG_MODULE_DECLARE(ADE7753_LOG_PROJECT_NAME, ADE7753_LOG_LEVEL);

    const struct spi_buf tx_spi_buf[] = {
        { .buf = a_tx_buffer, .len = a_tx_buffer_length }
    };

    const struct spi_buf_set tx_spi_buf_set = {
        .buffers = tx_spi_buf,
        .count = 1
    };

    const struct spi_buf rx_spi_buf[] = {
        { .buf = a_rx_buffer, .len = a_rx_buffer_length }
    };

    const struct spi_buf_set rx_spi_buf_set = {
        .buffers = rx_spi_buf,
        .count = 1
    };

    int ret = 0;

    ret = gpio_pin_set_dt(a_device->cs, 1);
    if (ret != 0)
    {
        LOG_ERR("Could not set CS pin to active state: %i", ret);
        return ret;
    }
    else
    {
        LOG_INF("Set CS pin to active state.");
    }

    ret = spi_write_dt(a_device->spi_spec, &tx_spi_buf_set);
    if (ret != 0)
    {
        LOG_ERR("Could not write to ADE7753: %i", ret);
        return ret;
    }
    else
    {
        LOG_INF("Wrote to ADE7753.");
    }

    ret = spi_read_dt(a_device->spi_spec, &rx_spi_buf_set);
    if (ret != 0)
    {
        LOG_ERR("Could not read from ADE7753: %i", ret);
        return ret;
    }
    else
    {
        LOG_INF("Read from ADE7753.");
    }

    ret = gpio_pin_set_dt(a_device->cs, 0);
    if (ret != 0)
        LOG_ERR("Could not set CS pin to inactive state: %i", ret);
    else
        LOG_INF("Set CS pin to inactive state.");

    return ret;
}

static int check_checksum(const struct ade7753_device *a_device)
{
    LOG_MODULE_DECLARE(ADE7753_LOG_PROJECT_NAME, ADE7753_LOG_LEVEL);

    int ret = 0;

    uint8_t tx_buffer[] = { ADE7753_CHKSUM };
    uint8_t rx_buffer[1];

    ret = read_register(a_device, tx_buffer, sizeof(tx_buffer), rx_buffer, sizeof(rx_buffer));
    if (ret != 0)
    {
        LOG_ERR("Could not read ADE7753 Checksum Register: %i", ret);
        return ret;
    }
    else
    {
        LOG_INF("Read ADE7753 Checksum Register.");
    }

    LOG_DBG("Checksum Register content: %X", rx_buffer[0]);

    return ret;
}

int ade7753_reset(struct ade7753_device *a_device)
{
    LOG_MODULE_DECLARE(ADE7753_LOG_PROJECT_NAME, ADE7753_LOG_LEVEL);

    int ret = 0;

    return ret;
}

int ade7753_initialize(struct ade7753_device *a_device)
{
    LOG_MODULE_DECLARE(ADE7753_LOG_PROJECT_NAME, ADE7753_LOG_LEVEL);
    
    int ret = 0;

    ret = gpio_pin_configure_dt(a_device->cs, GPIO_OUTPUT_INACTIVE);
    if (ret != 0)
        LOG_ERR("Could configure CS pin to output inactive: %i", ret);
    else
        LOG_INF("Configured CS pin to output inactive.");

    return ret;
}

int ade7753_get_active_energy(struct ade7753_device *a_device)
{
    LOG_MODULE_DECLARE(ADE7753_LOG_PROJECT_NAME, ADE7753_LOG_LEVEL);

    int ret = 0;

    uint8_t tx_buffer[] = { ADE7753_RVAENERGY };
    uint8_t rx_buffer[3];

    ret = read_register(a_device, tx_buffer, sizeof(tx_buffer), rx_buffer, sizeof(rx_buffer));
    if (ret != 0)
    {
        LOG_ERR("Could not read ADE7753 Active Energy Register: %i", ret);
        return ret;
    }
    else
    {
        LOG_INF("Read ADE7753 Active Energy Register.");
    }

    LOG_DBG("Active Energy Register content: %X:%X:%X", rx_buffer[0], rx_buffer[1], rx_buffer[2]);

    uint32_t active_energy = 0;
    active_energy |= rx_buffer[2];
    active_energy |= (rx_buffer[1] << 8);
    active_energy |= (rx_buffer[0] << 16);

    if (active_energy > 60000)
    {
        LOG_ERR("Incorrect active energy value!");
        return -EBADMSG;
    }

    a_device->readings.active_energy = (active_energy / 3600.0) / 1000.0;

    LOG_WRN("Active increment: %i!", active_energy);

    return ret;
}

int ade7753_get_apparent_energy(struct ade7753_device *a_device)
{
    LOG_MODULE_DECLARE(ADE7753_LOG_PROJECT_NAME, ADE7753_LOG_LEVEL);

    int ret = 0;

    uint8_t tx_buffer[] = { ADE7753_RAENERGY };
    uint8_t rx_buffer[3];

    ret = read_register(a_device, tx_buffer, sizeof(tx_buffer), rx_buffer, sizeof(rx_buffer));
    if (ret != 0)
    {
        LOG_ERR("Could not read ADE7753 Apparent Energy Register: %i", ret);
        return ret;
    }
    else
    {
        LOG_INF("Read ADE7753 Apparent Energy Register.");
    }

    LOG_DBG("Apparent Energy Register content: %X:%X:%X", rx_buffer[0], rx_buffer[1], rx_buffer[2]);

    uint32_t apparent_energy = 0;
    apparent_energy |= rx_buffer[2];
    apparent_energy |= (rx_buffer[1] << 8);
    apparent_energy |= (rx_buffer[0] << 16);

    if (apparent_energy > 60000)
    {
        LOG_ERR("Incorrect apparent energy value!");
        return -EBADMSG;
    }

    LOG_WRN("Apparent increment: %i!", apparent_energy);

    a_device->readings.apparent_energy = (apparent_energy / 3600.0) / 1000.0;

    return ret;
}

void ade7753_get_reactive_energy(struct ade7753_device *a_device)
{
    LOG_MODULE_DECLARE(ADE7753_LOG_PROJECT_NAME, ADE7753_LOG_LEVEL);

    double reactive_energy = sqrt(pow(a_device->readings.apparent_energy, 2) - pow(a_device->readings.active_energy, 2));

    LOG_INF("Calculated reactive energy.");

    if (isnan(reactive_energy))
    {
        LOG_ERR("Incorrect reactive energy value!");
        return;
    }

    a_device->readings.reactive_energy = reactive_energy;

    LOG_DBG("Reactive energy value: %lf", a_device->readings.reactive_energy);
}

int ade7753_get_current_rms(struct ade7753_device *a_device)
{
    LOG_MODULE_DECLARE(ADE7753_LOG_PROJECT_NAME, ADE7753_LOG_LEVEL);

    int ret = 0;

    uint8_t tx_buffer[] = { ADE7753_IRMS };
    uint8_t rx_buffer[3];

    ret = read_register(a_device, tx_buffer, sizeof(tx_buffer), rx_buffer, sizeof(rx_buffer));
    if (ret != 0)
    {
        LOG_ERR("Could not read ADE7753 Channel 1 RMS Value (Current Channel) Register: %i", ret);
        return ret;
    }
    else
    {
        LOG_INF("Read ADE7753 Channel 1 RMS Value (Current Channel) Register.");
    }

    LOG_DBG("Channel 1 RMS Value (Current Channel) Register content: %X:%X:%X", rx_buffer[0], rx_buffer[1], rx_buffer[2]);

    uint32_t current_rms = 0;
    current_rms |= rx_buffer[2];
    current_rms |= (rx_buffer[1] << 8);
    current_rms |= (rx_buffer[0] << 16);

    a_device->readings.current_rms = current_rms * 0.000039352801928;

    return ret;
}

int ade7753_get_voltage_rms(struct ade7753_device *a_device)
{
    LOG_MODULE_DECLARE(ADE7753_LOG_PROJECT_NAME, ADE7753_LOG_LEVEL);

    int ret = 0;

    uint8_t tx_buffer[] = { ADE7753_VRMS };
    uint8_t rx_buffer[3];

    ret = read_register(a_device, tx_buffer, sizeof(tx_buffer), rx_buffer, sizeof(rx_buffer));
    if (ret != 0)
    {
        LOG_ERR("Could not read ADE7753 Channel 2 RMS Value (Voltage Channel) Register: %i", ret);
        return ret;
    }
    else
    {
        LOG_INF("Read ADE7753 Channel 2 RMS Value (Voltage Channel) Register.");
    }

    LOG_DBG("Channel 2 RMS Value (Voltage Channel) Register content: %X:%X:%X", rx_buffer[0], rx_buffer[1], rx_buffer[2]);

    uint32_t voltage_rms = 0;
    voltage_rms |= rx_buffer[2];
    voltage_rms |= (rx_buffer[1] << 8);
    voltage_rms |= (rx_buffer[0] << 16);

    a_device->readings.voltage_rms = voltage_rms * 0.000188788604906;

    return ret;
}

int ade7753_get_readings(struct ade7753_device *a_device)
{
    LOG_MODULE_DECLARE(ADE7753_LOG_PROJECT_NAME, ADE7753_LOG_LEVEL);

    int ret = 0;

    ret = ade7753_get_active_energy(a_device);
    if (ret != 0)
        LOG_ERR("Could not get active energy from ADE7753: %i!", ret);
    else
        LOG_INF("Got active energy from ADE7753.");

    ret = ade7753_get_apparent_energy(a_device);
    if (ret != 0)
        LOG_ERR("Could not get apparent energy from ADE7753: %i!", ret);
    else
        LOG_INF("Got apparent energy from ADE7753.");

    ade7753_get_reactive_energy(a_device);

    ret = ade7753_get_current_rms(a_device);
    if (ret != 0)
        LOG_ERR("Could not get current RMS from ADE7753: %i!", ret);
    else
        LOG_INF("Got current RMS from ADE7753.");

    ret = ade7753_get_voltage_rms(a_device);
    if (ret != 0)
        LOG_ERR("Could not get volatge RMS from ADE7753: %i!", ret);
    else
        LOG_INF("Got voltage RMS from ADE7753.");

    return ret;
}
