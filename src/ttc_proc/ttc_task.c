#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "ttc_task.h"
#include "shared.h"
#include <stdio.h>
#include <string.h>

void vTTCTask(void *pvParameters)
{
    (void)pvParameters;
    printf("[TTC_PROC] iniciado. Aguardando comando de solo...\n");

    Command_t comando_simulado;
    CommandResponse_t resposta_recebida;

    for (;;)
    {
        // --- Comando 1: LIGAR_CAMERA ---
        strcpy(comando_simulado.comando, "LIGAR_CAMERA");
        xQueueSend(xQueueTTC_to_MAIN, &comando_simulado, portMAX_DELAY);
        printf("[TTC_PROC] Enviado comando: %s\n", comando_simulado.comando);

        memset(&resposta_recebida, 0, sizeof(resposta_recebida));  // ← limpa memória
        if (xQueueReceive(xQueueMAIN_to_TTC, &resposta_recebida, pdMS_TO_TICKS(3000)) == pdTRUE)
        {
            printf("[TTC_PROC] Resposta: %s => %s\n", 
                   resposta_recebida.comando,
                   resposta_recebida.status);
        }
        else
        {
            printf("[TTC_PROC] ⚠️ Timeout esperando resposta do MAIN_SO.\n");
        }

        vTaskDelay(pdMS_TO_TICKS(10000));

        // --- Comando 2: REPORTAR_TELEMETRIA ---
        strcpy(comando_simulado.comando, "REPORTAR_TELEMETRIA");
        xQueueSend(xQueueTTC_to_MAIN, &comando_simulado, portMAX_DELAY);
        printf("[TTC_PROC] Enviado comando: %s\n", comando_simulado.comando);

        memset(&resposta_recebida, 0, sizeof(resposta_recebida));
        if (xQueueReceive(xQueueMAIN_to_TTC, &resposta_recebida, pdMS_TO_TICKS(3000)) == pdTRUE)
        {
            printf("[TTC_PROC] Resposta: %s => %s\n", 
                   resposta_recebida.comando,
                   resposta_recebida.status);
        }
        else
        {
            printf("[TTC_PROC] ⚠️ Timeout esperando resposta do MAIN_SO.\n");
        }

        vTaskDelay(pdMS_TO_TICKS(10000));

        // --- Comando 3: ATIVAR_MOTORES_3000 ---
        strcpy(comando_simulado.comando, "ATIVAR_MOTORES_3000");
        xQueueSend(xQueueTTC_to_MAIN, &comando_simulado, portMAX_DELAY);
        printf("[TTC_PROC] Enviado comando: %s\n", comando_simulado.comando);

        memset(&resposta_recebida, 0, sizeof(resposta_recebida));
        if (xQueueReceive(xQueueMAIN_to_TTC, &resposta_recebida, pdMS_TO_TICKS(3000)) == pdTRUE)
        {
            printf("[TTC_PROC] Resposta: %s => %s\n", 
                   resposta_recebida.comando,
                   resposta_recebida.status);
        }
        else
        {
            printf("[TTC_PROC] ⚠️ Timeout esperando resposta do MAIN_SO.\n");
        }

        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}
