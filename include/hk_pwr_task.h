#ifndef HK_PWR_TASK_H
#define HK_PWR_TASK_H

#include "FreeRTOS.h"
#include "task.h"

// Estrutura para dados de Power (tensão/corrente)
typedef struct {
    float voltage;  // Voltagem
    float current;  // Corrente
} HKPWRData_t;

// Protótipo da task
void vHKPWRTask(void *pvParameters);

#endif // HK_PWR_TASK_H