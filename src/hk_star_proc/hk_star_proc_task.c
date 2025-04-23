#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "hk_star_task.h"


void vHKStarTask(void *pvParameters) {
    printf("[HK_STAR_PROC] Tarefa iniciada\n");
    fflush(stdout);

    int roll = 5, pitch = 10, yaw = -3;

    for (;;) {
        printf("[HK_STAR_PROC] Roll: %d° | Pitch: %d° | Yaw: %d°\n", roll, pitch, yaw);
        fflush(stdout);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
