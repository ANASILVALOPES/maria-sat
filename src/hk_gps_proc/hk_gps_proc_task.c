#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

void vHKGpsTask(void *pvParameters) {
    // Coordenadas simuladas (São Paulo, por exemplo)
    float latitude = -23.5505;
    float longitude = -46.6333;

    for (;;) {
        printf("[HK_GPS_PROC] Localização simulada: Lat %.4f, Lon %.4f\n", latitude, longitude);
        fflush(stdout);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
