#ifndef MAIN_SO_H
#define MAIN_SO_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// Filas externas
extern QueueHandle_t xQueueTTC_to_MAIN;
extern QueueHandle_t xQueueMAIN_to_TTC;

// Protótipos
void vMainTask(void *pvParameters);

#endif // MAIN_SO_H