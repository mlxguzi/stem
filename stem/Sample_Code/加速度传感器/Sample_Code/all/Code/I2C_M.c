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
#include "MOVE.h"

#define TH0_INIT        1300 
#define TL0_INIT        1300					//��ʱ��

U8 u8TH0_Tmp,u8TL0_Tmp;
U8 buzzer_blink = 0;




void main(void)
{
	Set_All_GPIO_Quasi_Mode; 
//	track_search_init();
//	buzzer_pwm_init();

#if 0	
		TIMER0_MODE1_ENABLE;	
	  u8TH0_Tmp = (65536-TH0_INIT)/256;
    u8TL0_Tmp = (65536-TL0_INIT)%256; 
	
		TH0 = u8TH0_Tmp;
    TL0 = u8TL0_Tmp;
	
	  set_ET0;                                    //enable Timer0 interrupt
    set_EA;                                     //enable interrupts
	
#endif
	delay(500);		//�ϵ���ʱ		
	init_uart();
	InitMPU6050();	//��ʼ��MPU6050
	delay(150);
	while(1)
	{
	
		Display10BitData(GetData(ACCEL_XOUT_H));	//��ʾX����ٶ�
		Display10BitData(GetData(ACCEL_YOUT_H));	//��ʾY����ٶ�
		Display10BitData(GetData(ACCEL_ZOUT_H));	//��ʾZ����ٶ�
		Display10BitData(GetData(GYRO_XOUT_H));		//��ʾX����ٶ�
		Display10BitData(GetData(GYRO_YOUT_H));		//��ʾY����ٶ�
		Display10BitData(GetData(GYRO_ZOUT_H));		//��ʾZ����ٶ�
     
		SeriPushSend(0x0d); 
        SeriPushSend(0x0a);//���У��س�
		delay(2000);
	}
	
	

		
}


/************************ (C) COPYRIGHT OAZON ******************END OF FILE****/


