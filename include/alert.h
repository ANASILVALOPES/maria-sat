#ifndef ALERT_H
#define ALERT_H

// Definições dos tipos de alerta
#define ALERT_TEMP_HIGH     1
#define ALERT_LOW_VOLTAGE   2
#define ALERT_CPU_HIGH      3
#define ALERT_GPS_FAIL      4
#define ALERT_STAR_FAIL     5
#define ALERT_OVERCURRENT   6

#ifdef __cplusplus
extern "C" {
#endif

// Prototipagem da função de envio de alerta
void send_alert(int tipo, float valor);

#ifdef __cplusplus
}
#endif

#endif // ALERT_H
