#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "shared.h"
#include "tm_task.h"

#define UART0_DR (*(volatile uint32_t *)0x4000C000)
#define UART0_FR (*(volatile uint32_t *)0x4000C018)
#define UART_FR_RXFF 0x40
#define UART_FR_TXFF 0x20

extern QueueHandle_t xQueueTTC_to_MAIN;
extern QueueHandle_t xQueueMAIN_to_TTC;
extern QueueHandle_t xQueueTM_Request;
extern QueueHandle_t xBMSQueue;
extern QueueHandle_t xQueueAlerts;

// 🧩 Utilitário para logs
static void log_info(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
    fflush(stdout);
}

// UART
static int uart_get_char_non_blocking(void) {
    return (UART0_FR & UART_FR_RXFF) ? (char)UART0_DR : -1;
}

static void uart_put_char(char c) {
    while (UART0_FR & UART_FR_TXFF);
    UART0_DR = c;
}

// UART - comando completo
static bool receive_command_from_uart(char* buffer, size_t max_len) {
    static char temp[128] = "";
    static size_t idx = 0;
    static int timeout = 0;

    int c;
    while ((c = uart_get_char_non_blocking()) != -1) {
        uart_put_char((char)c);
        timeout = 0;

        if (c == '\n' || c == '\r') {
            uart_put_char('\n');
            temp[idx] = '\0';
            strncpy(buffer, temp, max_len);
            buffer[max_len - 1] = '\0';
            idx = 0;
            return true;
        }
        if (idx < sizeof(temp) - 1) {
            temp[idx++] = (char)c;
        }
    }

    if (idx > 0 && ++timeout >= 1000) {
        log_info("[MAIN_SO] ⚠️ Timeout UART. Buffer parcial: %s", temp);
        idx = timeout = 0;
    }

    static int dbg = 0;
    if (++dbg % 500 == 0)
        log_info("[MAIN_SO] Aguardando entrada UART... (UART_FR: 0x%08lX)", UART0_FR);

    return false;
}

// 🛰️ MAIN TASK
void vMainTask(void *pvParameters) {
    (void)pvParameters;
    log_info("[MAIN_SO] iniciado.");

    if (!xQueueTTC_to_MAIN) log_info("[MAIN_SO] ❌ Fila xQueueTTC_to_MAIN não inicializada!");

    char command[128];
    Command_t recebido;
    CommandResponse_t resposta;
    int idleCount = 0;

    while (1) {
        bool uart_ok = receive_command_from_uart(command, sizeof(command));
        if (uart_ok) {
            char proc[64], param[64];
            if (sscanf(command, "%63[^:]:%63[^\n]", proc, param) == 2 && strcmp(proc, "main_so") == 0) {
                strncpy(recebido.comando, param, sizeof(recebido.comando) - 1);
                recebido.comando[sizeof(recebido.comando) - 1] = '\0';
            } else {
                log_info("[MAIN_SO] ⚠️ Comando inválido ou não suportado: %s", command);
                continue;
            }
        } else if (xQueueReceive(xQueueTTC_to_MAIN, &recebido, 0) != pdTRUE) {
            if (++idleCount >= 120) {
                log_info("[MAIN_SO] MARIA-SAT está operando normalmente.");
                idleCount = 0;
            }
            vTaskDelay(pdMS_TO_TICKS(10));
            continue;
        }

        log_info("[MAIN_SO] Comando processado: %s", recebido.comando);
        memset(&resposta, 0, sizeof(resposta));
        strncpy(resposta.comando, recebido.comando, sizeof(resposta.comando) - 1);

        // Executando ações de comando
        if (strcmp(recebido.comando, "LIGAR_CAMERA") == 0) {
            ligar_camera(); strcpy(resposta.status, "Implementado");
        } else if (strcmp(recebido.comando, "DESLIGAR_CAMERA") == 0) {
            desligar_camera(); strcpy(resposta.status, "Implementado");
        } else if (strcmp(recebido.comando, "REPORTAR_TELEMETRIA") == 0) {
            reportar_telemetria(); strcpy(resposta.status, "Implementado");
        } else if (strcmp(recebido.comando, "REPORTAR_BMS") == 0) {
            reportar_bms(); strcpy(resposta.status, "Implementado");
        } else if (strcmp(recebido.comando, "REPORTAR_ALERTAS") == 0) {
            reportar_alertas(); strcpy(resposta.status, "Implementado");
        } else if (strncmp(recebido.comando, "ATIVAR_MOTORES_", 15) == 0) {
            int tempo = atoi(recebido.comando + 15);
            if (tempo > 0) {
                ativar_motores(tempo);
                strcpy(resposta.status, "Implementado");
            } else {
                log_info("[MAIN_SO] ❌ Tempo inválido em comando: %s", recebido.comando);
                strcpy(resposta.status, "Erro no comando");
            }
        } else {
            snprintf(resposta.status, sizeof(resposta.status), "Desconhecido: %.15s", recebido.comando);
            log_info("[MAIN_SO] ❓ Comando não reconhecido: %s", recebido.comando);
        }

        // Enviar resposta para TTC_PROC
        if (xQueueSend(xQueueMAIN_to_TTC, &resposta, pdMS_TO_TICKS(100)) != pdPASS)
            log_info("[MAIN_SO] ❌ Erro ao enviar resposta.");
        
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

// ⚙️ Ações
void ligar_camera(void)        { log_info("[MAIN_SO] 📷 Câmera ligada."); }
void desligar_camera(void)     { log_info("[MAIN_SO] 📷 Câmera desligada."); }
void ativar_motores(int t)     { log_info("[MAIN_SO] 🚀 Motores ativados por %d ms.", t); }

void reportar_telemetria(void) {
    log_info("[MAIN_SO] 📡 Solicitando telemetria...");
    // Codificar telemetria em CCSDS e enviar
    TelemetryPacket_t telemetry;
    collectTelemetry(&telemetry);
    uint8_t encoded_packet[sizeof(CCSDSPacket_t)];
    ccsds_encode(&telemetry, encoded_packet);  // Codificação de telemetria em CCSDS

    // Enviar pacotes codificados para CCSDS
    if (xQueueSend(xQueueTM_CCSDS, &encoded_packet, pdMS_TO_TICKS(100)) != pdPASS)
        log_info("[MAIN_SO] ❌ Erro ao solicitar telemetria.");
    else
        log_info("[MAIN_SO] ✅ Telemetria enviada para CCSDS.");
}

void reportar_bms(void) {
    BMSData_t bms;
    if (xQueueReceive(xBMSQueue, &bms, pdMS_TO_TICKS(100)) == pdTRUE)
        log_info("[MAIN_SO] 🔋 BMS: V=%.2fV I=%.2fA T=%.2f°C SoC=%.1f%% T=%u",
                 bms.voltage, bms.current, bms.temperature, bms.soc, bms.timestamp);
    else
        log_info("[MAIN_SO] ⚠️ BMS não respondeu.");
}

void reportar_alertas(void) {
    AlertData_t alert;
    bool any = false;
    while (xQueueReceive(xQueueAlerts, &alert, 0) == pdTRUE) {
        log_info("[MAIN_SO] ⚠️ ALERTA às %us: %s", alert.timestamp, alert.message);
        any = true;
    }
    if (!any) log_info("[MAIN_SO] 📢 Nenhum alerta ativo.");
}
