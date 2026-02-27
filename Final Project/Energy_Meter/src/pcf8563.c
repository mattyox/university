#include "pcf8563.h"

#include <zephyr/logging/log.h>


#define PCF8563_LOG_PROJECT_NAME Energy_Meter
#define PCF8563_LOG_LEVEL LOG_LEVEL_DBG


int pcf8563_get_time(const struct pcf8563_device *a_device, struct tm *a_time)
{
    LOG_MODULE_DECLARE(PCF8563_LOG_PROJECT_NAME, PCF8563_LOG_LEVEL);

	if (k_is_in_isr())
	{
		LOG_ERR("I2C bus is not avaible from interrupt!");
		return -EWOULDBLOCK;
	}

	int ret = 0;

	ret = k_sem_take(a_device->lock, K_FOREVER);
	if (ret != 0)
	{
		LOG_ERR("Could not take semaphore for pcf8563_get_time: %i!", ret);
		return ret;
	}
	else
	{
		LOG_INF("Took semaphore for pcf8563_get_time.");
	}

    // Variable for storing values from time registers
    uint8_t byte_time[7] = { 0 };

	const uint16_t addr = a_device->i2c_spec->addr;

    // Read values from all time registers
	ret = i2c_burst_read_dt(a_device->i2c_spec, PCF8563_VLSECONDS, byte_time, sizeof(byte_time));
	if (ret != 0)
	{
		LOG_ERR("Could not read time from PCF8563 @%X: %i.", addr, ret);
		return ret;
	}
	else
	{
		LOG_INF("Read time from PCF8563 @%X.", addr);
	}

	LOG_DBG("PCF8563 @%X", addr);
	LOG_HEXDUMP_DBG(byte_time, sizeof(byte_time), "time and date registers read data"); 

	// Ensure clock integrity
	if (byte_time[0] & PCF8563_VLSECONDS_VL)
	{
		LOG_WRN("PCF8563 @%X time integrity is not guaranteed!", addr);

		k_sem_give(a_device->lock);
		
		return -EBADMSG;
	}
	else
	{
		LOG_INF("PCF8563 @%X time integrity is guaranteed.", addr);
	}

    // Second
	a_time->tm_sec = (((byte_time[0] & 0x70) >> BCD_SHIFT) * 10) + (byte_time[0] & BCD_UNITS);

	// Minute
	a_time->tm_min = (((byte_time[1] & 0x70) >> BCD_SHIFT) * 10) + (byte_time[1] & BCD_UNITS);

	// Hour
	a_time->tm_hour = (((byte_time[2] & 0x30) >> BCD_SHIFT) * 10) + (byte_time[2] & BCD_UNITS);

	// Day
	a_time->tm_mday = (((byte_time[3] & 0x30) >> BCD_SHIFT) * 10) + (byte_time[3] & BCD_UNITS);

	// Weekday
	a_time->tm_wday = 0x07 & byte_time[4];

	// Month
	a_time->tm_mon = ((((byte_time[5] & 0x10) >> BCD_SHIFT) * 10) + (byte_time[5] & BCD_UNITS)) - 1;

	// Year
	a_time->tm_year = ((((byte_time[6] & 0xF0) >> BCD_SHIFT) * 10) + (byte_time[6] & BCD_UNITS)) + (((byte_time[5] & PCF8563_CENTURYMONTHS_C) >> 7) * 100);

	// Day of year
	a_time->tm_yday = 0;

	// Daylight Saving Time flag - no information avaible
	a_time->tm_isdst = -1;

	k_sem_give(a_device->lock);

    return ret;
}

int pcf8563_set_time(const struct pcf8563_device *a_device, const struct tm *a_time)
{
    LOG_MODULE_DECLARE(PCF8563_LOG_PROJECT_NAME, PCF8563_LOG_LEVEL);

	if (k_is_in_isr())
	{
		LOG_ERR("I2C bus is not avaible from interrupt!");
		return -EWOULDBLOCK;
	}

	int ret = 0;

	ret = k_sem_take(a_device->lock, K_FOREVER);
	if (ret != 0)
	{
		LOG_ERR("Could not take semaphore for pcf8563_set_time: %i!", ret);
		return ret;
	}
	else
	{
		LOG_INF("Took semaphore for pcf8563_set_time.");
	}

	// Variable for storing values to time registers
	uint8_t byte_time[7] = { 0 };

    // Second
	byte_time[0] = ((a_time->tm_sec / 10) << BCD_SHIFT) + (a_time->tm_sec % 10);

	// Minute
	byte_time[1] = ((a_time->tm_min / 10) << BCD_SHIFT) + (a_time->tm_min % 10);

	// Hour
	byte_time[2] = ((a_time->tm_hour / 10) << BCD_SHIFT) + (a_time->tm_hour % 10);

	// Day
	byte_time[3] = ((a_time->tm_mday / 10) << BCD_SHIFT) + (a_time->tm_mday % 10);

	// Weekday
	byte_time[4] = a_time->tm_wday;

	// Month
	byte_time[5] = (((a_time->tm_year - 100) >= 0) << 7) + ((0x1F & (((a_time->tm_mon / 10) << BCD_SHIFT) + (a_time->tm_mon % 10))) + 1U);

	// Year
	const uint8_t year = a_time->tm_year % 100;
	byte_time[6] = ((year / 10) << BCD_SHIFT) + (year % 10);

	const uint16_t addr = a_device->i2c_spec->addr;

	LOG_DBG("PCF8563 @%X", addr);
	LOG_HEXDUMP_DBG(byte_time, sizeof(byte_time), "time and date registers write data");

    // Write values to all time registers
	ret = i2c_burst_write_dt(a_device->i2c_spec, PCF8563_VLSECONDS, byte_time, sizeof(byte_time));
	if (ret != 0)
		LOG_ERR("Could not write time to PCF8563 @%X: %i.", addr, ret);
	else
		LOG_DBG("Wrote time to PCF8563 @%X.", addr);

	k_sem_give(a_device->lock);

    return ret;
}

int pcf8563_set_clkout_frequency(const struct pcf8563_device *a_device, const uint8_t a_frequency)
{
	LOG_MODULE_DECLARE(PCF8563_LOG_PROJECT_NAME, PCF8563_LOG_LEVEL);

	if (k_is_in_isr())
	{
		LOG_ERR("I2C bus is not avaible from interrupt!");
		return -EWOULDBLOCK;
	}

    int ret = 0;

	ret = k_sem_take(a_device->lock, K_FOREVER);
	if (ret != 0)
	{
		LOG_ERR("Could not take semaphore for pcf8563_set_clkout_frequency: %i!", ret);
		return ret;
	}
	else
	{
		LOG_INF("Took semaphore for pcf8563_set_clkout_frequency.");
	}

    uint8_t buffer[] = { PCF8563_CLKOUT, (PCF8563_CLKOUT_FE | a_frequency) };

	ret = i2c_write_dt(a_device->i2c_spec, buffer, sizeof(buffer));
	if (ret != 0)
	{
		LOG_ERR("Could not write CLKOUT frequency %X to PCF8563 @%X: %i!", a_frequency, a_device->i2c_spec->addr, ret);
	}
	else
	{
		LOG_DBG("Wrote CLKOUT frequency %X to PCF8563 @%X.", a_frequency, a_device->i2c_spec->addr);
		LOG_HEXDUMP_DBG(buffer, sizeof(buffer), "CLKOUT_control and clock output register write data");
	}

	k_sem_give(a_device->lock);

	return ret;
}
