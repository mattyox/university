#pragma once

#include <zephyr/drivers/eeprom.h>


#define EUI_OFFSET 0U
#define BOOT_TIME_OFFSET 8U
#define RESET_TIME_OFFSET 16U
#define READINGS_OFFSET 24U


int em_eeprom_get_device(void);
size_t em_eeprom_get_size(void);
int em_eeprom_read(const off_t a_data_offset, void *a_data, const size_t a_data_length);
int em_eeprom_write(const off_t a_data_offset, const void *a_data, const size_t a_data_length);
