// Based on zephyr/drivers/gpio/gpio_tca6424a.c by Chromium OS Authors

#pragma once

#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/gpio.h>


#define TCA6408A_INPUT			        0x00
#define TCA6408A_OUTPUT			        0x01
#define TCA6408A_POLARITYINVERSION		0x02
#define TCA6408A_CONFIGURATION		    0x03


struct tca6408a_pins_state {
	uint8_t input;
	uint8_t output;
    uint8_t polarity_inversion;
	uint8_t configuration;
};

struct tca6408a_interrupts_state {
	uint8_t rising;
	uint8_t falling;
	uint8_t status;
};

struct tca6408a_data {
	struct tca6408a_pins_state pins_state;
    struct tca6408a_interrupts_state interrupts_state;
};

struct tca6408a_device {
    struct i2c_dt_spec *i2c_spec;
	struct gpio_dt_spec *reset_pin;
    struct gpio_dt_spec *interrupt_pin;
	struct gpio_callback interrupt_callback;
    struct tca6408a_data data;
	struct k_sem *lock;
};


int tca6408a_initialize(struct tca6408a_device *a_device);
int tca6408a_interrupt_callback_configure(struct tca6408a_device *a_device, gpio_callback_handler_t a_handler);
int tca6408a_pin_configure(struct tca6408a_device *a_device, const gpio_pin_t a_pin, const gpio_flags_t flags);
int tca6408a_pin_set(struct tca6408a_device *a_device, const gpio_pin_t a_pin, const bool a_value);
int tca6408a_pin_get(struct tca6408a_device *a_device, const gpio_pin_t a_pin, bool *a_value);
void tca6408a_interrupt_handler(struct tca6408a_device *a_device);
int tca6408a_pin_interrupt_configure(struct tca6408a_device *a_device, const gpio_pin_t a_pin, const gpio_flags_t flags);
bool tca6408a_pin_interrupt_get_status(struct tca6408a_device *a_device, const gpio_pin_t a_pin);
