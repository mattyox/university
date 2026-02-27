#pragma once

#include <time.h>
#include <stdbool.h>


int em_rtc_get_device(void);
int em_rtc_get_time(int64_t *a_unix_epoch);
int em_rtc_set_time(struct tm *a_struct_time);
void em_rtc_log_time(int64_t a_unix_epoch);
bool em_rtc_get_interrupt_status(void);
