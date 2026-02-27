#pragma once

#include "tca6408a.h"


int em_ioexpander_get_devices(void);

int em_ioxpander_0_pin_configure(const gpio_pin_t a_pin, const gpio_flags_t flags);
int em_ioxpander_0_pin_set(const gpio_pin_t a_pin, const bool a_value);
bool em_ioxpander_0_pin_get(const gpio_pin_t a_pin);
int em_ioxpander_0_pin_interrupt_configure(const gpio_pin_t a_pin, const gpio_flags_t flags);
bool em_ioxpander_0_pin_interrupt_status(const gpio_pin_t a_pin);

int em_ioxpander_1_pin_configure(const gpio_pin_t a_pin, const gpio_flags_t flags);
int em_ioxpander_1_pin_set(const gpio_pin_t a_pin, const bool a_value);
bool em_ioxpander_1_pin_get(const gpio_pin_t a_pin);
int em_ioxpander_1_pin_interrupt_configure(const gpio_pin_t a_pin, const gpio_flags_t flags);
bool em_ioxpander_1_pin_interrupt_status(const gpio_pin_t a_pin);
