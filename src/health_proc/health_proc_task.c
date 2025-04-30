#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "health_task.h"
#include "shared.h"
#include <stdio.h>
#include <stdlib.h>  // para rand()
#include "health_task.h"

QueueHandle_t xQueueHealth = NULL;  

void vHEALTHProcTask(void *pvParameters)
{
    (void)pvParameters;
    printf("[HEALTH_PROC] iniciado.\n");

    HealthData_t healthData;

    while (1)
    {
        // Simulando coleta dos dados dos subsistemas
        healthData.cpu_usage = getCPUUsage();
        healthData.power_level = getPowerLevel();
        healthData.gps_status = getGPSStatus();
        healthData.star_status = getStarStatus();

        // Enviando para a fila para análise do FDIR
        if (xQueueHealth != NULL) {
            xQueueSend(xQueueHealth, &healthData, 0);
        }

        // Exibindo no terminal para debug
        printf("[HEALTH_PROC] CPU: %d%% | Power: %.2fV | GPS: %s | STAR: %s\n",
            healthData.cpu_usage,
            healthData.power_level,
            healthData.gps_status ? "OK" : "FAIL",
            healthData.star_status ? "OK" : "FAIL");

        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay de 1 segundo
    }
}

// Funções de simulação
int getCPUUsage(void) {
    return rand() % 100;
}

float getPowerLevel(void) {
    return 3.0 + ((rand() % 120) / 100.0); // entre 3.0V e 4.2V
}

bool getGPSStatus(void) {
    return (rand() % 10) > 1; // 90% de chance de OK
}

bool getStarStatus(void) {
    return (rand() % 10) > 2; // 80% de chance de OK
}
