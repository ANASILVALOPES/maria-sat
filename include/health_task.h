#ifndef HEALTH_TASK_H
#define HEALTH_TASK_H

#include <stdbool.h>

typedef struct {
    int cpu_usage;          // Em porcentagem (%)
    float power_level;      // Tensão em Volts (V)
    bool gps_status;        // Status do GPS (OK ou FAIL)
    bool star_status;       // Status do Star Tracker (OK ou FAIL)
} HealthData_t;

// Fila para enviar dados de saúde para FDIR_proc
extern QueueHandle_t xQueueHealth;

// Protótipos das funções simuladas
int getCPUUsage(void);
float getPowerLevel(void);
bool getGPSStatus(void);
bool getStarStatus(void);

// Protótipo da tarefa principal do HEALTH
void vHEALTHProcTask(void *pvParameters);

#endif // HEALTH_TASK_H
