#ifndef HK_GPS_TASK_H
#define HK_GPS_TASK_H

#include "FreeRTOS.h"
#include "task.h"

// Estrutura para dados de GPS
typedef struct {
    float lat;   // Latitude
    float lon;   // Longitude
} HKGPSData_t;

// Protótipo da task
void vHKGpsTask(void *pvParameters);

#endif // HK_GPS_TASK_H