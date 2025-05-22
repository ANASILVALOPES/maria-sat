#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

// Include project modules
#include "bms.h"
#include "shared.h"
#include "tm_task.h"
#include "rf_tc_task.h"
#include "rf_tm_task.h"
#include "packet.h"
#include "hk_cpu_task.h"
#include "hk_gps_task.h"
#include "hk_pwr_task.h"
#include "hk_star_task.h"
#include "ccsds_task.h"
#include "log_task.h"
#include "alert.h"
#include "health_task.h"
#include "fdir_task.h"
#include "main.h"

// === Stack sizes and priorities ===
#define STACK_STD       256
#define PRIORITY_STD    1
#define PRIORITY_ALERT  2
#define PRIORITY_HIGH   5

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
void vHEALTHProcTask(void *pvParameters);
void vFDIRProcTask(void *pvParameters);
void vCCSDSTask(void *pvParameters);

// === Macro for task creation ===
#define CREATE_TASK(fn, name, stack, prio) \
    if (xTaskCreate(fn, name, stack, NULL, prio, NULL) != pdPASS) { \
        printf("Error creating task: %s\n", name); \
    }

int main(void)
{
    printf("Starting MARIA-SAT execution...\n");
    printf("Initializing MARIA-SAT system...\n");

    // === Queues and mutexes ===
    xBMSQueue = xQueueCreate(5, sizeof(BMSData_t));
    xBMSMutex = xSemaphoreCreateMutex();
    xQueueHealth = xQueueCreate(5, sizeof(HealthData_t));
    xQueueTTC = xQueueCreate(10, sizeof(Command_t));
    xQueueTTC_RX = xQueueCreate(5, sizeof(Command_t));
    xQueueTTC_to_MAIN = xQueueCreate(10, sizeof(Command_t));
    xQueueMAIN_to_TTC = xQueueCreate(5, sizeof(Command_t));
    xQueueTM_Request = xQueueCreate(3, sizeof(char[16]));
    xQueueTM = xQueueCreate(5, sizeof(TelemetryPacket_t));

    // ✅ Correção aplicada aqui:
    xQueueTM_RF = xQueueCreate(5, sizeof(CCSDSPacket_t));
    xQueueTM_CCSDS = xQueueCreate(5, sizeof(CCSDSPacket_t));

    xQueueAlerts = xQueueCreate(10, sizeof(AlertData_t));

    configASSERT(xBMSQueue);
    configASSERT(xBMSMutex);
    configASSERT(xQueueHealth);
    configASSERT(xQueueTTC);
    configASSERT(xQueueTM);
    configASSERT(xQueueTTC_RX);
    configASSERT(xQueueTTC_to_MAIN);
    configASSERT(xQueueMAIN_to_TTC);
    configASSERT(xQueueTM_Request);
    configASSERT(xQueueTM_RF);
    configASSERT(xQueueTM_CCSDS);
    configASSERT(xQueueAlerts);

    // === Task creation ===
    CREATE_TASK(vMainTask,      "MAIN_SO",       512, PRIORITY_HIGH);
    CREATE_TASK(vBMSProcTask,   "BMS_PROC",      STACK_STD, PRIORITY_STD);
    CREATE_TASK(vALERTProcTask, "ALERT_PROC",    configMINIMAL_STACK_SIZE, PRIORITY_ALERT);
    CREATE_TASK(vMPPTProcTask,  "MPPT_PROC",     configMINIMAL_STACK_SIZE, PRIORITY_STD);
    CREATE_TASK(vRFTCTask,      "RF_TC_PROC",    512, PRIORITY_STD);
    xTaskCreate(vTMTask, "TM_PROC", 1024, NULL, 2, NULL);
    CREATE_TASK(vRFTMTask,      "RF_TM_PROC",    512, PRIORITY_STD);
    CREATE_TASK(vPacketProcTask,"PACKET_PROC",   STACK_STD, PRIORITY_STD);
    CREATE_TASK(vHKCPUTask,     "HK_CPU_PROC",   STACK_STD, PRIORITY_STD);
    CREATE_TASK(vHKGpsTask,     "HK_GPS_PROC",   STACK_STD, PRIORITY_STD);
    CREATE_TASK(vHKPWRTask,     "HK_PWR_PROC",   STACK_STD, PRIORITY_STD);
    CREATE_TASK(vHKStarTask,    "HK_STAR_PROC",  STACK_STD, PRIORITY_STD);
    CREATE_TASK(vCCSDSTask,     "CCSDS_PROC",    512, PRIORITY_STD);
    CREATE_TASK(vHEALTHProcTask,"HEALTH",        256, 2);
    CREATE_TASK(vFDIRProcTask,  "FDIR",          256, 2);
    CREATE_TASK(vTTCTask,       "TTC",           512, PRIORITY_STD);
    CREATE_TASK(vLogTask,       "LOG_TASK",      256, 1);

    // === Start scheduler ===
    printf("All tasks successfully created!\n");
    printf("Starting scheduler...\n");
    vTaskStartScheduler();

    // Should never reach here
    printf("Scheduler exited unexpectedly.\n");
    fflush(stdout);
    while (1);
}
