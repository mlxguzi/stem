//#ifndef	__BUZZER_H__
//#define  __BUZZER_H__


void buzzer_pwm_init();
void buzzer_off();
void buzzer_open(unsigned char duration,unsigned char cycle,unsigned char interval);
void buzzer_blin();
void buzzer_sos();