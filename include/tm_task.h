#ifndef TM_TASK_H
#define TM_TASK_H

#include "shared.h"

// Declarar a função collectTelemetry
void collectTelemetry(TelemetryPacket_t *telemetry);

// Prototipo da tarefa principal do TM
void vTMTask(void *pvParameters);

#endif // TM_TASK_H
