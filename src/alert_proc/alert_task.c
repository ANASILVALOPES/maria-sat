#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include "shared.h"

void vALERTProcTask(void *pvParameters)
{
    (void)pvParameters;

    printf("[ALERT_PROC] iniciado.\n");

    BMSData_t leitura;

    while (1)
    {
        // Espera por nova leitura da fila
        if (xQueueReceive(xBMSQueue, &leitura, portMAX_DELAY) == pdTRUE)
        {
            // Verifica condição imediata
            if (leitura.temperatura >= ALERTA_TEMPERATURA_CRITICA)
            {
                printf("[ALERT] Temperatura nominal: %d ºC\n", leitura.temperatura);
            }

            // Verifica anomalia no histórico (3 leituras consecutivas >= 45)
            BMSData_t historico[BMS_HISTORY_SIZE];
            int count = 0;
            getBMSHistory(historico, &count);

            int consecutivos = 0;
            for (int i = 0; i < count; i++)
            {
                if (historico[i].temperatura >= ALERTA_TEMPERATURA_CRITICA)
                {
                    consecutivos++;
                    if (consecutivos >= 3)
                    {
                        printf("[ALERT] 🚨 Alerta crítico: 3+ leituras consecutivas >= %d ºC\n", ALERTA_TEMPERATURA_CRITICA);
                        break;
                    }
                }
                else
                {
                    consecutivos = 0;
                }
            }
        }
    }
}
