#include <string.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "ttc_task.h"
#include "shared.h"
#include "main.h"

void vTTCTask(void *pvParameters) {
    (void)pvParameters;
    CommandResponse_t resposta_recebida;

    printf("[TTC_PROC] Tarefa iniciada.\n");
    printf("[TTC_PROC] Aguardando respostas do MAIN_SO...\n");
    fflush(stdout);

    while (1) {
        // Aguarda respostas do MAIN_SO
        memset(&resposta_recebida, 0, sizeof(resposta_recebida));
        if (xQueueReceive(xQueueMAIN_to_TTC, &resposta_recebida, portMAX_DELAY) == pdTRUE) {
            // Garante que as strings estejam terminadas corretamente
            resposta_recebida.comando[sizeof(resposta_recebida.comando) - 1] = '\0';
            resposta_recebida.status[sizeof(resposta_recebida.status) - 1] = '\0';
            printf("[TTC_PROC] Resposta: %s => %s\n", 
                   resposta_recebida.comando,
                   resposta_recebida.status);
        }

        vTaskDelay(pdMS_TO_TICKS(100)); // Pequeno atraso para evitar consumo excessivo de CPU
    }
}