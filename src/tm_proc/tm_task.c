#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include <string.h>
#include "shared.h"
#include "ccsds_task.h"

#define TM_REQUEST_SIGNAL "REQUEST"

extern QueueHandle_t xQueueTM_Request;
extern QueueHandle_t xQueueTM_RF;
extern QueueHandle_t xQueueTM_CCSDS;

void collectTelemetry(TelemetryPacket_t *telemetry)
{
    telemetry->cpuData.cpu_usage = (telemetry->cpuData.cpu_usage < 0) ? 0 :
                                   (telemetry->cpuData.cpu_usage > 100) ? 100 : telemetry->cpuData.cpu_usage;
    telemetry->cpuData.ram_free_kb = 64;
    telemetry->gpsData.lat = -23.5505f;
    telemetry->gpsData.lon = -46.6333f;

    if (telemetry->pwrData.voltage < 0) telemetry->pwrData.voltage = 0;
    if (telemetry->pwrData.current < 0) telemetry->pwrData.current = 0;

    telemetry->starData.roll = 3;
    telemetry->starData.pitch = 7;
    telemetry->starData.yaw = -1;
}

void vTMTask(void *pvParameters)
{
    (void)pvParameters;
    printf("[TM_PROC] iniciado.\n");

    TelemetryPacket_t telemetry;
    uint8_t encoded_packet[sizeof(CCSDSPacket_t)];
    char requestSignal[16];
    const TickType_t xTimeout = pdMS_TO_TICKS(5000);

    while (1)
    {
        if (xQueueReceive(xQueueTM_Request, &requestSignal, xTimeout) == pdTRUE)
        {
            if (strncmp(requestSignal, TM_REQUEST_SIGNAL, strlen(TM_REQUEST_SIGNAL)) == 0)
                printf("[TM_PROC] Sinal de coleta recebido via comando.\n");
            else {
                printf("[TM_PROC] Sinal inválido: %s\n", requestSignal);
                continue;
            }
        }
        else
            printf("[TM_PROC] Nenhum sinal recebido. Enviando telemetria automaticamente.\n");

        collectTelemetry(&telemetry);
        ccsds_encode(&telemetry, encoded_packet);

        printf("[TM_PROC] ✅ Telemetria codificada com sucesso em CCSDS.\n");

        if (xQueueSend(xQueueTM_RF, &encoded_packet, pdMS_TO_TICKS(100)) != pdPASS)
            printf("[TM_PROC] Erro ao enviar TM para RF_TM.\n");
        else
            printf("[TM_PROC] ✅ TM enviada para RF_TM.\n");

        if (xQueueSend(xQueueTM_CCSDS, &encoded_packet, pdMS_TO_TICKS(100)) != pdPASS)
            printf("[TM_PROC] Erro ao enviar TM para CCSDS.\n");
        else
            printf("[TM_PROC] ✅ TM enviada para CCSDS.\n");

        fflush(stdout);
    }
}
