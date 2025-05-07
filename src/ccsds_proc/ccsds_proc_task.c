#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include "shared.h"

extern QueueHandle_t xQueueTM_CCSDS;

void vCCSDSTask(void *pvParameters)
{
    (void) pvParameters;
    TelemetryPacket_t tm;

    printf("[CCSDS_PROC] Tarefa iniciada\n");

    while (1)
    {
        if (xQueueReceive(xQueueTM_CCSDS, &tm, pdMS_TO_TICKS(1000)) == pdTRUE)
        {
            printf("[CCSDS_PROC] Empacotando dados no formato CCSDS...\n");
            printf("Dados: CPU=%d%%, RAM=%dKB, GPS=(%.4f, %.4f), V=%.2fV I=%.2fA\n",
                   tm.cpuData.cpu_usage,
                   tm.cpuData.ram_free_kb,
                   tm.gpsData.lat,
                   tm.gpsData.lon,
                   tm.pwrData.voltage,
                   tm.pwrData.current);
        }
        else
        {
            printf("[CCSDS_PROC] Aguardando TM...\n");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}