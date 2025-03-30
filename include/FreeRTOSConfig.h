#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * FreeRTOS Kernel Configuration for MARIA-SAT (POSIX)
 *----------------------------------------------------------*/

/* Configurações gerais */
#define configUSE_PREEMPTION            1
#define configUSE_IDLE_HOOK             0
#define configUSE_TICK_HOOK             0
#define configUSE_16_BIT_TICKS          0
#define configCPU_CLOCK_HZ              ( ( unsigned long ) 16000000 )
#define configTICK_RATE_HZ              ( 1000 )  // Tick a cada 1ms
#define configMAX_PRIORITIES            ( 5 )
#define configMINIMAL_STACK_SIZE        ( ( unsigned short ) 128 )
#define configTOTAL_HEAP_SIZE           ( ( size_t ) ( 4 * 1024 ) )
#define configMAX_TASK_NAME_LEN         ( 16 )
#define configUSE_TRACE_FACILITY        0

/* Configurações para timers */
#define configUSE_TIMERS                1
#define configTIMER_TASK_PRIORITY       ( 3 )
#define configTIMER_QUEUE_LENGTH        10
#define configTIMER_TASK_STACK_DEPTH    ( configMINIMAL_STACK_SIZE )

/* Comunicação entre tarefas */
#define configUSE_MUTEXES               1
#define configUSE_COUNTING_SEMAPHORES   1
#define configUSE_QUEUE_SETS            1

/* Stack e memória */
#define configSTACK_DEPTH_TYPE          uint16_t
#define portBYTE_ALIGNMENT              4

/* Otimizações e compatibilidade */
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1
#define configENABLE_BACKWARD_COMPATIBILITY     0

/* Definir portMAX_DELAY para evitar erro de símbolo não encontrado */
#ifndef portMAX_DELAY
    #define portMAX_DELAY ( ( TickType_t ) 0xffffffffUL )
#endif

#endif /* FREERTOS_CONFIG_H */
