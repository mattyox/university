#include "em_eeprom.h"

#include <zephyr/logging/log.h>


#define EEPROM_LOG_PROJECT_NAME Energy_Meter
#define EEPROM_LOG_LEVEL LOG_LEVEL_DBG

#define EEPROM_NODE DT_ALIAS(eeprom)


static const struct device *s_eeprom;


int em_eeprom_get_device(void)
{
    LOG_MODULE_DECLARE(EEPROM_LOG_PROJECT_NAME, EEPROM_LOG_LEVEL);

	s_eeprom = DEVICE_DT_GET(EEPROM_NODE);

	if (!device_is_ready(s_eeprom))
	{
		LOG_ERR("Device \"%s\" is not ready. Check the driver initialization logs for errors!", s_eeprom->name);

		s_eeprom = NULL;

		return -ENODEV;
	}

	LOG_INF("Found EEPROM device \"%s\".", s_eeprom->name);

	return 0;
}

size_t em_eeprom_get_size(void)
{
	LOG_MODULE_DECLARE(EEPROM_LOG_PROJECT_NAME, EEPROM_LOG_LEVEL);

	const size_t eeprom_size = eeprom_get_size(s_eeprom);
	LOG_INF("Using EEPROM %s with size of: %zu [B].", s_eeprom->name, eeprom_size);

	return eeprom_size;
}

int em_eeprom_read(const off_t a_data_offset, void *a_data, const size_t a_data_length)
{
	LOG_MODULE_DECLARE(EEPROM_LOG_PROJECT_NAME, EEPROM_LOG_LEVEL);

	int ret = 0;
	
	ret = eeprom_read(s_eeprom, a_data_offset, a_data, a_data_length);
	if (ret != 0)
		LOG_ERR("Could not read from EEPROM %s: %i!", s_eeprom->name, ret);
	else
		LOG_DBG("Read %i bytes at %lX from EEPROM %s.", a_data_length, a_data_offset, s_eeprom->name);

	return ret;
}

int em_eeprom_write(const off_t a_data_offset, const void *a_data, const size_t a_data_length)
{
	LOG_MODULE_DECLARE(EEPROM_LOG_PROJECT_NAME, EEPROM_LOG_LEVEL);

	int ret = 0;

	ret = eeprom_write(s_eeprom, a_data_offset, a_data, a_data_length);
	if (ret != 0)
		LOG_ERR("Could not write to EEPROM %s: %i!", s_eeprom->name, ret);
	else
		LOG_DBG("Wrote %i bytes at %lX to EEPROM %s.", a_data_length, a_data_offset, s_eeprom->name);

	return ret;
}
