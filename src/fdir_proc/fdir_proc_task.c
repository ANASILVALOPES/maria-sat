#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "fdir_task.h"
#include "health_task.h"
#include "alert.h"
#include "shared.h"
#include <stdio.h>


void vFDIRProcTask(void *pvParameters)
{
    (void)pvParameters;
    printf("[FDIR_PROC] iniciado.\n");


    HealthData_t healthData;

    while (1)
    {
        if (xQueueHealth != NULL && xQueueReceive(xQueueHealth, &healthData, portMAX_DELAY) == pdTRUE) 
        {
            // Verificação de uso da CPU
            if (healthData.cpu_usage > 90) {
                printf("[FDIR_PROC] 🚨 CPU alta detectada (%d%%)\n", healthData.cpu_usage);
                send_alert(ALERT_CPU_HIGH, (float)healthData.cpu_usage);
            }

            // Verificação de baixa tensão com valor real
            if (healthData.power_level < 3.3f) {
                printf("[FDIR_PROC] 🚨 Baixa tensão detectada (%.2fV)\n", healthData.power_level);
                send_alert(ALERT_LOW_VOLTAGE, healthData.power_level);  // CORRIGIDO: envio do valor real
            }

            // Verificação de falha no GPS
            if (!healthData.gps_status) {
                printf("[FDIR_PROC] 🚨 Falha no GPS detectada!\n");
                send_alert(ALERT_GPS_FAIL, 0.0f);  // mantém assinatura
            }

            // Verificação de falha no Star Tracker
            if (!healthData.star_status) {
                printf("[FDIR_PROC] 🚨 Falha no Star Tracker detectada!\n");
                send_alert(ALERT_STAR_FAIL, 0.0f);  // mantém assinatura
            }
        }

        vTaskDelay(pdMS_TO_TICKS(500)); // Delay entre verificações
    }
}
