#include "FreeRTOS.h"
#include "task.h"
extern void vMainTask(void *pvParameters);
int main(void) {
 xTaskCreate(vMainTask, "MAIN_SO", 256, NULL, 1, NULL);
 vTaskStartScheduler();
 while (1);
}
