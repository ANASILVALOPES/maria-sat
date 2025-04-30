#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "shared.h"  // Define Command_t, CommandResponse_t etc.

// Filas externas
extern QueueHandle_t xQueueTTC_to_MAIN;
extern QueueHandle_t xQueueMAIN_to_TTC;
extern QueueHandle_t xQueueTM_Request;  // <- Fila para sinalizar coleta de TM

// Protótipos
void ligar_camera(void);
void desligar_camera(void);
void reportar_telemetria(void);
void ativar_motores(int tempo_ms);

void vMainTask(void *pvParameters)
{
    (void)pvParameters;
    printf("[MAIN_SO] iniciado.\n");

    Command_t recebido;
    CommandResponse_t resposta;

    while (1)
    {
        if (xQueueReceive(xQueueTTC_to_MAIN, &recebido, pdMS_TO_TICKS(1000)) == pdTRUE)
        {
            printf("[MAIN_SO] Comando recebido: %s\n", recebido.comando);

            memset(&resposta, 0, sizeof(resposta));
            strncpy(resposta.comando, recebido.comando, sizeof(resposta.comando) - 1);

            if (strcmp(recebido.comando, "LIGAR_CAMERA") == 0)
            {
                ligar_camera();
                strcpy(resposta.status, "Implementado");
            }
            else if (strcmp(recebido.comando, "DESLIGAR_CAMERA") == 0)
            {
                desligar_camera();
                strcpy(resposta.status, "Implementado");
            }
            else if (strcmp(recebido.comando, "REPORTAR_TELEMETRIA") == 0)
            {
                reportar_telemetria();  // Envia sinal para vTMTask
                strcpy(resposta.status, "Implementado");
            }
            else if (strncmp(recebido.comando, "ATIVAR_MOTORES_", 15) == 0)
            {
                int tempo = atoi(recebido.comando + 15);
                ativar_motores(tempo);
                strcpy(resposta.status, "Implementado");
            }
            else
            {
                printf("[MAIN_SO] Comando desconhecido: %s\n", recebido.comando);
                strcpy(resposta.status, "Não reconhecido");
            }

            if (xQueueSend(xQueueMAIN_to_TTC, &resposta, pdMS_TO_TICKS(100)) != pdPASS)
            {
                printf("[MAIN_SO] Falha ao enviar resposta para TTC.\n");
            }
        }
        else
        {
            printf("[MAIN_SO] MARIA-SAT está operando normalmente.\n");
        }

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

// Implementações
void ligar_camera(void)        { printf("[MAIN_SO] 📷 Camera ligada.\n"); }
void desligar_camera(void)     { printf("[MAIN_SO] 📷 Camera desligada.\n"); }
void reportar_telemetria(void) {
    printf("[MAIN_SO] 📡 Solicitando telemetria ao TM_PROC...\n");

    const char signal[] = "REQUEST";  // ou qualquer string
    if (xQueueSend(xQueueTM_Request, &signal, pdMS_TO_TICKS(100)) != pdPASS)
    {
        printf("[MAIN_SO] Erro ao solicitar telemetria.\n");
    }
}

void ativar_motores(int t)     { printf("[MAIN_SO] 🚀 Motores ativados por %d ms.\n", t); }