#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "log_task.h"

void vLogTask(void *pvParameters) {
    (void) pvParameters;

    while (1) {
        FILE *logfile = fopen("/mnt/c/Users/atend/Desktop/maria_sat/interface/logs.txt", "a");  // caminho absoluto para WSL
        if (logfile != NULL) {
            int temp = 40 + rand() % 10;
            int tensao = 3900 + rand() % 100;

            fprintf(logfile, "[%ld] BMS_PROC Temp: %d ºC | Tensão: %d mV\n",
                    xTaskGetTickCount(), temp, tensao);

            fclose(logfile);
        }
        vTaskDelay(pdMS_TO_TICKS(2000));  // delay de 2 segundos
    }
}
