#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * FreeRTOS Kernel Configuration for maria-sat
 *----------------------------------------------------------*/

/* Configurações gerais */
#define configUSE_PREEMPTION            1      // Habilita a preempção de tarefas
#define configUSE_IDLE_HOOK            0      // Não usa o hook de idle
#define configUSE_TICK_HOOK            0      // Não usa o hook de tick
#define configUSE_16_BIT_TICKS         0      // Usa ticks de 32 bits (0 significa 32-bit)
#define configCPU_CLOCK_HZ             ( ( unsigned long ) 16000000 )  // Ajuste conforme o seu clock (16MHz é um exemplo)
#define configTICK_RATE_HZ             ( ( TickType_t ) 1000 )         // Taxa de tick 1ms
#define configMAX_PRIORITIES           ( 5 )    // Número máximo de prioridades (ajuste conforme necessário)
#define configMINIMAL_STACK_SIZE       ( ( unsigned short ) 128 )      // Tamanho mínimo da pilha de tarefas
#define configTOTAL_HEAP_SIZE          ( ( size_t ) ( 4 * 1024 ) )    // Tamanho do heap (4KB, ajuste conforme necessário)
#define configMAX_TASK_NAME_LEN        ( 16 )   // Comprimento máximo dos nomes das tarefas
#define configUSE_TRACE_FACILITY       0      // Não usa trace para depuração

/*-----------------------------------------------------------
 * Configurações específicas para o sistema de tempo real
 *----------------------------------------------------------*/
#define configUSE_TIMERS               1      // Habilita timers
#define configTIMER_TASK_PRIORITY      ( 3 )   // Prioridade da tarefa do timer
#define configTIMER_QUEUE_LENGTH       10     // Tamanho da fila de timers
#define configTIMER_TASK_STACK_DEPTH   ( configMINIMAL_STACK_SIZE )  // Tamanho da pilha para a tarefa de timers

/*-----------------------------------------------------------
 * Configurações para comunicação entre tarefas
 *----------------------------------------------------------*/
#define configUSE_MUTEXES              1      // Habilita semáforos binários
#define configUSE_COUNTING_SEMAPHORES  1      // Habilita semáforos de contagem
#define configUSE_QUEUE_SETS           1      // Habilita conjunto de filas

/*-----------------------------------------------------------
 * Funções de stack e memória
 *----------------------------------------------------------*/
#define configSTACK_DEPTH_TYPE         uint16_t  // Tamanho da pilha da tarefa
#define configTOTAL_HEAP_SIZE          ( 4 * 1024 )  // Tamanho total do heap (4KB)

#define portBYTE_ALIGNMENT             4           // Alinhamento de byte de 4 bytes para arquitetura 32-bit

/*-----------------------------------------------------------
 * Definições de tipo de dados do FreeRTOS
 *----------------------------------------------------------*/
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1    // Habilita a seleção otimizada de tarefas

/* Definições adicionais para plataformas específicas */
#define configENABLE_BACKWARD_COMPATIBILITY 0 // Desabilita compatibilidade com versões antigas

/*-----------------------------------------------------------
 * Definindo os tipos para arquitetura 32 bits
 *----------------------------------------------------------*/
typedef int BaseType_t;       // Tipo base para valores inteiros
typedef unsigned int UBaseType_t;  // Tipo base sem sinal para valores inteiros
typedef unsigned long TickType_t;  // Tipo para contar os ticks de tempo

#endif /* FREERTOS_CONFIG_H */
