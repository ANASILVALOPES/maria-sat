#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "health_task.h"

void vHealthTask(void *pvParameters) {
    (void)pvParameters;
    printf("[HEALTH_PROC] Tarefa iniciada\n");
    fflush(stdout);

    while (1) {
        // Aqui no futuro: coleta dados dos HK_* e armazena/encaminha
        printf("[HEALTH_PROC] Coletando dados de HK...\n");
        fflush(stdout);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
