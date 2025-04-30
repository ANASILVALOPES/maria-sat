#ifndef HK_CPU_TASK_H
#define HK_CPU_TASK_H

#include "FreeRTOS.h"
#include "task.h"

// Estrutura para dados de CPU
typedef struct {
    int cpu_usage;      // Percentual de uso da CPU (%)
    int ram_free_kb;    // Memória RAM livre em KB
} HKCPUData_t;

// Protótipo da task
void vHKCPUTask(void *pvParameters);

#endif // HK_CPU_TASK_H