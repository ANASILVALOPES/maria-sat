#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

void vMainTask(void *pvParameters)
{
    printf("[MAIN_SO] iniciado.\n");

    while (1)
    {
        printf("[MAIN_SO] MARIA-SAT está operando normalmente.\n");
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
