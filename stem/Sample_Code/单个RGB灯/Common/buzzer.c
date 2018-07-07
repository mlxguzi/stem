#include "buzzer.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "N76E003.h"
#include "Common.h"




/**********************************************************************
	PWM frequency = Fpwm/((PWMPH,PWMPL) + 1) <Fpwm = Fsys/PWM_CLOCK_DIV> 
								= (16MHz/8)/(0x7CF + 1)
								= 1KHz (1ms)
***********************************************************************/
void buzzer_pwm_init()
{		
		P10_PushPull_Mode;
		PWM2_P10_OUTPUT_ENABLE;


		PWM_CLOCK_DIV_8;
		PWMPH = 0x07;
		PWMPL = 0xCF;
	
		PWM2H = 0x00;						
		PWM2L = 0x55;
	
	  set_LOAD;
    set_PWMRUN;	
}

void buzzer_off()
{
		PWM2H = 0x00;						
		PWM2L = 0x00;
}
void buzzer_open(unsigned char duration,unsigned char cycle,unsigned char interval)
{
		
		
}

void buzzer_blin()
{

}
void buzzer_sos()
{

}













