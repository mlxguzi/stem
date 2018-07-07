#include "buzzer.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "N76E003.h"
#include "Common.h"

void track_search_init()
{
/**********************************************************************
	PWM输出，37.8KHZ
***********************************************************************/
		P11_PushPull_Mode;

//		P11_Quasi_Mode;;
		PWM1_P11_OUTPUT_ENABLE;

/**********************************************************************
	PWM frequency = Fpwm/((PWMPH,PWMPL) + 1) <Fpwm = Fsys/PWM_CLOCK_DIV> 
								= (16MHz/8)/(0x34 + 1)
								= 38KHz (1ms)
***********************************************************************/

		PWM_CLOCK_DIV_8;
		PWMPH = 0x00;
		PWMPL = 0x34;
	
		PWM1H = 0x00;						
		PWM1L = 0x1A;
																			//37.8khz，50%占空比
	  set_LOAD;
    set_PWMRUN;	
		
/**********************************************************************
	接收传感器信号
***********************************************************************/		
		P10_Input_Mode;										//红外传感器输出口
}