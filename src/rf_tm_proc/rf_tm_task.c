#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include "shared.h"
#include "tm_task.h"

extern QueueHandle_t xQueueTM_RF;  

void vRFTMTask(void *pvParameters)
{
    (void) pvParameters;
    TelemetryPacket_t tmData;

    puts("[RF_TM_PROC] iniciado.");

    while (1)
    {
        if (xQueueReceive(xQueueTM_RF, &tmData, pdMS_TO_TICKS(1000)) == pdTRUE)
        {
            char buffer[256];

            snprintf(buffer, sizeof(buffer),
                "[RF_TM_PROC] Enviando TM:\n"
                "CPU=%d%% | RAM=%dKB\n"
                "GPS=(%d.%04d, %d.%04d)\n"
                "PWR=%d.%02dV %d.%02dA\n"
                "STAR=(Roll=%d° Pitch=%d° Yaw=%d°)\n",
                tmData.cpuData.cpu_usage,
                tmData.cpuData.ram_free_kb,
                (int)tmData.gpsData.lat,
                abs((int)(tmData.gpsData.lat * 10000) % 10000),
                (int)tmData.gpsData.lon,
                abs((int)(tmData.gpsData.lon * 10000) % 10000),
                (int)tmData.pwrData.voltage,
                abs((int)(tmData.pwrData.voltage * 100) % 100),
                (int)tmData.pwrData.current,
                abs((int)(tmData.pwrData.current * 100) % 100),
                tmData.starData.roll,
                tmData.starData.pitch,
                tmData.starData.yaw
            );

            puts(buffer);
        }
        else
        {
            puts("[RF_TM_PROC] Nenhum dado TM recebido.");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}