#include "tm_task.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

void vTMTask(void *pvParameters)
{
    (void) pvParameters;

    printf("[TM_PROC] iniciado.\n");

    while (1)
    {
        printf("[TM_PROC] Enviando telemetria...\n");
        vTaskDelay(pdMS_TO_TICKS(5000)); // Espera 5 segundos
    }
}
