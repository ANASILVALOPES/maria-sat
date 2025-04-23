#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include "shared.h"

void vBMSProcTask(void *pvParameters)
{
    (void)pvParameters;

    printf("[BMS_PROC] iniciado.\n");

    while (1)
    {
        BMSData_t leitura;

        // Simulação: valores semi-aleatórios (pode trocar por sensor real depois)
        leitura.temperatura = 40 + (rand() % 10); // 40–49 °C
        leitura.tensao = 3900 + (rand() % 100);   // 3900–3999 mV

        // Mostra leitura atual
        printf("[BMS_PROC] Temp: %d ºC | Tensão: %d mV\n", leitura.temperatura, leitura.tensao);

        // Envia leitura via fila para o ALERT_PROC
        xQueueSend(xBMSQueue, &leitura, portMAX_DELAY);

        // Salva no histórico com proteção por mutex
        addBMSHistory(leitura);

        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}
