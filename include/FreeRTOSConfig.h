#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * FreeRTOS Kernel Configuration for MARIA-SAT (ARM/QEMU)
 *----------------------------------------------------------*/

/* Core settings */
#define configUSE_PREEMPTION            1
#define configUSE_IDLE_HOOK             0
#define configUSE_TICK_HOOK             0
#define configCPU_CLOCK_HZ              ( ( unsigned long ) 50000000 ) // 50 MHz
#define configTICK_RATE_HZ              ( ( TickType_t ) 1000 )        // 1ms tick
#define configMAX_PRIORITIES            ( 5 )
#define configMINIMAL_STACK_SIZE        ( ( unsigned short ) 128 )
#define configTOTAL_HEAP_SIZE           ( ( size_t ) ( 50 * 1024 ) )   // 50 KB
#define configMAX_TASK_NAME_LEN         ( 16 )
#define configUSE_16_BIT_TICKS          0
#define configIDLE_SHOULD_YIELD         1

/* Optional features */
#define configUSE_MUTEXES               1
#define configUSE_MALLOC_FAILED_HOOK    1
#define configUSE_RECURSIVE_MUTEXES     0
#define configUSE_COUNTING_SEMAPHORES   1
#define configUSE_QUEUE_SETS            0
#define configCHECK_FOR_STACK_OVERFLOW  2
#define configUSE_MALLOC_FAILED_HOOK    1
#define configUSE_TRACE_FACILITY        1
#define configUSE_STATS_FORMATTING_FUNCTIONS 1  // Permite vTaskList()

/* Timer task config */
#define configUSE_TIMERS                1
#define configTIMER_TASK_PRIORITY       2
#define configTIMER_QUEUE_LENGTH        10
#define configTIMER_TASK_STACK_DEPTH    ( configMINIMAL_STACK_SIZE )

/* Cortex-M specifics (important!) */
#define configKERNEL_INTERRUPT_PRIORITY         255
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    191
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY 15
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5

/* Map handlers to CMSIS (important for QEMU Cortex-M3) */
#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

/* Memory alignment */
#define portBYTE_ALIGNMENT          8
#define configSTACK_DEPTH_TYPE      uint16_t

/* Function includes */
#define INCLUDE_vTaskDelay                  1
#define INCLUDE_uxTaskGetStackHighWaterMark 1
#define INCLUDE_xTaskGetHandle              1
#define INCLUDE_xTaskGetCurrentTaskHandle   1
#define INCLUDE_xTaskGetSchedulerState      1
#define INCLUDE_uxTaskPriorityGet           1
#define INCLUDE_vTaskPrioritySet            1
#define INCLUDE_vTaskDelete                 1
#define INCLUDE_vTaskSuspend                1
#define INCLUDE_eTaskGetState               1

/* Correção importante para builds */
#define portDISABLEINTERRUPTS portDISABLE_INTERRUPTS

#endif /* FREERTOS_CONFIG_H */
