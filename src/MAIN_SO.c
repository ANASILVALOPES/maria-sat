#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
void vMainTask(void *pvParameters) {
 while (1) {
 printf("MARIA-SAT est rodando com sucesso! \n");
 vTaskDelay(pdMS_TO_TICKS(1000));
 }
}
