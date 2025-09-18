/*
 * Copyright (C) 2024 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */
#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
* Application specific definitions.
*
* These definitions should be adjusted for your particular hardware and
* application requirements.
*
* THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
* FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.  See
* http://www.freertos.org/a00110.html
*----------------------------------------------------------*/

/* [Required] Enable safety features. */
#define configUSE_SAFETY                             1

/* [Required] Allocation related configurations. */
#define configSUPPORT_STATIC_ALLOCATION              1
#define configSUPPORT_DYNAMIC_ALLOCATION             0
#define configUSE_STATIC_BUFFERS                     0

/* [Required] Kernel fundamental configurations. */
#define configNUMBER_OF_CORES                        1
#define configUSE_PREEMPTION                         1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION      1
#define configCHECK_FOR_STACK_OVERFLOW               0
#define configIDLE_SHOULD_YIELD                      0
#define configUSE_16_BIT_TICKS                       0
#define configUSE_TIME_SLICING                       0
#define configRUN_MULTIPLE_PRIORITIES                0

/* [Required] Port fundamental configurations. */
#define configCPU_CLOCK_HZ                           ( 110000000U )
#define portCRITICAL_NESTING_IN_TCB                  0
#define configENABLE_FPU                             1U
#define configENABLE_MPU                             0

/* [Required] Kernel object availability configs. */
#define configUSE_COUNTING_SEMAPHORES                1
#define configUSE_MUTEXES                            1
#define configUSE_CO_ROUTINES                        0
#define configUSE_DAEMON_TASK_STARTUP_HOOK           0
#define configUSE_EVENT_GROUPS                       0
#define configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES    0
#define configUSE_MINI_LIST_ITEM                     0
#define configUSE_QUEUE_SETS                         0
#define configUSE_RECURSIVE_MUTEXES                  0
#define configUSE_STREAM_BUFFERS                     0
#define configUSE_TASK_NOTIFICATIONS                 0
#define configUSE_TICKLESS_IDLE                      0
#define configUSE_TIMERS                             0
#define configUSE_TRACE_FACILITY                     0

/* [Required] API availability configs. */
#define INCLUDE_uxTaskPriorityGet                    1
#define INCLUDE_vTaskDelay                           1
#define INCLUDE_xTaskGetSchedulerState               1
#define INCLUDE_uxQueueMessagesWaiting               1
#define INCLUDE_uxQueueMessagesWaitingFromISR        1
#define INCLUDE_eTaskGetState                        0
#define INCLUDE_vTaskPrioritySet                     0
#define INCLUDE_vTaskDelete                          0
#define INCLUDE_vTaskCleanUpResources                0
#define INCLUDE_vTaskSuspend                         0
#define INCLUDE_vTaskDelayUntil                      0
#define INCLUDE_uxTaskGetStackHighWaterMark          0
#define INCLUDE_uxTaskGetStackHighWaterMark2         0
#define INCLUDE_xTimerGetTimerDaemonTaskHandle       0
#define INCLUDE_xTaskGetIdleTaskHandle               0
#define INCLUDE_xTaskGetHandle                       0
#define INCLUDE_xSemaphoreGetMutexHolder             0
#define INCLUDE_xTimerPendFunctionCall               0
#define INCLUDE_xTaskAbortDelay                      0
#define INCLUDE_xTaskResumeFromISR                   0
#define INCLUDE_vTaskEndScheduler                    0
#define INCLUDE_vTaskResetState                      0
#define INCLUDE_xTaskCatchUpTicks                    0
#define INCLUDE_vTaskPlaceOnUnorderedEventList       0
#define INCLUDE_uxTaskResetEventItemValue            0
#define INCLUDE_uxQueueGetQueueItemSize              0
#define INCLUDE_uxQueueGetQueueLength                0
#define INCLUDE_uxQueueSpacesAvailable               0
#define INCLUDE_vQueueDelete                         0
#define INCLUDE_xQueueGiveFromISR                    0
#define INCLUDE_xQueuePeek                           0
#define INCLUDE_xQueuePeekFromISR                    0
#define INCLUDE_xQueueIsQueueEmptyFromISR            0
#define INCLUDE_xQueueIsQueueFullFromISR             0

/* Config items used by our customers. */

/* Modifiable config values*/
#define configTICK_RATE_HZ                         ( 1000U )
#define configMINIMAL_STACK_SIZE                   ( ( unsigned short ) 0x400 )
#define configSYSTEM_CALL_STACK_SIZE               configMINIMAL_STACK_SIZE
#define configTOTAL_HEAP_SIZE                      ( ( size_t ) ( 80U * 512U ) )
#define configMAX_PRIORITIES                       ( 30UL )
#define configMAX_TASK_NAME_LEN                    ( 32U )
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS    5

/* Optional config items - can be enabled/disabled as you see fit. */

/* Testing config items. */
#define configUSE_IDLE_HOOK    1
#define configUSE_TICK_HOOK    1
/* Section of the file that can't be included in ASM Pre-processor */
#ifndef FREERTOS_ASSEMBLY
    extern void vMainSetupTimerInterrupt( void );
    #define configCLEAR_TICK_INTERRUPT()
    #define configSETUP_TICK_INTERRUPT()    vMainSetupTimerInterrupt()
#endif /* FREERTOS_ASSEMBLY */

/** Note: These value come from the Board Support Package. They are pulled directly
 * from sys_vim.h, and reg_vim.h. These values correspond to hardware registers
 * and keys exclusive to the board that this demo was written for.
 */

#include "freertos_kernel_trace_defs.h"

#ifndef configMAX_TASK_NAME_LEN
    #define configMAX_TASK_NAME_LEN    ( 32U )
#endif

#endif /* FREERTOS_CONFIG_H */
