#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

void vHKGpsTask(void *pvParameters) {
    (void)pvParameters;
    // Coordenadas simuladas (São Paulo, por exemplo)
    float latitude = -23.5505;
    float longitude = -46.6333;
    static int counter = 0;

    printf("[HK_GPS_PROC] Tarefa iniciada.\n");
    for (;;) {
        // Log a cada 10 segundos (aproximadamente 5000 ticks a 1 tick = 2ms)
        if (counter % 5000 == 0) {
            printf("[HK_GPS_PROC] Localização simulada: Lat %.4f, Lon %.4f\n", latitude, longitude);
            fflush(stdout);
        }
        counter++;
        vTaskDelay(pdMS_TO_TICKS(2));
    }
}