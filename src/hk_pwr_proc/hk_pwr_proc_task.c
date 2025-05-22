#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "hk_pwr_task.h"


void vHKPWRTask(void *pvParameters) {
    (void) pvParameters;

    printf("[HK_PWR_PROC] Tarefa iniciada\n");
    fflush(stdout);
    

    while (1) {
        printf("[HK_PWR_PROC] Monitorando consumo de energia...\n");
        fflush(stdout);
        vTaskDelay(pdMS_TO_TICKS(3000)); // Espera 3 segundos
    }
}
