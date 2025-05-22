#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

void vHKCPUTask(void *pvParameters) {
    (void)pvParameters;
    int cpu_usage = 57;   // simulado
    int ram_free = 68;    // em KB, simulado
    static int counter = 0;

    printf("[HK_CPU_PROC] Tarefa iniciada.\n");
    for (;;) {
        // Log a cada 10 segundos (aproximadamente 5000 ticks a 1 tick = 2ms)
        if (counter % 5000 == 0) {
            printf("[HK_CPU_PROC] Uso CPU: %d%% | RAM livre: %dKB\n", cpu_usage, ram_free);
            fflush(stdout);
        }
        counter++;
        vTaskDelay(pdMS_TO_TICKS(2));
    }
}