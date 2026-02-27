#pragma once

#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>


#define ADE7753_WAVEFORM    0x01
#define ADE7753_AENERGY     0x02
#define ADE7753_RAENERGY    0x03
#define ADE7753_LAENERGY    0x04
#define ADE7753_VAENERGY    0x05
#define ADE7753_RVAENERGY   0x06
#define ADE7753_LVAENERGY   0x07
#define ADE7753_LVARENERGY  0x08
#define ADE7753_MODE        0x09
#define ADE7753_IRQEN       0x0A
#define ADE7753_STATUS      0x0B
#define ADE7753_RSTSTATUS   0x0C
#define ADE7753_CH1OS       0x0D
#define ADE7753_CH2OS       0x0E
#define ADE7753_GAIN        0x0F
#define ADE7753_PHCAL       0x10
#define ADE7753_APOS        0x11
#define ADE7753_WGAIN       0x12
#define ADE7753_WDIV        0x13
#define ADE7753_CFNUM       0x14
#define ADE7753_CFDEN       0x15
#define ADE7753_IRMS        0x16
#define ADE7753_VRMS        0x17
#define ADE7753_IRMSOS      0x18
#define ADE7753_VRMSOS      0x19
#define ADE7753_VAGAIN      0x1A
#define ADE7753_VADIV       0x1B
#define ADE7753_LINECYC     0x1C
#define ADE7753_ZXTOUT      0x1D
#define ADE7753_SAGCYC      0x1E
#define ADE7753_SAGLVL      0x1F
#define ADE7753_IPKLVL      0x20
#define ADE7753_VPKLVL      0x21
#define ADE7753_IPEAK       0x22
#define ADE7753_RSTIPEAK    0x23
#define ADE7753_VPEAK       0x24
#define ADE7753_RSTVPEAK    0x25
#define ADE7753_TEMP        0x26
#define ADE7753_PERIOD      0x27
#define ADE7753_TMODE       0x3D
#define ADE7753_CHKSUM      0x3E
#define ADE7753_DIEREV      0x3F


struct ade7753_readings {
    double active_energy;
    double apparent_energy;
    double reactive_energy;
    double current_rms;
    double voltage_rms;
};

struct ade7753_device {
    struct spi_dt_spec *spi_spec;
    struct gpio_dt_spec *cs;
    struct ade7753_readings readings;
	struct k_sem *lock;
};


int ade7753_reset(struct ade7753_device *a_device);
int ade7753_initialize(struct ade7753_device *a_device);
int ade7753_get_active_energy(struct ade7753_device *a_device);
int ade7753_get_apparent_energy(struct ade7753_device *a_device);
void ade7753_get_reactive_energy(struct ade7753_device *a_device);
int ade7753_get_current_rms(struct ade7753_device *a_device);
int ade7753_get_voltage_rms(struct ade7753_device *a_device);
int ade7753_get_readings(struct ade7753_device *a_device);
