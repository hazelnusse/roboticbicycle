#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

// Map the FreeRTOS handlers to the names of the Handlers in the CMSIS
// vector table
#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

#define configUSE_PREEMPTION                1
#define configUSE_IDLE_HOOK                 0
#define configUSE_TICK_HOOK                 0
#define configCPU_CLOCK_HZ                  72000000
#define configTICK_RATE_HZ                  500
#define configMAX_PRIORITIES                5
#define configMINIMAL_STACK_SIZE            128
#define configTOTAL_HEAP_SIZE               10240
#define configMAX_TASK_NAME_LEN             16
#define configUSE_TRACE_FACILITY            0
#define configUSE_16_BIT_TICKS              0
#define configIDLE_SHOULD_YIELD             1
#define configUSE_MUTEXES                   1
#define configUSE_RECURSIVE_MUTEXES         1
#define configUSE_COUNTING_SEMAPHORES       1
#define configUSE_ALTERNATIVE_API           0
#define configCHECK_FOR_STACK_OVERFLOW      0
#define configQUEUE_REGISTRY_SIZE           10
#define configGENERATE_RUN_TIME_STATS       0

#define configUSE_CO_ROUTINES               0
#define configMAX_CO_ROUTINE_PRIORITIES     1

#define configUSE_TIMERS                    1
#define configTIMER_TASK_PRIORITY           3
#define configTIMER_QUEUE_LENGTH            10
#define configTIMER_TASK_STACK_DEPTH        configMINIMAL_STACK_SIZE

#define configKERNEL_INTERRUPT_PRIORITY      255
#define configMAX_SYSCALL_INTERRUPT_PRIORITY  191

// This is the value being used as per the ST library which permits 16
// priority values, 0 to 15.  This must correspond to the
// configKERNEL_INTERRUPT_PRIORITY setting.  Here 15 corresponds to the lowest
// NVIC value of 255.
#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY	15

//#define configASSERT((x))               if((x) == 0) vCallAssert( __FILE__, __LINE__ )

#define INCLUDE_vTaskPrioritySet            1
#define INCLUDE_uxTaskPriorityGet           1
#define INCLUDE_vTaskDelete                 1
#define INCLUDE_vTaskCleanUpResources       0
#define INCLUDE_vTaskSuspend                1
#define INCLUDE_vResumeFromISR              1
#define INCLUDE_vTaskDelayUntil             1
#define INCLUDE_vTaskDelay                  1
#define INCLUDE_xTaskGetSchedulerState      1
#define INCLUDE_xTaskGetCurrentTaskHandle   1
#define INCLUDE_uxTaskGetStackHighWaterMark 0

#endif /* FREERTOS_CONFIG_H */
