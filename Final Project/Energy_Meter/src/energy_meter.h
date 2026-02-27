#pragma once


#include "ade7753.h"


struct readings {
    double active_energy;
    double apparent_energy;
    double reactive_energy;
};


int get_components(void);
int check_device_data(void);

bool is_readings_update_required(void);
int update_readings(void);

float get_active_energy(void);
float get_apparent_energy(void);
float get_reactive_energy(void);
float get_node_0_current(void);
float get_node_0_voltage(void);
float get_node_1_current(void);
float get_node_1_voltage(void);
float get_node_2_current(void);
float get_node_2_voltage(void);
int64_t get_boot_time(void);
int64_t get_reset_time(void);
void reset_readings(void);
