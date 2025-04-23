#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

void vRFTMTask(void *pvParameters) {
    for (;;) {
        printf("[RF_TM_PROC] Enviando dados simulados para a Terra...\n");
        fflush(stdout);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
