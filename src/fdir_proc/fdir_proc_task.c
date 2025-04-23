#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "fdir_task.h"

void vFDIRTask(void *pvParameters) {
    (void)pvParameters;
    printf("[FDIR_PROC] Tarefa iniciada\n");
    fflush(stdout);

    while (1) {
        // Simula monitoramento de falhas
        printf("[FDIR_PROC] Verificando falhas no sistema...\n");
        fflush(stdout);

        // Aqui poderia incluir leitura de dados de HEALTH_PROC ou alertas

        vTaskDelay(pdMS_TO_TICKS(3000)); // a cada 3s
    }
}
