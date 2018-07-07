
#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "intrins.h"
#include "led1.h"

UINT16 xdata temp1;
uint8_t xdata ring_brightness_duty1 = 255;//0-255


void H1_nop_800ns(){
	_nop_();_nop_();_nop_();

}

void H0_nop_400ns(){
	_nop_();
}


void Din_1(void) {
	LED_RING_DATE_PIN_HIGH();
	H1_nop_800ns();
	LED_RING_DATE_PIN_LOW();
	//L1_nop_450ns();
}

void Din_0(void) {
	LED_RING_DATE_PIN_HIGH();
//	H0_nop_400ns();
	_nop_();_nop_(); //延时400ns
	LED_RING_DATE_PIN_LOW();
	//L0_nop_850ns();
}

void Send_8bits(uint8_t dat) {
	uint8_t i;
	for(i=0;i<8;i++) {
		if(dat & 0x80) {//1,for "1",H:0.8us,L:0.45us;
			Din_1();
		} else {		//0,for "0",H:0.4us,L:0.85us
			Din_0();
		}
		dat=dat<<1;
	}
}

void Send_2811_24bits(uint8_t RData, uint8_t GData, uint8_t BData) {
	//G--R--B
	Send_8bits(GData*(ring_brightness_duty1/255));
	Send_8bits(RData*(ring_brightness_duty1/255));
	Send_8bits(BData*(ring_brightness_duty1/255));
 }

 void rst() {
	LED_RING_DATE_PIN_LOW();
	//HAL_Delay (1);
}

void ring_set_color_all_same(uint8_t r, uint8_t g, uint8_t b) {
	uint8_t i;
	for(i=0;i<numLEDs;i++) {
		Send_2811_24bits(r,g,b);
	}
	
}

void ring_display_clear(){
	uint8_t i;
	for( i=0; i<numLEDs; i++) {
		Send_2811_24bits(0,0,0);
	}
}

void led_out(UINT16 flag)			//关掉的灯的数量
{
		for(temp1 = 0;temp1<flag;temp1++)
		{
			ring_display_clear();
		}
}

void led_open(UINT16 flag,uint8_t r, uint8_t g, uint8_t b)//打开灯数量
{
		for(temp1 = 0;temp1<flag;temp1++)
		{
				ring_set_color_all_same(r,g,b);
		}

}



void led1(void)
{
		
    Set_All_GPIO_Quasi_Mode;
	
		while(1)
		{
#if 0			
				ring_set_color_all_same(a+6,b+20,c);						//单个灯，测试用
				Timer1_Delay10ms(1);	
#endif 	

#if 0
			for(wait_wifi = 0;wait_wifi<33;wait_wifi++)
			{
//						ring_display_clear();
//						ring_display_clear();
//						ring_set_color_all_same(a, b, c);
							ring_set_color_all_same(a, b, c);	
			}
			
			Timer1_Delay10ms(50);	
			for(wait_wifi = 0;wait_wifi<33;wait_wifi++)
			{
						Timer1_Delay10ms(5);
						for(p=0;p<wait_wifi;p++)
						{
						ring_set_color_all_same(a+5,b+6,c+15);
						}
			}

					Timer1_Delay10ms(50);																		//旋转灯第一种颜色		

#endif			
			
#if 0
			for(wait_wifi = 0;wait_wifi<16;wait_wifi++)
			{
						ring_display_clear();
						ring_set_color_all_same(a,b,c);			
			}
						Timer1_Delay10ms(20);																		//旋转灯第一种颜色
			for(wait_wifi = 0;wait_wifi<16;wait_wifi++)
			{
						ring_set_color_all_same(a,b,c);
						ring_display_clear();			
			}
						Timer1_Delay10ms(20);	

#endif			
			
#if 0

			for(wait_wifi = 0;wait_wifi<33;wait_wifi++)
			{

						ring_set_color_all_same(0x23,0xa2,0xf5);			
			}
						
			for(wait_wifi = 0;wait_wifi<33;wait_wifi++)
			{
						Timer1_Delay10ms(5);
						for(p=0;p<wait_wifi;p++)
						{
						ring_set_color_all_same(0xff,0xa1,0x23);
						}
			}
			Timer1_Delay10ms(5);
			
#endif			
			
		}
	
}


/************************ (C) COPYRIGHT OAZON ******************END OF FILE****/


