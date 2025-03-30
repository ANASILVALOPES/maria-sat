#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <string.h>

void vMainTask(void *pvParameters)
{
    (void) pvParameters;  // Evita warning de parâmetro não usado

    const char *msg = "MARIA-SAT está rodando com sucesso!\n";

    while (1)
    {
        printf("%s", msg);
        fflush(stdout);  // Garante que o texto seja impresso imediatamente

        // Delay de 1 segundo (1000 ms)
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}