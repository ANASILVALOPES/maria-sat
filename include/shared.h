#ifndef SHARED_H
#define SHARED_H

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#define ALERTA_TEMPERATURA_CRITICA 45
#define BMS_HISTORY_SIZE 5

typedef struct {
    int temperatura;
    int tensao;
} BMSData_t;

// Fila de comunicação entre BMS e ALERT
extern QueueHandle_t xBMSQueue;

// Mutex para proteger acesso ao buffer histórico
extern SemaphoreHandle_t xBMSMutex;

// Buffer circular para armazenar histórico das leituras
extern BMSData_t xBMSHistory[BMS_HISTORY_SIZE];
extern int bmsHistoryIndex;

// Função para adicionar uma leitura ao buffer
void addBMSHistory(BMSData_t novaLeitura);

// Função para obter uma cópia segura do histórico
void getBMSHistory(BMSData_t *destino, int *count);

#endif /* SHARED_H */
