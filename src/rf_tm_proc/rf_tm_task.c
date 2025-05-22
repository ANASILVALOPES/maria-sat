#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include "shared.h"
#include "ccsds_task.h"

extern QueueHandle_t xQueueTM_RF;

void vRFTMTask(void *pvParameters)
{
    (void) pvParameters;
    uint8_t encoded_packet[sizeof(CCSDSPacket_t)];
    TelemetryPacket_t tmData;

    puts("[RF_TM_PROC] iniciado.");

    while (1)
    {
        if (xQueueReceive(xQueueTM_RF, &encoded_packet, pdMS_TO_TICKS(1000)) == pdTRUE)
        {
            if (ccsds_decode(encoded_packet, &tmData) == 0)
            {
                printf("[RF_TM_PROC] Dados decodificados corretamente.\n");
                printf("CPU=%d%% | RAM=%dKB\n"
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
                       tmData.starData.yaw);
            }
            else
            {
                printf("[RF_TM_PROC] Erro ao decodificar dados CCSDS.\n");
            }
        }
        else
        {
            puts("[RF_TM_PROC] Nenhum dado TM recebido.");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
