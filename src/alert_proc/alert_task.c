#include "alert.h"
#include "shared.h"
#include <stdio.h>

// === Timestamp simulado ===
static int fake_seconds = 0;

void get_simulated_timestamp(char *buffer, size_t size) {
    int h = (fake_seconds / 3600) % 24;
    int m = (fake_seconds / 60) % 60;
    int s = fake_seconds % 60;
    snprintf(buffer, size, "%02d:%02d:%02d", h, m, s);
    fake_seconds++;
}

// === Função auxiliar para imprimir alertas com horário ===
void send_alert(int tipo, float valor) {
    char timestamp[9];
    get_simulated_timestamp(timestamp, sizeof(timestamp));

    switch (tipo) {
        case ALERT_TEMP_HIGH:
            printf("[%s] [ALERT] 🚨 Temperatura crítica detectada: %.2f ºC\n", timestamp, valor);
            break;
        case ALERT_LOW_VOLTAGE:
            printf("[%s] [ALERT] ⚠️ Baixa tensão detectada: %.2f V\n", timestamp, valor);
            break;
        case ALERT_CPU_HIGH:
            printf("[%s] [ALERT] 🚨 Uso de CPU muito alto: %.2f %%\n", timestamp, valor);
            break;
        case ALERT_GPS_FAIL:
            printf("[%s] [ALERT] 🚨 Falha no GPS detectada!\n", timestamp);
            break;
        case ALERT_STAR_FAIL:
            printf("[%s] [ALERT] 🚨 Falha no Star Tracker detectada!\n", timestamp);
            break;
        default:
            printf("[%s] [ALERT] ⚠️ Alerta desconhecido (tipo %d): %.2f\n", timestamp, tipo, valor);
            break;
    }
}

// === Tarefa principal do ALERT_PROC ===
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
