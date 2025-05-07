#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include <string.h>
#include "shared.h"

// Supondo que esta fila envia comandos simulados ao MAIN_SO
extern QueueHandle_t xQueueTTC_to_MAIN;

void vRFTCTask(void *pvParameters)
{
    (void)pvParameters;
    printf("[RF_TC_PROC] iniciado.\n");
    printf("[RF_TC_PROC] Aguardando comando de solo...\n");

    Command_t comando_simulado;

    while (1)
    {
        // Simula o recebimento de um comando do solo
        strcpy(comando_simulado.comando, "STATUS");  // exemplo de comando novo
        xQueueSend(xQueueTTC_to_MAIN, &comando_simulado, portMAX_DELAY);
        printf("[RF_TC_PROC] Comando recebido via UART: %s\n", comando_simulado.comando);

        vTaskDelay(pdMS_TO_TICKS(15000));  // a cada 15 segundos simula novo comando
    }
}