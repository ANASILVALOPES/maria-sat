#ifndef SHARED_H
#define SHARED_H

#include <time.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "hk_cpu_task.h"
#include "hk_gps_task.h"
#include "hk_pwr_task.h"
#include "hk_star_task.h"
#include <stdbool.h>

#define BMS_HISTORY_SIZE 10

bool shared_uart_read_line(char *buffer, size_t maxlen);

// Estrutura de dados do BMS
typedef struct {
    float voltage;
    float current;
    float temperature;
    float soc;
    time_t timestamp;
} BMSData_t;

// Estrutura para comandos
typedef struct {
    char comando[32];
} Command_t;

// Estrutura para resposta de comando
typedef struct {
    char comando[32];
    char status[32];
} CommandResponse_t;

// Estrutura para alertas
typedef struct {
    char message[64];
    uint32_t timestamp;
} AlertData_t;

// Estrutura principal de telemetria
typedef struct {
    HKCPUData_t cpuData;
    HKGPSData_t gpsData;
    HKPWRData_t pwrData;
    HKSTARData_t starData;
} TelemetryPacket_t;

// ✅ Corrigido: estrutura CCSDS sem padding e com tamanho real
#pragma pack(push, 1)
typedef struct {
    uint16_t packet_id;
    uint16_t packet_length;
    uint8_t data[sizeof(TelemetryPacket_t)];
    uint16_t checksum;
} CCSDSPacket_t;
#pragma pack(pop)

// Filas compartilhadas
extern QueueHandle_t xBMSQueue;
extern SemaphoreHandle_t xBMSMutex;
extern QueueHandle_t xQueueHealth;
extern QueueHandle_t xQueueTTC;
extern QueueHandle_t xQueueTM;
extern QueueHandle_t xQueueTTC_RX;
extern QueueHandle_t xQueueTTC_to_MAIN;
extern QueueHandle_t xQueueMAIN_to_TTC;
extern QueueHandle_t xQueueTM_Request;
extern QueueHandle_t xQueueTM_RF;
extern QueueHandle_t xQueueTM_CCSDS;
extern QueueHandle_t xQueueAlerts;

// Histórico do BMS
void addBMSHistory(BMSData_t data);
void getBMSHistory(BMSData_t *buffer, int *outCount);
void printBMSHistory(void);

#endif // SHARED_H