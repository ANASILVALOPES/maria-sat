#include "bms.h"
#include "alert.h"
#include "shared.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_TEMP 60.0
#define MIN_VOLTAGE 3.2

extern QueueHandle_t xBMSQueue;
BMSData_t bmsData;

// Simulador de tempo incremental
static unsigned long fake_timestamp = 0;

static float get_random(float min, float max) {
    return ((float)rand() / RAND_MAX) * (max - min) + min;
}

static void calculate_soc() {
    float max_voltage = 4.2;
    float min_voltage = 3.0;
    bmsData.soc = ((bmsData.voltage - min_voltage) / (max_voltage - min_voltage)) * 100.0;
    if (bmsData.soc > 100.0) bmsData.soc = 100.0;
    if (bmsData.soc < 0.0) bmsData.soc = 0.0;
}

void vBMSProcTask(void *pvParameters) {
    (void)pvParameters;
    srand(123); // Semente fixa para testes previsíveis
    xBMSQueue = xQueueCreate(10, sizeof(BMSData_t));

    while (1) {
        bmsData.voltage = get_random(3.0, 4.2);
        bmsData.current = get_random(0.5, 2.0);
        bmsData.temperature = get_random(25.0, 70.0);
        bmsData.timestamp = fake_timestamp++; // Simula o tempo

        calculate_soc();

        xQueueSend(xBMSQueue, &bmsData, 0);
        addBMSHistory(bmsData); // Atualiza histórico

        if (bmsData.temperature > MAX_TEMP) {
            send_alert(ALERT_TEMP_HIGH, bmsData.temperature);
        }
        if (bmsData.voltage < MIN_VOLTAGE) {
            send_alert(ALERT_LOW_VOLTAGE, bmsData.voltage);
        }

        printf("[BMS] V=%.2fV I=%.2fA T=%.2f°C SoC=%.1f%% Timestamp=%lld\n",
            bmsData.voltage, bmsData.current, bmsData.temperature, bmsData.soc, (long long)bmsData.timestamp);
     


        vTaskDelay(pdMS_TO_TICKS(1000)); // Espera 1 segundo
    }
}
