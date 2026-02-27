#pragma once

#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/gpio.h>

#include <time.h>


// Control and status registers
#define PCF8563_CONTROL1            0x00
#define PCF8563_CONTROL1_TEST1      BIT(7)
#define PCF8563_CONTROL1_STOP       BIT(5)
#define PCF8563_CONTROL1_TESTC      BIT(3)

#define PCF8563_CONTROL2            0x01
#define PCF8563_CONTROL2_TITP       BIT(4)
#define PCF8563_CONTROL2_AF         BIT(3)
#define PCF8563_CONTROL2_TF         BIT(2)
#define PCF8563_CONTROL2_AIE        BIT(1)
#define PCF8563_CONTROL2_TIE        BIT(0)

// Time and date registers
#define PCF8563_VLSECONDS           0x02
#define PCF8563_VLSECONDS_VL        BIT(7)

#define PCF8563_MINUTES             0x03

#define PCF8563_HOURS               0x04

#define PCF8563_DAYS                0x05

#define PCF8563_WEEKDAYS            0x06

#define PCF8563_CENTURYMONTHS       0x07
#define PCF8563_CENTURYMONTHS_C     BIT(7)

#define PCF8563_YEAR                0x08

// Alarm registers
#define PCF8563_MINUTEALARM         0x09
#define PCF8563_MINUTEALARM_AEM     BIT(7)

#define PCF8563_HOURALARM           0x0A
#define PCF8563_HOURALARM_AEH       BIT(7)

#define PCF8563_DAYALARM            0x0B
#define PCF8563_DAYALARM_AED        BIT(7)

#define PCF8563_WEEKDAYALARM        0x0C
#define PCF8563_WEEKDAYALARM_AEW    BIT(7)

// CLKOUT control register
#define PCF8563_CLKOUT              0x0D
#define PCF8563_CLKOUT_FE           BIT(7)
#define PCF8563_CLKOUT_32KHZ768     0x00
#define PCF8563_CLKOUT_1KHZ024      0x01
#define PCF8563_CLKOUT_32HZ         0x02
#define PCF8563_CLKOUT_1HZ          0x03

// Timer registers
#define PCF8563_TIMERCONTROL        0x0E
#define PCF8563_TIMERCONTROL_TE     BIT(7)

// Timer
#define PCF8563_TIMER               0x0F

// BCD helpers
#define BCD_UNITS                   0x0F
#define BCD_SHIFT                   4U


struct pcf8563_device {
    struct i2c_dt_spec *i2c_spec;
    struct gpio_dt_spec *clock_pin;
    struct gpio_callback clock_callback;
    bool clock_interrupt;
	struct k_sem *lock;
};


int pcf8563_get_time(const struct pcf8563_device *a_device, struct tm *a_time);
int pcf8563_set_time(const struct pcf8563_device *a_device, const struct tm *a_time);
int pcf8563_set_clkout_frequency(const struct pcf8563_device *a_device, const uint8_t a_frequency);
