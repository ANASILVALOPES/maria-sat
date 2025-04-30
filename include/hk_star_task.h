#ifndef HK_STAR_TASK_H
#define HK_STAR_TASK_H

#include "FreeRTOS.h"
#include "task.h"

// Estrutura para dados do Star Tracker
typedef struct {
    int roll;   // Roll em graus
    int pitch;  // Pitch em graus
    int yaw;    // Yaw em graus
} HKSTARData_t;

// Protótipo da task
void vHKStarTask(void *pvParameters);

#endif // HK_STAR_TASK_H