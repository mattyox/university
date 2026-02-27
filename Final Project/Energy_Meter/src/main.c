#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "energy_meter.h"


LOG_MODULE_REGISTER(Energy_Meter, LOG_LEVEL_DBG);


int main(void)
{
    int ret = 0;

    ret = get_components();
    if (ret != 0)
	{
		LOG_ERR("Could not start Energy Meter: %i!", ret);
		return ret;
	}
	else
	{
		LOG_INF("Started Energy Meter.");
	}

    ret = check_device_data();
    if (ret != 0)
	{
		LOG_ERR("Could not check Energy Meter data: %i!", ret);
		return ret;
	}
	else
	{
		LOG_INF("Checked Energy Meter data.");
	}

    while (1)
    {
        if (is_readings_update_required())
        {
            LOG_INF("Updating Energy Meter readings.");

            ret = update_readings();
            if (ret != 0)
                LOG_WRN("Could not update Energy Meter readings: %i!", ret);
            else
                LOG_INF("Updated Energy Meter readings.");
        }
    }


    return ret;
}
