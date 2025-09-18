#ifndef TEST_TRACKER_HARDWARE_CONFIG_H
#define TEST_TRACKER_HARDWARE_CONFIG_H

#include "FreeRTOS.h"

#define TEST_TRACKER_DATA PRIVILEGED_DATA

__attribute__((weak)) void vApplicationSafeAssertHandler( void );

__attribute__((weak)) void vApplicationSVCHandler( void );

#endif /* TEST_TRACKER_HARDWARE_CONFIG_H */
