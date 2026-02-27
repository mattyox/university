#include "em_rtc.h"

#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>

#include "pcf8563.h"
#include "em_ot.h"


#define RTC_LOG_PROJECT_NAME Energy_Meter
#define RTC_LOG_LEVEL LOG_LEVEL_DBG

#define RTC_NODE DT_ALIAS(rtc)
#define GPIO1_PORT DT_NODELABEL(gpio1)

// #define RTC_CLKOUT_PIN 4
#define RTC_CLKOUT_PIN 15


static struct pcf8563_device s_rtc;

K_SEM_DEFINE(lock, 1, 1);


static inline void log_time(const struct tm *a_time)
{
	LOG_MODULE_DECLARE(RTC_LOG_PROJECT_NAME, RTC_LOG_LEVEL);

	LOG_DBG("%d-%d-%d %d:%d:%d", (1900 + a_time->tm_year), (a_time->tm_mon + 1), a_time->tm_mday, a_time->tm_hour, a_time->tm_min, a_time->tm_sec);
}

static void clkout_interrupt_handler(const struct device *a_port, struct gpio_callback *a_cb, gpio_port_pins_t a_pins)
{
	LOG_MODULE_DECLARE(RTC_LOG_PROJECT_NAME, RTC_LOG_LEVEL);

    ARG_UNUSED(a_port);
	ARG_UNUSED(a_cb);
	ARG_UNUSED(a_pins);

	LOG_INF("Called RTC CLKOUT interrupt handler.");

	s_rtc.clock_interrupt = true;
}

static int configure_clock_input(void)
{
	LOG_MODULE_DECLARE(RTC_LOG_PROJECT_NAME, RTC_LOG_LEVEL);

	int ret = 0;

	const uint16_t addr = s_rtc.i2c_spec->addr;

	ret = pcf8563_set_clkout_frequency(&s_rtc, PCF8563_CLKOUT_1HZ);
	if (ret != 0)
	{
		LOG_ERR("Could not set RTC @%X CLKOUT frequency %X: %i!", addr, PCF8563_CLKOUT_1HZ, ret);
		return ret;
	}
	else
	{
		LOG_INF("Set RTC @%X CLKOUT frequency %X.", addr, PCF8563_CLKOUT_1HZ);
	}

	static struct gpio_dt_spec clkout_pin = {
        .port = DEVICE_DT_GET(GPIO1_PORT),
        .pin = RTC_CLKOUT_PIN
    };

	s_rtc.clock_pin = &clkout_pin;
    
    if (!device_is_ready(DEVICE_DT_GET(GPIO1_PORT)))
    {
		LOG_ERR("GPIO port %s for CLKOUT pin of RTC @%X is not ready!", s_rtc.clock_pin->port->name, addr);
        return -ENODEV;
    }
	else
	{
		LOG_INF("GPIO port %s for CLKOUT pin of RTC @%X is ready.", s_rtc.clock_pin->port->name, addr);
	}

	if (!gpio_is_ready_dt(s_rtc.clock_pin))
	{
		LOG_ERR("Could not get pointer to GPIO CLKOUT pin for RTC @%X!", addr);
		return -EINVAL;
	}
	else
	{
		LOG_INF("GPIO CLKOUT pin for RTC @%X is ready.", addr);
	}

	ret = gpio_pin_configure_dt(s_rtc.clock_pin, (GPIO_INPUT | GPIO_PULL_UP | GPIO_ACTIVE_HIGH));
	if (ret != 0)
	{
		LOG_ERR("Could not configure RTC CLKOUT pin @%X: %i!", addr, ret);
		return ret;
	}
	else
	{
		LOG_INF("Configured RTC CLKOUT pin @%X.", addr);
	}

	ret = gpio_pin_interrupt_configure_dt(s_rtc.clock_pin, GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0)
	{
		LOG_ERR("Could not configure RTC CLKOUT interrupt from pin @%X: %i!", addr, ret);
		return ret;
	}
	else
	{
		LOG_INF("Configured RTC CLKOUT interrupt pin @%X.", addr);
	}

	gpio_init_callback(&s_rtc.clock_callback, clkout_interrupt_handler, BIT(s_rtc.clock_pin->pin));

	ret = gpio_add_callback(s_rtc.clock_pin->port, &s_rtc.clock_callback);
	if (ret != 0)
		LOG_ERR("Could not configure RTC CLKOUT callback @%X: %i!", addr, ret);
	else
		LOG_INF("Configured RTC CLKOUT callback @%X.", addr);

	return ret;
}

int em_rtc_get_device(void)
{
    LOG_MODULE_DECLARE(RTC_LOG_PROJECT_NAME, RTC_LOG_LEVEL);

	memset(&s_rtc, 0, sizeof(s_rtc));

	static struct i2c_dt_spec rtc = I2C_DT_SPEC_GET(RTC_NODE);

	s_rtc.i2c_spec = &rtc;

	if (!device_is_ready(s_rtc.i2c_spec->bus))
	{
		LOG_ERR("I2C bus %s for RTC device is not ready!", s_rtc.i2c_spec->bus->name);

		s_rtc.i2c_spec = NULL;

		return -ENODEV;
	}

	LOG_INF("Found RTC device at address \"0x%X\".", s_rtc.i2c_spec->addr);

	s_rtc.lock = &lock;

	return configure_clock_input();
}

int em_rtc_get_time(int64_t *a_unix_epoch)
{
	LOG_MODULE_DECLARE(RTC_LOG_PROJECT_NAME, RTC_LOG_LEVEL);

	int ret = 0;

	struct tm struct_time;
	memset(&struct_time, 0, sizeof(struct_time));

	const uint16_t addr = s_rtc.i2c_spec->addr;

	ret = pcf8563_get_time(&s_rtc, &struct_time);
	if (ret == -EBADMSG)
	{
		LOG_WRN("RTC @%X time integrity is not guaranteed!", addr);

		ret = em_rtc_set_time(&struct_time);
		if (ret != 0)
		{
			LOG_ERR("Could not restore time to RTC @%X: %i!", addr, ret);
			return ret;
		}
		else
		{
			LOG_INF("Restored time to RTC @%X.", addr);
		}
	}
	else if (ret != 0)
	{
		LOG_ERR("Could not get time from RTC @%X: %i!", addr, ret);
		*a_unix_epoch = 0;
		return ret;
	}
	else
	{
		LOG_INF("Got time from RTC @%X.", addr);
	}

	log_time(&struct_time);

	*a_unix_epoch = mktime(&struct_time);

	return ret;
}

int em_rtc_set_time(struct tm *a_struct_time)
{
	LOG_MODULE_DECLARE(RTC_LOG_PROJECT_NAME, RTC_LOG_LEVEL);

	int ret = 0;

	int64_t unix_epoch = 0;

    ret = em_ot_get_sntp_time(&unix_epoch);
	if (ret != 0)
	{
		LOG_ERR("Could not get Unix epoch: %i!", ret);
		return ret;
	}
	else
	{
		LOG_INF("Got Unix epoch.");
	}

	// Example unix epoch
	// unix_epoch = 1701008503; // 2023-11-26 14:21:43

    // Convert unix time to struct tm
	a_struct_time = gmtime(&unix_epoch);

	log_time(a_struct_time);

    ret = pcf8563_set_time(&s_rtc, a_struct_time);
	if (ret != 0)
		LOG_ERR("Could not set time to RTC @%X: %i!", s_rtc.i2c_spec->addr, ret);
	else
		LOG_INF("Set time to RTC @%X.", s_rtc.i2c_spec->addr);

	return ret;
}

void em_rtc_log_time(int64_t a_unix_epoch)
{
	LOG_MODULE_DECLARE(RTC_LOG_PROJECT_NAME, RTC_LOG_LEVEL);

	LOG_INF("Called em_rtc_log_time for Unix epoch: %lli", a_unix_epoch);

	struct tm *struct_time;
    struct_time = gmtime(&a_unix_epoch);

	log_time(struct_time);
}

bool em_rtc_get_interrupt_status(void)
{
	LOG_MODULE_DECLARE(RTC_LOG_PROJECT_NAME, RTC_LOG_LEVEL);

	const bool interrupt_status = s_rtc.clock_interrupt;

	LOG_INF("Returning RTC interrupt status.");
    LOG_DBG("RTC interrupt status: %i.", interrupt_status);

	if (interrupt_status)
	{
		s_rtc.clock_interrupt = false;

		LOG_DBG("Cleared RTC interrupt status.");
	}

    return s_rtc.clock_interrupt;
}
