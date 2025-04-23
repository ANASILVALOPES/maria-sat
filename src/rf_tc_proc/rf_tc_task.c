#include "rf_tc_task.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

void vRFTCTask(void *pvParameters)
{
    (void) pvParameters;

    printf("[RF_TC_PROC] iniciado.\n");

    while (1)
    {
        printf("[RF_TC_PROC] Aguardando comando de solo...\n");
        vTaskDelay(pdMS_TO_TICKS(7000)); // A cada 7 segundos
    }
}
