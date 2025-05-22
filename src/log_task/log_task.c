#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "log_task.h"


void vLogTask(void *pvParameters) {
    (void) pvParameters;

    while (1) {
        // Gera valores simulados
        int temp = 40 + rand() % 10;  // Temperatura entre 40 e 49 ºC
        int tensao = 3900 + rand() % 100;  // Tensão entre 3900 e 3999 mV

        // Formata o log no formato compatível com o frontend
        char log_msg[128];
        snprintf(log_msg, sizeof(log_msg), "[%ld] [BMS_PROC] Temp: %d ºC | Tensão: %d mV",
                 xTaskGetTickCount(), temp, tensao);

        // Imprime no stdout para ser capturado pelo Flask-SocketIO
        printf("%s\n", log_msg);
        fflush(stdout);  // Garante que a saída seja enviada imediatamente

        vTaskDelay(pdMS_TO_TICKS(2000));  // Delay de 2 segundos
    }
}