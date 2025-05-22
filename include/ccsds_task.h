#ifndef CCSDS_TASK_H
#define CCSDS_TASK_H

#include <stdint.h>
#include "shared.h"

// Função que calcula o checksum (XOR simples)
uint16_t calculate_checksum(uint8_t *data, uint16_t length);

// Codifica um pacote de telemetria em formato CCSDS
void ccsds_encode(TelemetryPacket_t *telemetry, uint8_t *encoded_packet);

// Decodifica um pacote CCSDS e extrai a telemetria
int ccsds_decode(uint8_t *encoded_packet, TelemetryPacket_t *telemetry);

// Task de processamento CCSDS
void vCCSDSTask(void *pvParameters);

#endif // CCSDS_TASK_H
