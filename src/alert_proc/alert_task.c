#include "alert.h"
#include "shared.h"
#include <stdio.h>

void send_alert(int tipo, float valor) {
    switch (tipo) {
        case ALERT_TEMP_HIGH:
            printf("[ALERT] 🚨 Temperatura crítica detectada: %.2f ºC\n", valor);
            break;
        case ALERT_LOW_VOLTAGE:
            printf("[ALERT] ⚠️ Baixa tensão detectada: %.2f V\n", valor);
            break;
        case ALERT_CPU_HIGH:
            printf("[ALERT] 🚨 Uso de CPU muito alto: %.2f %%\n", valor);
            break;
        case ALERT_GPS_FAIL:
            printf("[ALERT] 🚨 Falha no GPS detectada!\n");
            break;
        case ALERT_STAR_FAIL:
            printf("[ALERT] 🚨 Falha no Star Tracker detectada!\n");
            break;
        default:
            printf("[ALERT] ⚠️ Alerta desconhecido (tipo %d): %.2f\n", tipo, valor);
            break;
    }
}

void vALERTProcTask(void *pvParameters) {
    (void)pvParameters;

    printf("[ALERT_PROC] iniciado.\n");

    BMSData_t leitura;

    while (1) {
        if (xQueueReceive(xBMSQueue, &leitura, portMAX_DELAY) == pdTRUE) {
            if (leitura.temperature > 60.0) {
                send_alert(ALERT_TEMP_HIGH, leitura.temperature);
            }
        }
    }
}
