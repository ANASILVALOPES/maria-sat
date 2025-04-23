#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

// Include project modules
#include "shared.h"
#include "tm_task.h"
#include "rf_tc_task.h"
#include "rf_tm_task.h"
#include "ttc_task.h"
#include "packet.h"
#include "hk_cpu_task.h"
#include "hk_gps_task.h"
#include "hk_pwr_task.h"
#include "hk_star_task.h"
#include "health_task.h"
#include "ccsds_task.h"
#include "fdir_task.h"
#include "log_task.h"


// === Stack sizes and priorities for each task ===
#define STACK_STD       256
#define PRIORITY_STD    1
#define PRIORITY_ALERT  2

// === Task Prototypes ===
void vALERTProcTask(void *pvParameters);
void vBMSProcTask(void *pvParameters);
void vMainTask(void *pvParameters);
void vMPPTProcTask(void *pvParameters);
void vTMTask(void *pvParameters);
void vRFTCTask(void *pvParameters);
void vRFTMTask(void *pvParameters);
void vTTCTask(void *pvParameters);
void vPacketProcTask(void *pvParameters);
void vHKCPUTask(void *pvParameters);
void vHKGpsTask(void *pvParameters);
void vHKPWRTask(void *pvParameters);
void vHKStarTask(void *pvParameters);
void vHealthTask(void *pvParameters);
void vCCSDSTask(void *pvParameters);
void vFDIRTask(void *pvParameters);

// === Macro for cleaner task creation ===
#define CREATE_TASK(fn, name, stack, prio)                      \
    if (xTaskCreate(fn, name, stack, NULL, prio, NULL) != pdPASS) { \
        printf("Error creating task: %s\n", name);              \
    }

int main(void)
{
    printf("Initializing MARIA-SAT system...\n");

    // === Global queue and mutex initialization ===
    xBMSQueue = xQueueCreate(5, sizeof(BMSData_t));
    xBMSMutex = xSemaphoreCreateMutex();

    configASSERT(xBMSQueue);
    configASSERT(xBMSMutex);

    // === Create all RTOS tasks ===
    CREATE_TASK(vMainTask,      "MAIN_SO",       configMINIMAL_STACK_SIZE, PRIORITY_STD);
    CREATE_TASK(vBMSProcTask,   "BMS_PROC",      STACK_STD,                PRIORITY_STD);
    CREATE_TASK(vALERTProcTask, "ALERT_PROC",    configMINIMAL_STACK_SIZE, PRIORITY_ALERT);
    CREATE_TASK(vMPPTProcTask,  "MPPT_PROC",     configMINIMAL_STACK_SIZE, PRIORITY_STD);
    CREATE_TASK(vTMTask,        "TM_PROC",       configMINIMAL_STACK_SIZE, PRIORITY_STD);
    CREATE_TASK(vRFTCTask,      "RF_TC_PROC",    configMINIMAL_STACK_SIZE, PRIORITY_STD);
    CREATE_TASK(vRFTMTask,      "RF_TM_PROC",    configMINIMAL_STACK_SIZE, PRIORITY_STD);
    CREATE_TASK(vTTCTask,       "TTC_PROC",      configMINIMAL_STACK_SIZE, PRIORITY_STD);
    CREATE_TASK(vPacketProcTask,"PACKET_PROC",   STACK_STD,                PRIORITY_STD);
    CREATE_TASK(vHKCPUTask,     "HK_CPU_PROC",   STACK_STD,                PRIORITY_STD);
    CREATE_TASK(vHKGpsTask,     "HK_GPS_PROC",   STACK_STD,                PRIORITY_STD);
    CREATE_TASK(vHKPWRTask,     "HK_PWR_PROC",   STACK_STD,                PRIORITY_STD);
    CREATE_TASK(vHKStarTask,    "HK_STAR_PROC",  STACK_STD,                PRIORITY_STD);
    CREATE_TASK(vHealthTask,    "HEALTH_PROC",   STACK_STD,                PRIORITY_STD);
    CREATE_TASK(vCCSDSTask,     "CCSDS_PROC",    STACK_STD,                PRIORITY_STD);
    CREATE_TASK(vFDIRTask,      "FDIR_PROC",     STACK_STD,                PRIORITY_STD);
    xTaskCreate(vLogTask, "LOG_TASK", 256, NULL, 1, NULL);



    // === Start RTOS scheduler ===
    printf("All tasks successfully created!\n");
    printf("Starting scheduler...\n");
    vTaskStartScheduler();

    // Should never reach here
    printf("Scheduler exited unexpectedly.\n");
    while (1);
}
