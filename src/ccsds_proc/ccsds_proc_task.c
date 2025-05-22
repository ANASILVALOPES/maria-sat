#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include <string.h>
#include "ccsds_task.h"
#include "shared.h"

extern QueueHandle_t xQueueTM_CCSDS;

// Função de checksum simples (XOR)
uint16_t calculate_checksum(uint8_t *data, uint16_t length) {
    uint16_t checksum = 0;
    for (int i = 0; i < length; i++) {
        checksum ^= data[i];
    }
    return checksum;
}

// Função de codificação CCSDS
void ccsds_encode(TelemetryPacket_t *telemetry, uint8_t *encoded_packet) {
    CCSDSPacket_t ccsds_packet = {0};  // ✅ Correção: zera toda a estrutura

    ccsds_packet.packet_id = 0x01;
    ccsds_packet.packet_length = sizeof(TelemetryPacket_t) + sizeof(ccsds_packet.checksum);

    memcpy(ccsds_packet.data, telemetry, sizeof(TelemetryPacket_t));
    ccsds_packet.checksum = calculate_checksum(ccsds_packet.data, sizeof(TelemetryPacket_t));

    memcpy(encoded_packet, &ccsds_packet, sizeof(CCSDSPacket_t));
}

// Função de decodificação CCSDS
int ccsds_decode(uint8_t *encoded_packet, TelemetryPacket_t *telemetry) {
    CCSDSPacket_t *ccsds_packet = (CCSDSPacket_t *)encoded_packet;
    uint16_t checksum = calculate_checksum(ccsds_packet->data, sizeof(TelemetryPacket_t));

    if (checksum != ccsds_packet->checksum) {
        printf("[CCSDS_PROC] Erro: Checksum inválido! Esperado: 0x%04X, Recebido: 0x%04X\n", checksum, ccsds_packet->checksum);
        return -1;
    }

    memcpy(telemetry, ccsds_packet->data, sizeof(TelemetryPacket_t));
    return 0;
}

// Task CCSDS
void vCCSDSTask(void *pvParameters)
{
    (void) pvParameters;
    TelemetryPacket_t tm;
    uint8_t encoded_packet[sizeof(CCSDSPacket_t)];

    printf("[CCSDS_PROC] Tarefa iniciada\n");

    while (1)
    {
        if (xQueueReceive(xQueueTM_CCSDS, encoded_packet, pdMS_TO_TICKS(1000)) == pdTRUE)

        {
            printf("[CCSDS_PROC] Empacotando dados no formato CCSDS...\n");

            if (ccsds_decode(encoded_packet, &tm) == 0) {
                printf("[CCSDS_PROC] ✅ Telemetria decodificada com sucesso.\n");
            } else {
                printf("[CCSDS_PROC] ❌ Erro ao decodificar telemetria CCSDS.\n");
            }
        }
        else
        {
            printf("[CCSDS_PROC] Aguardando TM...\n");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
