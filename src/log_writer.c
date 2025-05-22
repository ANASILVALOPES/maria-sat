#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


void write_logs_periodically() {
    while (1) {
        FILE *fp = fopen("logs.txt", "w");
        if (!fp) {
            perror("Erro ao abrir logs.txt");
            exit(1);
        }

        fprintf(fp, "[ALERT_PROC] iniciado.\n");
        fprintf(fp, "[MAIN_SO] MARIA-SAT está operando normalmente.\n");
        fprintf(fp, "[BMS_PROC] Temp: %d ºC | Tensão: %d mV\n", 40 + rand() % 5, 3900 + rand() % 100);
        fprintf(fp, "[MPPT_PROC] Analisando carga solar...\n");
        fprintf(fp, "[TM_PROC] Enviando telemetria...\n");
        fprintf(fp, "[RF_TC_PROC] Aguardando comando de solo...\n");
        fprintf(fp, "[RF_TM_PROC] Enviando dados simulados para a Terra...\n");
        fprintf(fp, "[TTC_PROC] Comando STATUS processado. Tudo OK!\n");
        fprintf(fp, "[PACKET_PROC] Pacote gerado: <PKT:TEMP=42;TENS=3900>\n");
        fprintf(fp, "[HK_CPU_PROC] Uso CPU: %d%% | RAM livre: %dKB\n", 50 + rand() % 30, 64 + rand() % 10);
        fprintf(fp, "[HK_GPS_PROC] Localização simulada: Lat -23.5505, Lon -46.6333\n");
        fprintf(fp, "[HK_PWR_PROC] Monitorando consumo de energia...\n");
        fprintf(fp, "[HK_STAR_PROC] Roll: 5° | Pitch: 10° | Yaw: -3°\n");
        fprintf(fp, "[HEALTH_PROC] Coletando dados de HK...\n");
        fprintf(fp, "[CCSDS_PROC] Empacotando dados no formato CCSDS...\n");
        fprintf(fp, "[FDIR_PROC] Verificando falhas no sistema...\n");

        fclose(fp);
        sleep(2); // Atualiza a cada 2 segundos
    }
}
