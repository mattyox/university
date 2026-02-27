#pragma once

#include <zephyr/net/openthread.h>


void em_ot_set_context(struct openthread_context *a_context);
void em_ot_set_instance(struct otInstance *a_instance);
otError em_ot_initialize(void);
int em_ot_get_sntp_time(int64_t *a_unix_epoch);
uint64_t em_ot_get_eui64(void);
char *em_ot_get_eui64_string(char eui64_string[]);
