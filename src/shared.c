#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "shared.h"

// Definição das filas globais
QueueHandle_t xBMSQueue = NULL;
SemaphoreHandle_t xBMSMutex = NULL;
QueueHandle_t xQueueHealth = NULL;
QueueHandle_t xQueueTTC = NULL;
QueueHandle_t xQueueTM = NULL;
QueueHandle_t xQueueTTC_RX = NULL;
QueueHandle_t xQueueTTC_to_MAIN = NULL;
QueueHandle_t xQueueMAIN_to_TTC = NULL;
QueueHandle_t xQueueTM_Request = NULL;
QueueHandle_t xQueueTM_RF = NULL;
QueueHandle_t xQueueTM_CCSDS = NULL;
QueueHandle_t xQueueAlerts = NULL;  // Adicionado para alertas

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

// Lê uma linha da UART (usando stdin no QEMU)
bool shared_uart_read_line(char *buffer, size_t maxlen) {
    // Configura o stdin para modo não-bloqueante
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    // Tenta ler uma linha do stdin
    char *result = fgets(buffer, maxlen, stdin);
    if (result) {
        // Remove a nova linha, se presente
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        printf("[SHARED] Dados UART lidos: %s\n", buffer);
        return true;
    } else {
        printf("[SHARED] Nenhum dado UART disponível.\n");
        return false;
    }
}