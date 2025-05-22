#include "FreeRTOS.h"
#include "task.h"
#include "mppt.h"
#include <stdio.h>
#include <stdlib.h>


void vMPPTProcTask(void *pvParameters)
{
    (void)pvParameters;

    printf("[MPPT_PROC] iniciado.\n");
    

    // Embarcado não tem time(), inicializa o random com um valor fixo
    srand(1234); // Corrigido para evitar o uso de time(NULL)

    while (1)
    {
        printf("[MPPT_PROC] Analisando carga solar...\n");

        vTaskDelay(pdMS_TO_TICKS(2000)); // A cada 2 segundos
    }
}