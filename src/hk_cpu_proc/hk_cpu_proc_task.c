#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

void vHKCPUTask(void *pvParameters) {
    int cpu_usage = 57;   // simulado
    int ram_free = 68;    // em KB, simulado

    for (;;) {
        printf("[HK_CPU_PROC] Uso CPU: %d%% | RAM livre: %dKB\n", cpu_usage, ram_free);
        fflush(stdout);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
