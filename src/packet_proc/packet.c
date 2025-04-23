#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

void vPacketProcTask(void *pvParameters) {
    int temperatura = 42;
    int tensao = 3900;

    char pacote[64];

    for (;;) {
        // Simula codificação de pacote de telemetria
        sprintf(pacote, "<PKT:TEMP=%d;TENS=%d>", temperatura, tensao);

        // Mostra como seria o "pacote codificado"
        printf("[PACKET_PROC] Pacote gerado: %s\n", pacote);
        fflush(stdout);

        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}
