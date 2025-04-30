#ifndef SHARED_H
#define SHARED_H

#include <time.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"

#define BMS_HISTORY_SIZE 10

// Estrutura de dados do BMS, compartilhada entre tarefas
typedef struct {
    float voltage;
    float current;
    float temperature;
    float soc;
    time_t timestamp;
} BMSData_t;

// Estrutura de dados para comandos do TTC
typedef struct {
    char comando[32];  // até 31 caracteres + \0
} Command_t;

// Estrutura de resposta
typedef struct {
    char comando[32];
    char status[32];
} CommandResponse_t;


// Variáveis globais compartilhadas (definidas em shared.c)
extern QueueHandle_t xBMSQueue;
extern SemaphoreHandle_t xBMSMutex;
extern QueueHandle_t xQueueHealth;
extern QueueHandle_t xQueueTTC;
extern QueueHandle_t xQueueTM;
extern QueueHandle_t xQueueTTC_RX;
extern QueueHandle_t xQueueTTC_to_MAIN;
extern QueueHandle_t xQueueMAIN_to_TTC;
extern QueueHandle_t xQueueTM_Request;



// Funções para manipular histórico circular
void addBMSHistory(BMSData_t data);
void getBMSHistory(BMSData_t *buffer, int *outCount);
void printBMSHistory(void); // (opcional) Para debug no terminal

#endif // SHARED_H
