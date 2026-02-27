// Based on zephyr/drivers/gpio/gpio_tca6424a.c by Chromium OS Authors

#include "tca6408a.h"

#include <zephyr/logging/log.h>


#define TCA6408A_LOG_PROJECT_NAME Energy_Meter
#define TCA6408A_LOG_LEVEL LOG_LEVEL_ERR


static int read_register(const struct i2c_dt_spec *a_i2c_spec, const uint8_t a_register, uint8_t *a_register_data)
{
    LOG_MODULE_DECLARE(TCA6408A_LOG_PROJECT_NAME, TCA6408A_LOG_LEVEL);

	int ret = 0;

	ret = i2c_write_read_dt(a_i2c_spec, &a_register, sizeof(a_register), a_register_data, sizeof(*a_register_data));
	if (ret != 0)
		LOG_ERR("Could not read from TCA6408A register 0x%X at @%X: %d!", a_register, a_i2c_spec->addr, ret);
	else
    	LOG_DBG("Read data 0x%X from TCA6408A register 0x%X @%X.", *a_register_data, a_register, a_i2c_spec->addr);

	return ret;
}

static int write_register(const struct i2c_dt_spec *a_i2c_spec, const uint8_t a_register, const uint8_t a_register_data)
{
    LOG_MODULE_DECLARE(TCA6408A_LOG_PROJECT_NAME, TCA6408A_LOG_LEVEL);

    int ret = 0;

    uint8_t buffer[] = { a_register, a_register_data };

	ret = i2c_write_dt(a_i2c_spec, buffer, sizeof(buffer));
	if (ret != 0)
		LOG_ERR("Could not write to TCA6408A register 0x%X @%X: %i!", a_register, a_i2c_spec->addr, ret);
	else
		LOG_DBG("Wrote data 0x%X to TCA6408A register 0x%X @%X.", a_register_data, a_register, a_i2c_spec->addr);

	return ret;
}

static int update_input_register(struct tca6408a_device *a_device, uint8_t *a_register_data)
{
	LOG_MODULE_DECLARE(TCA6408A_LOG_PROJECT_NAME, TCA6408A_LOG_LEVEL);

	int ret = 0;

	ret = read_register(a_device->i2c_spec, TCA6408A_INPUT, a_register_data);
	if (ret != 0)
    {
		LOG_ERR("Could not update TCA6408A input register @%X: %i!", a_device->i2c_spec->addr, ret);
	}
    else
    {
        a_device->data.pins_state.input = *a_register_data;

        LOG_DBG("Updated TCA6408A input register @%X.", a_device->i2c_spec->addr);
    }

	return ret;
}

static int update_output_register(struct tca6408a_device *a_device, const uint8_t a_register_data)
{
	LOG_MODULE_DECLARE(TCA6408A_LOG_PROJECT_NAME, TCA6408A_LOG_LEVEL);

	int ret = 0;

	ret = write_register(a_device->i2c_spec, TCA6408A_OUTPUT, a_register_data);
	if (ret != 0)
    {
		LOG_ERR("Could not update TCA6408A output register @%X: %i!", a_device->i2c_spec->addr, ret);
	}
    else
    {
        a_device->data.pins_state.output = a_register_data;

        LOG_DBG("Updated TCA6408A output register @%X.", a_device->i2c_spec->addr);
    }

	return ret;
}

static int update_polarityinversion_register(struct tca6408a_device *a_device, const uint8_t a_register_data)
{
    LOG_MODULE_DECLARE(TCA6408A_LOG_PROJECT_NAME, TCA6408A_LOG_LEVEL);

	int ret = 0;

	ret = write_register(a_device->i2c_spec, TCA6408A_POLARITYINVERSION, a_register_data);
	if (ret != 0)
    {
		LOG_ERR("Could not update TCA6408A polarity inversion register @%X: %i!", a_device->i2c_spec->addr, ret);
	}
    else
    {
        a_device->data.pins_state.polarity_inversion = a_register_data;

        LOG_DBG("Updated TCA6408A polarity inversion register @%X.", a_device->i2c_spec->addr);
    }

	return ret;
}

static int update_config_register(struct tca6408a_device *a_device, const uint8_t a_register_data)
{
    LOG_MODULE_DECLARE(TCA6408A_LOG_PROJECT_NAME, TCA6408A_LOG_LEVEL);

	int ret = 0;

	ret = write_register(a_device->i2c_spec, TCA6408A_CONFIGURATION, a_register_data);
	if (ret != 0)
    {
		LOG_ERR("Could not update TCA6408A config register @%X: %i!", a_device->i2c_spec->addr, ret);
	}
    else
    {
		a_device->data.pins_state.configuration = a_register_data;

        LOG_DBG("Updated TCA6408A config register @%X.", a_device->i2c_spec->addr);
    }

	return ret;
}

int tca6408a_initialize(struct tca6408a_device *a_device)
{
	LOG_MODULE_DECLARE(TCA6408A_LOG_PROJECT_NAME, TCA6408A_LOG_LEVEL);

	struct gpio_dt_spec *reset_pin = a_device->reset_pin;
	const uint16_t addr = a_device->i2c_spec->addr;

	if (!gpio_is_ready_dt(reset_pin))
	{
		LOG_ERR("Could not get pointer to GPIO reset device for TCA6408A @%X!", addr);
		return -EINVAL;
	}
	else
	{
		LOG_INF("GPIO reset device for TCA6408A @%X is ready.", addr);
	}

	int ret = 0;

	ret = gpio_pin_configure_dt(reset_pin, (GPIO_ACTIVE_LOW | GPIO_OUTPUT_ACTIVE));
	if (ret != 0)
	{
		LOG_ERR("Could not configure TCA6408A reset pin @%X: %i!", addr, ret);
		return ret;
	}
	else
	{
		LOG_INF("Configured TCA6408A reset pin @%X.", addr);
	}

	// Time to reset device
	k_usleep(1);

	ret = gpio_pin_set_dt(reset_pin, 0);
	if (ret != 0)
	{
		LOG_ERR("Could not deactivate TCA6408A reset pin @%X: %i!", addr, ret);
		return ret;
	}
	else
	{
		LOG_INF("Deactivated TCA6408A reset pin @%X.", addr);
	}

	// Time to recover from reset state
	k_usleep(1);

	// Initialize registers
	ret = update_config_register(a_device, 0xFF);
	if (ret != 0)
	{
		LOG_ERR("Could not initialize TCA6408A configuration register @%X: %i!", addr, ret);
		return ret;
	}
	else
	{
		LOG_INF("Initialized TCA6408A configuration register @%X.", addr);
	}

	ret = update_output_register(a_device, 0x00);
	if (ret != 0)
	{
		LOG_ERR("Could not initialize TCA6408A output register @%X: %i!", addr, ret);
		return ret;
	}
	else
	{
		LOG_INF("Initialized TCA6408A output register @%X.", addr);
	}

	ret = update_polarityinversion_register(a_device, 0x00);
	if (ret != 0)
	{
		LOG_ERR("Could not initialize TCA6408A polarity inversion register @%X: %i!", addr, ret);
		return ret;
	}
	else
	{
		LOG_INF("Initialized TCA6408A polarity inversion register @%X.", addr);
	}

	ret = update_input_register(a_device, &a_device->data.pins_state.input);
	if (ret != 0)
	{
		LOG_ERR("Could not initialize TCA6408A input register @%X: %i!", addr, ret);
		return ret;
	}
	else
	{
		LOG_INF("Initialized TCA6408A input register @%X.", addr);
	}

	struct gpio_dt_spec *interrupt_pin = a_device->interrupt_pin;

	a_device->data.interrupts_state.falling = 0;
	a_device->data.interrupts_state.rising = 0;

	if (!gpio_is_ready_dt(interrupt_pin))
	{
		LOG_ERR("Could not get pointer to GPIO interrupt device for TCA6408A @%X!", addr);
		return -EINVAL;
	}
	else
	{
		LOG_INF("GPIO interrupt device for TCA6408A @%X is ready.", addr);
	}

	ret = gpio_pin_configure_dt(interrupt_pin, (GPIO_INPUT | GPIO_PULL_UP | GPIO_ACTIVE_LOW));
	if (ret != 0)
	{
		LOG_ERR("Could not configure TCA6408A interrupt pin @%X: %i!", addr, ret);
		return ret;
	}
	else
	{
		LOG_INF("Configured TCA6408A interrupt pin @%X.", addr);
	}

	ret = gpio_pin_interrupt_configure_dt(interrupt_pin, GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0)
	{
		LOG_ERR("Could not configure TCA6408A interrupt from pin @%X: %i!", addr, ret);
		return ret;
	}
	else
	{
		LOG_INF("Configured TCA6408A interrupt pin @%X.", addr);
	}

	LOG_INF("Initialized TCA6408A @%X.", addr);

	return ret;
}

int tca6408a_interrupt_callback_configure(struct tca6408a_device *a_device, gpio_callback_handler_t a_handler)
{
	LOG_MODULE_DECLARE(TCA6408A_LOG_PROJECT_NAME, TCA6408A_LOG_LEVEL);

	gpio_init_callback(&a_device->interrupt_callback, a_handler, BIT(a_device->interrupt_pin->pin));

	int ret = 0;

	ret = gpio_add_callback(a_device->interrupt_pin->port, &a_device->interrupt_callback);
	if (ret != 0)
		LOG_ERR("Could not configure TCA6408A interrupt callback @%X: %i!", a_device->i2c_spec->addr, ret);
	else
		LOG_INF("Configured TCA6408A interrupt callback @%X.", a_device->i2c_spec->addr);

	return ret;
}

int tca6408a_pin_configure(struct tca6408a_device *a_device, const gpio_pin_t a_pin, const gpio_flags_t flags)
{
	LOG_MODULE_DECLARE(TCA6408A_LOG_PROJECT_NAME, TCA6408A_LOG_LEVEL);

	if (k_is_in_isr())
	{
		LOG_ERR("I2C bus is not avaible from interrupt!");
		return -EWOULDBLOCK;
	}

	int ret = 0;

	ret = k_sem_take(a_device->lock, K_FOREVER);
	if (ret != 0)
	{
		LOG_ERR("Could not take semaphore for tca6408a_pin_configure: %i!", ret);
		return ret;
	}
	else
	{
		LOG_INF("Took semaphore for tca6408a_pin_configure.");
	}

	uint8_t configuration = a_device->data.pins_state.configuration;
	uint8_t output = a_device->data.pins_state.output;

	if ((flags & GPIO_INPUT) != 0)
	{
		configuration |= BIT(a_pin);
	}
	else if ((flags & GPIO_OUTPUT) != 0)
	{
		configuration &= ~BIT(a_pin);

		if ((flags & GPIO_OUTPUT_HIGH) != 0)
		{
			output |= BIT(a_pin);
		}
		else if ((flags & GPIO_OUTPUT_LOW) != 0)
		{
			output &= ~BIT(a_pin);
		}
		else
		{
			LOG_ERR("GPIO flags \"%i\" not supported by TCA6408A!", flags);

			k_sem_give(a_device->lock);

			return -ENOTSUP;
		}

		ret = update_output_register(a_device, output);
		if (ret != 0)
		{
			LOG_ERR("Could not set value \"%i\" for pin %i TCA6408A @%X: %i!", flags, a_pin, a_device->i2c_spec->addr, ret);

			k_sem_give(a_device->lock);

			return ret;
		}
		else
		{
			LOG_INF("Set value \"%i\" for pin %i TCA6408A @%X.", flags, a_pin, a_device->i2c_spec->addr);
		}
	}
	else
	{
		LOG_ERR("GPIO flags \"%i\" not supported by TCA6408A!", flags);

		k_sem_give(a_device->lock);

		return -ENOTSUP;
	}

	ret = update_config_register(a_device, configuration);
	if (ret != 0)
		LOG_ERR("Could not set configuration \"%i\" for pin %i TCA6408A @%X: %i!", flags, a_pin, a_device->i2c_spec->addr, ret);
	else
		LOG_INF("Set configuration \"%i\" for pin %i TCA6408A @%X.", flags, a_pin, a_device->i2c_spec->addr);

	k_sem_give(a_device->lock);

	return ret;
}

int tca6408a_pin_set(struct tca6408a_device *a_device, const gpio_pin_t a_pin, const bool a_value)
{
	LOG_MODULE_DECLARE(TCA6408A_LOG_PROJECT_NAME, TCA6408A_LOG_LEVEL);

	if (k_is_in_isr())
	{
		LOG_ERR("I2C bus is not avaible from interrupt!");
		return -EWOULDBLOCK;
	}

	int ret = 0;

	ret = k_sem_take(a_device->lock, K_FOREVER);
	if (ret != 0)
	{
		LOG_ERR("Could not take semaphore for tca6408a_pin_set: %i!", ret);
		return ret;
	}
	else
	{
		LOG_INF("Took semaphore for tca6408a_pin_set.");
	}

	uint8_t output = a_device->data.pins_state.output;

	if (a_value == 0)
		output &= ~BIT(a_pin);
	else
		output |= BIT(a_pin);

	ret = update_output_register(a_device, output);
	if (ret != 0)
		LOG_ERR("Could not set value \"%i\" to pin %i!", a_value, a_pin);
	else
		LOG_INF("Set value \"%i\" to pin %i.", a_value, a_pin);

	k_sem_give(a_device->lock);

	return ret;
}

int tca6408a_pin_get(struct tca6408a_device *a_device, const gpio_pin_t a_pin, bool *a_value)
{
	LOG_MODULE_DECLARE(TCA6408A_LOG_PROJECT_NAME, TCA6408A_LOG_LEVEL);

	if (k_is_in_isr())
	{
		LOG_ERR("I2C bus is not avaible from interrupt!");
		return -EWOULDBLOCK;
	}

	int ret = 0;

	ret = k_sem_take(a_device->lock, K_FOREVER);
	if (ret != 0)
	{
		LOG_ERR("Could not take semaphore for tca6408a_pin_get: %i!", ret);
		return ret;
	}
	else
	{
		LOG_INF("Took semaphore for tca6408a_pin_get.");
	}

	uint8_t buffer = 0;

	ret = update_input_register(a_device, &buffer);
	if (ret != 0)
	{
		LOG_ERR("Could not get value from pin %i!", a_pin);
	}
	else
	{
		*a_value = buffer & BIT(a_pin);
		LOG_INF("Got value \"%i\" from pin %i.", *a_value, a_pin);
	}

	k_sem_give(a_device->lock);

	return ret;
}

void tca6408a_interrupt_handler(struct tca6408a_device *a_device)
{
	LOG_MODULE_DECLARE(TCA6408A_LOG_PROJECT_NAME, TCA6408A_LOG_LEVEL);

	if (!a_device->data.interrupts_state.rising && !a_device->data.interrupts_state.falling)
	{
		LOG_WRN("No interrupts were configured for TCA6408A @%X!", a_device->i2c_spec->addr);
		return;
	}

	int ret = 0;

	ret = k_sem_take(a_device->lock, K_FOREVER);
	if (ret != 0)
	{
		LOG_ERR("Could not take semaphore for tca6408a_interrupt_handler: %i!", ret);
		return;
	}
	else
	{
		LOG_INF("Took semaphore for tca6408a_interrupt_handler.");
	}

	uint8_t previous_states = a_device->data.pins_state.input;

	uint8_t current_states = 0;

	ret = update_input_register(a_device, &current_states);
	if (ret != 0)
	{
		LOG_ERR("Could not read input states for TCA6408A @%X: %i!", a_device->i2c_spec->addr, ret);
		k_sem_give(a_device->lock);
		return;
	}
	else
	{
		LOG_INF("Read input states for TCA6408A @%X.", a_device->i2c_spec->addr);
	}

	uint8_t changed_pins = previous_states ^ current_states;

	a_device->data.interrupts_state.status |= a_device->data.interrupts_state.rising & (changed_pins & current_states);
	a_device->data.interrupts_state.status |= a_device->data.interrupts_state.falling & (changed_pins & previous_states);

	LOG_INF("Updated interrupt status for TCA6408A @%X.", a_device->i2c_spec->addr);

	k_sem_give(a_device->lock);
}

int tca6408a_pin_interrupt_configure(struct tca6408a_device *a_device, const gpio_pin_t a_pin, const gpio_flags_t flags)
{
	LOG_MODULE_DECLARE(TCA6408A_LOG_PROJECT_NAME, TCA6408A_LOG_LEVEL);

	struct tca6408a_interrupts_state *interrupts = &a_device->data.interrupts_state;

	int ret = 0;

	ret = k_sem_take(a_device->lock, K_FOREVER);
	if (ret != 0)
	{
		LOG_ERR("Could not take semaphore for tca6408a_pin_interrupt_configure: %i!", ret);
		return ret;
	}
	else
	{
		LOG_INF("Took semaphore for tca6408a_pin_interrupt_configure.");
	}

	if (flags == GPIO_INT_DISABLE)
	{
		interrupts->falling &= ~BIT(a_pin);
		interrupts->rising &= ~BIT(a_pin);

		LOG_INF("Disabled interrupt for pin %i.", a_pin);
	}
	else if (flags == GPIO_INT_EDGE_RISING)
	{
		interrupts->falling &= ~BIT(a_pin);
		interrupts->rising |= BIT(a_pin);

		LOG_INF("Set rising edge interrupt for pin %i.", a_pin);
	}
	else if (flags == GPIO_INT_EDGE_FALLING)
	{
		interrupts->falling |= BIT(a_pin);
		interrupts->rising &= ~BIT(a_pin);

		LOG_INF("Set falling edge interrupt for pin %i.", a_pin);
	}
	else if (flags == GPIO_INT_EDGE_BOTH)
	{
		interrupts->falling |= BIT(a_pin);
		interrupts->rising |= BIT(a_pin);

		LOG_INF("Set both rising and falling edge interrupt for pin %i.", a_pin);
	}
	else
	{
		LOG_ERR("Interrupt flags \"%i\" not supported by TCA6408A!", flags);

		k_sem_give(a_device->lock);

		return -ENOTSUP;
	}

	k_sem_give(a_device->lock);

	return 0;
}

bool tca6408a_pin_interrupt_get_status(struct tca6408a_device *a_device, const gpio_pin_t a_pin)
{
	LOG_MODULE_DECLARE(TCA6408A_LOG_PROJECT_NAME, TCA6408A_LOG_LEVEL);

	bool state = a_device->data.interrupts_state.status & BIT(a_pin);
	LOG_DBG("Got interrupt state \"%i\" for pin %i TCA6408A @%X.", state, a_pin, a_device->i2c_spec->addr);

	if (state)
	{
		a_device->data.interrupts_state.status &= !BIT(a_pin);
		LOG_INF("Cleared interrupt status for pin %i TCA6408A @%X.", a_pin, a_device->i2c_spec->addr);
	}
	else
	{
		LOG_INF("Not cleared interrupt status for pin %i TCA6408A @%X.", a_pin, a_device->i2c_spec->addr);
	}

	return state;
}
