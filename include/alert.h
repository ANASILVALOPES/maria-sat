#ifndef ALERT_H
#define ALERT_H

#define ALERT_TEMP_HIGH    1
#define ALERT_LOW_VOLTAGE  2
#define ALERT_CPU_HIGH     3
#define ALERT_GPS_FAIL     4
#define ALERT_STAR_FAIL    5

void send_alert(int tipo, float valor);

#endif // ALERT_H
