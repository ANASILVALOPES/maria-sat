#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"


void vCCSDSTask(void *pvParameters) {
    printf("[CCSDS_PROC] Tarefa iniciada\n");
    fflush(stdout);

    while (1) {
        // Simulação de processamento de pacotes CCSDS
        printf("[CCSDS_PROC] Empacotando dados no formato CCSDS...\n");
        fflush(stdout);

        // Aqui viria lógica real de encoding CCSDS
        vTaskDelay(pdMS_TO_TICKS(2000));  // a cada 2 segundos
    }
}
