#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

void vMPPTProcTask(void *pvParameters)
{
    (void) pvParameters;

    printf("[MPPT_PROC] iniciado.\n");

    while (1)
    {
        printf("[MPPT_PROC] Analisando carga solar...\n");
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
