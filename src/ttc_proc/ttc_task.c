#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"

void vTTCTask(void *pvParameters) {
    // Simulando comando vindo do solo
    char comando_recebido[16] = "STATUS";

    for (;;) {
        if (strcmp(comando_recebido, "ALERTA") == 0) {
            printf("[TTC_PROC] Comando ALERTA reconhecido!\n");
        } else if (strcmp(comando_recebido, "STATUS") == 0) {
            printf("[TTC_PROC] Comando STATUS processado. Tudo OK!\n");
        } else {
            printf("[TTC_PROC] Comando desconhecido: %s\n", comando_recebido);
        }

        fflush(stdout);
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}
