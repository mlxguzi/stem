#include "N76E003.h"
#include "WS281X.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "intrins.h"
#include "WS281X.h"
#include "string.h"
#include "buzzer.h"
#include "infrared.h"

#define TH0_INIT        1300 
#define TL0_INIT        1300					//¶¨Ê±Æ÷

U8 u8TH0_Tmp,u8TL0_Tmp;
U8 buzzer_blink = 0;

void main(void)
{
	Set_All_GPIO_Quasi_Mode; 
//	track_search_init();
	buzzer_pwm_init();

#if 0	
		TIMER0_MODE1_ENABLE;	
	  u8TH0_Tmp = (65536-TH0_INIT)/256;
    u8TL0_Tmp = (65536-TL0_INIT)%256; 
	
		TH0 = u8TH0_Tmp;
    TL0 = u8TL0_Tmp;
	
	  set_ET0;                                    //enable Timer0 interrupt
    set_EA;                                     //enable interrupts
	
#endif
while(1)
{
		

		
}

		
}


/************************ (C) COPYRIGHT OAZON ******************END OF FILE****/


