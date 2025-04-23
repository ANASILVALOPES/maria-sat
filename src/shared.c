#include "shared.h"

QueueHandle_t xBMSQueue = NULL;
SemaphoreHandle_t xBMSMutex = NULL;

static BMSData_t bmsHistory[BMS_HISTORY_SIZE];
static int historyIndex = 0;
static int historyCount = 0;

void addBMSHistory(BMSData_t data) {
    if (xBMSMutex && xSemaphoreTake(xBMSMutex, portMAX_DELAY)) {
        bmsHistory[historyIndex] = data;
        historyIndex = (historyIndex + 1) % BMS_HISTORY_SIZE;
        if (historyCount < BMS_HISTORY_SIZE) historyCount++;
        xSemaphoreGive(xBMSMutex);
    }
}

void getBMSHistory(BMSData_t *buffer, int *outCount) {
    if (xBMSMutex && xSemaphoreTake(xBMSMutex, portMAX_DELAY)) {
        for (int i = 0; i < historyCount; i++) {
            int index = (historyIndex + i) % BMS_HISTORY_SIZE;
            buffer[i] = bmsHistory[index];
        }
        *outCount = historyCount;
        xSemaphoreGive(xBMSMutex);
    }
}
