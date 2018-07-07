#include "buzzer.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "N76E003.h"
#include "Common.h"

void pwm_set(UINT8 rh,UINT8 rl,UINT8 gh,UINT8 gl,UINT8 bh,UINT8 bl)
{
		
		PWM2H = bh;						
		PWM2L = bl;
/**********************************************************************
		set_SFRPAGE和clr_SFRPAGE是为了可以设置PWM4和PWM5
***********************************************************************/	
			set_SFRPAGE;	
		PWM4H = gh;						
		PWM4L = gl;
	
		PWM5H = rh;						
		PWM5L = rl;
			clr_SFRPAGE;
/**********************************************************************
		LOAD寄存器置1，PWM才能载入新周期和占空比，更改结束后会硬件置0
***********************************************************************/		
			set_LOAD;
}
void pwm_rgb_init()
{
/**********************************************************************
	p03 r                      p01 g                            p10 b
***********************************************************************/
		PWM2_P10_OUTPUT_ENABLE;			
		PWM4_P01_OUTPUT_ENABLE;
		PWM5_P03_OUTPUT_ENABLE;
	
		PWM_IMDEPENDENT_MODE;
		PWM_CLOCK_DIV_8;
		PWMPH = 0x07;
		PWMPL = 0xCF;
/**********************************************************************
	PWM frequency = Fpwm/((PWMPH,PWMPL) + 1) <Fpwm = Fsys/PWM_CLOCK_DIV> 
								= (16MHz/8)/(0x7CF + 1)
								= 1KHz (1ms)
***********************************************************************/	
		PWM2H = 0x03;						
		PWM2L = 0xCF;
		
		set_SFRPAGE;						//PWM4 and PWM5 duty seting is in SFP page 1
		PWM4H = 0x03;						
		PWM4L = 0xcf;
		PWM5H = 0x03;						
		PWM5L = 0xcf;
		clr_SFRPAGE;											
//-------- PWM start run--------------
    set_LOAD;
    set_PWMRUN;

}
void rgb_off()
{
		pwm_set(0x00,0x00,0x00,0x00,0x00,0x00);
}
void rgb_on()
{
		pwm_set(0x03,0x01,0x01,0xaa,0x06,0xd1);
}

















