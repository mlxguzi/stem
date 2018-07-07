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
#include "rgb.h"

#define TH0_INIT        3000 
#define TL0_INIT        3000					//¶¨Ê±Æ÷

U8 u8TH0_Tmp,u8TL0_Tmp;
U8 RGB_rh = 0x00,RGB_rl = 0x00;//ºôÎüµÆ²âÊÔ
 

U8 delay_rgb = 0,i = 0;

void Timer0_ISR (void) interrupt 1  //interrupt address is 0x000B
{
    TH0 = u8TH0_Tmp;
    TL0 = u8TL0_Tmp;    
		
		RGB_rl++;
		if(RGB_rl >= 0xf1)
		{
				RGB_rh++;
				RGB_rl = 0x00;							
		}

		if(RGB_rh >= 0x06)
		{
				RGB_rh = 0x00;
				RGB_rl = 0x00;
		}

		delay_rgb++;
}
//ÑÓÊ±º¯Êý£¬2ms
void delay_rg(U8 temp)
{
		delay_rgb = 0;
		while( delay_rgb!=temp );
}

void rgb_settings(UINT8 duration,UINT8 cycle,UINT8 interval)
{
		for(i=0;i<cycle;i++)
		{
				pwm_set(0x07,0x01,0x05,0x65,0x05,0x11);								
				delay_rg(interval);	
		}
}

void main(void)
{
	Set_All_GPIO_Quasi_Mode; 
	pwm_rgb_init();
#if 1	
		TIMER0_MODE1_ENABLE;	
	  u8TH0_Tmp = (65536-TH0_INIT)/256;
    u8TL0_Tmp = (65536-TL0_INIT)%256; 
	
		TH0 = u8TH0_Tmp;
    TL0 = u8TL0_Tmp;
	
	  set_ET0;                                    //enable Timer0 interrupt
    set_EA;                                     //enable interrupts
#endif
	
		set_TR0;
while(1)
{
		
		
	
}



}



