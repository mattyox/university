#include "em_ioexpander.h"

#include <zephyr/logging/log.h>


#define IOEXPANDER_LOG_PROJECT_NAME Energy_Meter
#define IOEXPANDER_LOG_LEVEL LOG_LEVEL_ERR

#define IOEXPANDER_0_NODE DT_ALIAS(ioexpander0)
#define IOEXPANDER_1_NODE DT_ALIAS(ioexpander1)

#define GPIO0_PORT DT_NODELABEL(gpio0)
#define GPIO1_PORT DT_NODELABEL(gpio1)

// #define IOEXPANDER_0_RESET_PIN 5
// #define IOEXPANDER_0_INT_PIN 6
#define IOEXPANDER_0_RESET_PIN 10
#define IOEXPANDER_0_INT_PIN 9

// #define IOEXPANDER_1_RESET_PIN 7
// #define IOEXPANDER_1_INT_PIN 8
#define IOEXPANDER_1_RESET_PIN 10
#define IOEXPANDER_1_INT_PIN 13


static struct tca6408a_device s_ioexpander_0;
static struct tca6408a_device s_ioexpander_1;

K_SEM_DEFINE(io_lock_0, 1, 1);
K_SEM_DEFINE(io_lock_1, 1, 1);


static void ioexpander_0_get_device(void)
{
    LOG_MODULE_DECLARE(IOEXPANDER_LOG_PROJECT_NAME, IOEXPANDER_LOG_LEVEL);

	memset(&s_ioexpander_0, 0, sizeof(s_ioexpander_0));

	static struct i2c_dt_spec ioexpander = I2C_DT_SPEC_GET(IOEXPANDER_0_NODE);

	s_ioexpander_0.i2c_spec = &ioexpander;

	if (!device_is_ready(s_ioexpander_0.i2c_spec->bus))
	{
		LOG_ERR("I2C bus %s for I/O Expander 0 device is not ready!", s_ioexpander_0.i2c_spec->bus->name);

		s_ioexpander_0.i2c_spec = NULL;

		return;
	}

	LOG_INF("Found I/O Expander 0 device at address \"0x%X\".", s_ioexpander_0.i2c_spec->addr);
}

static int ioexpander_0_get_pins(void)
{
	LOG_MODULE_DECLARE(IOEXPANDER_LOG_PROJECT_NAME, IOEXPANDER_LOG_LEVEL);

	static struct gpio_dt_spec reset_pin = {
        // .port = DEVICE_DT_GET(GPIO1_PORT),
		.port = DEVICE_DT_GET(GPIO0_PORT),
        .pin = IOEXPANDER_0_RESET_PIN
    };

	static struct gpio_dt_spec interrupt_pin = {
        // .port = DEVICE_DT_GET(GPIO1_PORT),
		.port = DEVICE_DT_GET(GPIO0_PORT),
        .pin = IOEXPANDER_0_INT_PIN
    };
    
    if (!device_is_ready(DEVICE_DT_GET(GPIO1_PORT)))
    {
        LOG_ERR("GPIO port %s for reset and interrupt pin of I/O Expander 0 is not ready!", reset_pin.port->name);
        return -ENODEV;
    }

	s_ioexpander_0.reset_pin = &reset_pin;
	s_ioexpander_0.interrupt_pin = &interrupt_pin;

    LOG_INF("Found I/O Expander 0 configuration pins.");

	return 0;
}

static void ioexpander_1_get_device(void)
{
    LOG_MODULE_DECLARE(IOEXPANDER_LOG_PROJECT_NAME, IOEXPANDER_LOG_LEVEL);

	memset(&s_ioexpander_1, 0, sizeof(s_ioexpander_1));

	static struct i2c_dt_spec ioexpander = I2C_DT_SPEC_GET(IOEXPANDER_1_NODE);

	s_ioexpander_1.i2c_spec = &ioexpander; 

	if (!device_is_ready(s_ioexpander_1.i2c_spec->bus))
	{
		LOG_ERR("I2C bus %s for I/O Expander 1 device is not ready!", s_ioexpander_1.i2c_spec->bus->name);

		s_ioexpander_1.i2c_spec = NULL;

		return;
	}

	LOG_INF("Found I/O Expander 1 device at address \"0x%X\".", s_ioexpander_1.i2c_spec->addr);
}

static int ioexpander_1_get_pins(void)
{
	LOG_MODULE_DECLARE(IOEXPANDER_LOG_PROJECT_NAME, IOEXPANDER_LOG_LEVEL);

	static struct gpio_dt_spec reset_pin = {
        .port = DEVICE_DT_GET(GPIO1_PORT),
        .pin = IOEXPANDER_1_RESET_PIN
    };

	static struct gpio_dt_spec interrupt_pin = {
        .port = DEVICE_DT_GET(GPIO1_PORT),
        .pin = IOEXPANDER_1_INT_PIN
    };
    
    if (!device_is_ready(DEVICE_DT_GET(GPIO1_PORT)))
    {
        LOG_ERR("GPIO port %s for reset and interrupt pin of I/O Expander 1 is not ready!", reset_pin.port->name);
        return -ENODEV;
    }

	s_ioexpander_1.reset_pin = &reset_pin;
	s_ioexpander_1.interrupt_pin = &interrupt_pin;

    LOG_INF("Found I/O Expander 1 configuration pins.");

	return 0;
}

static inline void ioexpander_0_interrupt_handler(const struct device *a_port, struct gpio_callback *a_cb, gpio_port_pins_t a_pins)
{
	LOG_MODULE_DECLARE(IOEXPANDER_LOG_PROJECT_NAME, IOEXPANDER_LOG_LEVEL);

    ARG_UNUSED(a_port);
	ARG_UNUSED(a_cb);
	ARG_UNUSED(a_pins);

	LOG_INF("Called I/O Expander 0 interrupt handler.");

	tca6408a_interrupt_handler(&s_ioexpander_0);
}

static inline void ioexpander_1_interrupt_handler(const struct device *a_port, struct gpio_callback *a_cb, gpio_port_pins_t a_pins)
{
	LOG_MODULE_DECLARE(IOEXPANDER_LOG_PROJECT_NAME, IOEXPANDER_LOG_LEVEL);

    ARG_UNUSED(a_port);
	ARG_UNUSED(a_cb);
	ARG_UNUSED(a_pins);

	LOG_INF("Called I/O Expander 1 interrupt handler.");

	tca6408a_interrupt_handler(&s_ioexpander_1);
}

static int initialize_devices(void)
{
	LOG_MODULE_DECLARE(IOEXPANDER_LOG_PROJECT_NAME, IOEXPANDER_LOG_LEVEL);

	int ret = 0;

	ret = tca6408a_initialize(&s_ioexpander_0);
	if (ret != 0)
	{
		LOG_ERR("Could not initialize I/O Expander 0!");
		return ret;
	}
	else
	{
		LOG_INF("Initialized I/O Expander 0.");
	}

	s_ioexpander_0.lock = &io_lock_0;

	ret = tca6408a_interrupt_callback_configure(&s_ioexpander_0, ioexpander_0_interrupt_handler);
	if (ret != 0)
	{
		LOG_ERR("Could not configure interrupt for I/O Expander 0!");
		return ret;
	}
	else
	{
		LOG_INF("Configured interrupt for I/O Expander 0.");
	}

	ret = tca6408a_initialize(&s_ioexpander_1);
	if (ret != 0)
	{
		LOG_ERR("Could not initialize I/O Expander 1!");
		return ret;
	}
	else
	{
		LOG_INF("Initialized I/O Expander 1.");
	}

	s_ioexpander_1.lock = &io_lock_1;

	ret = tca6408a_interrupt_callback_configure(&s_ioexpander_1, ioexpander_1_interrupt_handler);
	if (ret != 0)
	{
		LOG_ERR("Could not configure interrupt for I/O Expander 1!");
		return ret;
	}
	else
	{
		LOG_INF("Configured interrupt for I/O Expander 1.");
	}

	return ret;
}

int em_ioexpander_get_devices(void)
{
	LOG_MODULE_DECLARE(IOEXPANDER_LOG_PROJECT_NAME, IOEXPANDER_LOG_LEVEL);

	int ret = 0;

    ioexpander_0_get_device();
	ret = ioexpander_0_get_pins();
	if (ret != 0)
	{
		LOG_ERR("Could not get pins for I/O Expander 0!");
		return ret;
	}
	else
	{
		LOG_INF("Got pins for I/O Expander 0.");
	}

    ioexpander_1_get_device();
	ret = ioexpander_1_get_pins();
	if (ret != 0)
	{
		LOG_ERR("Could not get pins for I/O Expander 1!");
		return ret;
	}
	else
	{
		LOG_INF("Got pins for I/O Expander 1.");
	}

	LOG_INF("Got I/O Expander devices.");

	ret = initialize_devices();
	if (ret != 0)
		LOG_ERR("Could not initialize I/O Expander devices!");
	else
		LOG_INF("Initialized I/O Expander devices.");

	return ret;
}

int em_ioxpander_0_pin_configure(const gpio_pin_t a_pin, const gpio_flags_t flags)
{
	LOG_MODULE_DECLARE(IOEXPANDER_LOG_PROJECT_NAME, IOEXPANDER_LOG_LEVEL);

	LOG_INF("Configure I/O Expander 0 pin %i with flags %i.", a_pin, flags);

	return tca6408a_pin_configure(&s_ioexpander_0, a_pin, flags);
}

int em_ioxpander_0_pin_set(const gpio_pin_t a_pin, const bool a_value)
{
	LOG_MODULE_DECLARE(IOEXPANDER_LOG_PROJECT_NAME, IOEXPANDER_LOG_LEVEL);

	LOG_INF("Set I/O Expander 0 pin %i with value %i.", a_pin, a_value);

	return tca6408a_pin_set(&s_ioexpander_0, a_pin, a_value);
}

bool em_ioxpander_0_pin_get(const gpio_pin_t a_pin)
{
	LOG_MODULE_DECLARE(IOEXPANDER_LOG_PROJECT_NAME, IOEXPANDER_LOG_LEVEL);

	bool a_value = false;

	if (tca6408a_pin_get(&s_ioexpander_0, a_pin, &a_value) != 0)
		LOG_ERR("Could not get pin %i value from I/O Expander 0 @%X!", a_pin, s_ioexpander_0.i2c_spec->addr);
	else
		LOG_DBG("Got I/O Expander 0 pin %i value %i.", a_pin, a_value);

	return a_value;
}

int em_ioxpander_0_pin_interrupt_configure(const gpio_pin_t a_pin, const gpio_flags_t flags)
{
	LOG_MODULE_DECLARE(IOEXPANDER_LOG_PROJECT_NAME, IOEXPANDER_LOG_LEVEL);

	LOG_INF("Configure I/O Expander 0 interrupt for pin %i with flags %i.", a_pin, flags);

	return tca6408a_pin_interrupt_configure(&s_ioexpander_0, a_pin, flags);
}

bool em_ioxpander_0_pin_interrupt_status(const gpio_pin_t a_pin)
{
	LOG_MODULE_DECLARE(IOEXPANDER_LOG_PROJECT_NAME, IOEXPANDER_LOG_LEVEL);

	LOG_INF("Get I/O Expander 0 interrupt status for pin %i.", a_pin);

	return tca6408a_pin_interrupt_get_status(&s_ioexpander_0, a_pin);
}

int em_ioxpander_1_pin_configure(const gpio_pin_t a_pin, const gpio_flags_t flags)
{
	LOG_MODULE_DECLARE(IOEXPANDER_LOG_PROJECT_NAME, IOEXPANDER_LOG_LEVEL);

	LOG_INF("Configure I/O Expander 1 pin %i with flags %i.", a_pin, flags);

	return tca6408a_pin_configure(&s_ioexpander_1, a_pin, flags);
}

int em_ioxpander_1_pin_set(const gpio_pin_t a_pin, const bool a_value)
{
	LOG_MODULE_DECLARE(IOEXPANDER_LOG_PROJECT_NAME, IOEXPANDER_LOG_LEVEL);

	LOG_INF("Set I/O Expander 1 pin %i with value %i.", a_pin, a_value);

	return tca6408a_pin_set(&s_ioexpander_1, a_pin, a_value);
}

bool em_ioxpander_1_pin_get(const gpio_pin_t a_pin)
{
	LOG_MODULE_DECLARE(IOEXPANDER_LOG_PROJECT_NAME, IOEXPANDER_LOG_LEVEL);

	bool a_value = false;

	if (tca6408a_pin_get(&s_ioexpander_1, a_pin, &a_value) != 0)
		LOG_ERR("Could not get pin %i value from I/O Expander 1 @%X!", a_pin, s_ioexpander_1.i2c_spec->addr);
	else
		LOG_DBG("Got I/O Expander 1 pin %i value %i.", a_pin, a_value);

	return a_value;
}

int em_ioxpander_1_pin_interrupt_configure(const gpio_pin_t a_pin, const gpio_flags_t flags)
{
	LOG_MODULE_DECLARE(IOEXPANDER_LOG_PROJECT_NAME, IOEXPANDER_LOG_LEVEL);

	LOG_INF("Configure I/O Expander 1 interrupt for pin %i with flags %i.", a_pin, flags);

	return tca6408a_pin_interrupt_configure(&s_ioexpander_1, a_pin, flags);
}

bool em_ioxpander_1_pin_interrupt_status(const gpio_pin_t a_pin)
{
	LOG_MODULE_DECLARE(IOEXPANDER_LOG_PROJECT_NAME, IOEXPANDER_LOG_LEVEL);

	LOG_INF("Get I/O Expander 1 interrupt status for pin %i.", a_pin);

	return tca6408a_pin_interrupt_get_status(&s_ioexpander_1, a_pin);
}
