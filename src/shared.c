#include "shared.h"
#include <stdio.h>

// Fila de comunicação e proteção de histórico
QueueHandle_t xQueueTTC = NULL;
QueueHandle_t xBMSQueue = NULL;
SemaphoreHandle_t xBMSMutex = NULL;
QueueHandle_t xQueueTM = NULL;
QueueHandle_t xQueueTTC_RX;
QueueHandle_t xQueueTTC_to_MAIN;

// Histórico circular de leituras
static BMSData_t bmsHistory[BMS_HISTORY_SIZE];
static int historyIndex = 0;
static int historyCount = 0;

// Adiciona uma nova leitura ao histórico (com proteção)
void addBMSHistory(BMSData_t data) {
    if (xBMSMutex && xSemaphoreTake(xBMSMutex, portMAX_DELAY)) {
        bmsHistory[historyIndex] = data;
        historyIndex = (historyIndex + 1) % BMS_HISTORY_SIZE;
        if (historyCount < BMS_HISTORY_SIZE) historyCount++;
        xSemaphoreGive(xBMSMutex);
    }
}

// Retorna uma cópia do histórico para leitura segura
void getBMSHistory(BMSData_t *buffer, int *outCount) {
    if (xBMSMutex && xSemaphoreTake(xBMSMutex, portMAX_DELAY)) {
        for (int i = 0; i < historyCount; i++) {
            int index = (historyIndex + i) % BMS_HISTORY_SIZE;
            buffer[i] = bmsHistory[index];
        }
        *outCount = historyCount;
        xSemaphoreGive(xBMSMutex);
    } else {
        *outCount = 0;
    }
}

// (Opcional) Imprime o histórico no terminal para debug
void printBMSHistory(void) {
    BMSData_t buffer[BMS_HISTORY_SIZE];
    int count = 0;
    getBMSHistory(buffer, &count);

    printf("📋 Histórico BMS (%d leituras):\n", count);
    for (int i = 0; i < count; i++) {
        printf("  [%d] V=%.2fV | I=%.2fA | T=%.2f°C | SoC=%.1f%%\n",
               i, buffer[i].voltage, buffer[i].current, buffer[i].temperature, buffer[i].soc);
    }
}
