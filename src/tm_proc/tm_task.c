#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include <string.h>
#include "rf_tm_task.h"

// Headers dos módulos de housekeeping
#include "hk_cpu_task.h"
#include "hk_gps_task.h"
#include "hk_pwr_task.h"
#include "hk_star_task.h"

// Fila de solicitação de TM (recebe comandos de MAIN_SO)
extern QueueHandle_t xQueueTM_Request;
// Fila onde a TM será enviada
extern QueueHandle_t xQueueTM;

// Estrutura da telemetria
typedef struct {
    HKCPUData_t cpuData;
    HKGPSData_t gpsData;
    HKPWRData_t pwrData;
    HKSTARData_t starData;
} TelemetryPacket_t;

void vTMTask(void *pvParameters)
{
    (void)pvParameters;
    printf("[TM_PROC] iniciado.\n");
    fflush(stdout);

    TelemetryPacket_t telemetry;
    char requestSignal[16];

    while (1)
    {
        // Espera sinal para coletar TM
        if (xQueueReceive(xQueueTM_Request, &requestSignal, portMAX_DELAY) == pdTRUE)
        {
            // Simula coleta dos dados
            telemetry.cpuData.cpu_usage = 55;
            telemetry.cpuData.ram_free_kb = 64;
            telemetry.gpsData.lat = -23.5505f;
            telemetry.gpsData.lon = -46.6333f;
            telemetry.pwrData.voltage = 3.95f;
            telemetry.pwrData.current = 1.10f;
            telemetry.starData.roll = 3;
            telemetry.starData.pitch = 7;
            telemetry.starData.yaw = -1;

            // Envia o pacote de TM
            if (xQueueSend(xQueueTM, &telemetry, 0) == pdPASS)
            {
                printf("[TM_PROC] TM enviada após solicitação.\n");
            }
            else
            {
                printf("[TM_PROC] Fila de TM cheia.\n");
            }

            fflush(stdout);
        }
    }
}