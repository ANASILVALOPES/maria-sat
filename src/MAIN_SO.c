#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

// Tarefa principal do sistema
void vMainTask(void *pvParameters) {
    (void) pvParameters;  // Evita warning de parâmetro não usado

    while (1) {
        printf("MARIA-SAT está rodando com sucesso! \n");

        // Delay de 1 segundo (1000 ms)
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
