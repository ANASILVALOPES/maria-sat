#ifndef TM_TASK_H
#define TM_TASK_H

#include "hk_cpu_task.h"
#include "hk_gps_task.h"
#include "hk_pwr_task.h"
#include "hk_star_task.h"

// Definindo a estrutura aqui:
typedef struct {
    HKCPUData_t cpuData;
    HKGPSData_t gpsData;
    HKPWRData_t pwrData;
    HKSTARData_t starData;
} TelemetryPacket_t;

void vTMTask(void *pvParameters);

#endif // TM_TASK_H