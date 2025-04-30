#ifndef BMS_H
#define BMS_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "shared.h"

extern QueueHandle_t xBMSQueue;

// Protótipo da tarefa de monitoramento BMS
void bms_proc_task(void *pvParameters);

#endif
