#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

extern void vMainTask(void *pvParameters);

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0); // evita buffering

    xTaskCreate(vMainTask, "MAIN_SO", 256, NULL, 1, NULL); // cria a tarefa
    vTaskStartScheduler(); // inicia o RTOS

    while (1); // fallback se o scheduler falhar

}
