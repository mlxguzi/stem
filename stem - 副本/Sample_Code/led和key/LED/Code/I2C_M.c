
#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "intrins.h"


/* Private variables ---------------------------------------------------------*/
#define numLEDs 1
sbit LED_input = P0^7 ;

#define LED_RING_DATE_PIN_LOW() (LED_input = 0) 
#define LED_RING_DATE_PIN_HIGH() (LED_input = 1) 

UINT16 jj = 0xff;
UINT16 temp;
uint8_t ring_brightness_duty = 255;//0-255
UINT8  wait_wifi = 0;

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
	Send_8bits(GData*(ring_brightness_duty/255));
	Send_8bits(RData*(ring_brightness_duty/255));
	Send_8bits(BData*(ring_brightness_duty/255));
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
		for(temp = 0;temp<flag;temp++)
		{
			ring_display_clear();
		}
}

void led_open(UINT16 flag,uint8_t r, uint8_t g, uint8_t b)//打开灯数量
{
		for(temp = 0;temp<flag;temp++)
		{
				ring_set_color_all_same(r,g,b);
		}

}
void PWM_ISR (void) interrupt 13
{
    PWM1H = 0x01;							//PWM0 high duty = 1/2 PWM period
		PWM1L = 0xFF;		
		clr_PWMF;               // clear PWM interrupt flag

//    P12 = ~P12;							// GPIO1 show PWM INT
}
void main(void)
{
		
    Set_All_GPIO_Quasi_Mode;
#if 0	
		PWM1_P11_OUTPUT_ENABLE;
		PWM_INT_PWM1;
		PWM_FALLING_INT;
#endif	
//		ring_set_color_all_same(0x51, 0x11, 0xf1);
		
#if 1
		PWM_CLOCK_DIV_8;
		PWMPH = 0x03;							//Setting PWM period
		PWMPL = 0xFF;
		PWM1H = 0x01;							//PWM0 high duty = 1/2 PWM period
		PWM1L = 0xFF;		
		
		set_EPWM;									//Enable PWM interrupt
		set_EA;									
    set_LOAD;
    set_PWMRUN;
//		ring_set_color_all_same(0x55, 0x55, 0x55);

#endif
#if 1			
		  ring_set_color_all_same(0x51, 0x11, 0xf1);
			Timer1_Delay10ms(10);
			ring_display_clear();
			ring_set_color_all_same(0x51, 0x11, 0xf1);
			Timer1_Delay10ms(10);
			ring_display_clear();
			ring_display_clear();
			ring_set_color_all_same(0x51, 0x11, 0xf1);
			Timer1_Delay10ms(10);
			ring_display_clear();
			ring_display_clear();
			ring_display_clear();
//			ring_set_color_all_same(0x51, 0x11, 0xf1);
			Timer1_Delay10ms(10);
#endif
	
#if 0	
			ring_set_color_all_same(0x51, 0x11, 0xf1);
			led_out(31);
			ring_set_color_all_same(0x51, 0x11, 0xf1);
			Timer1_Delay10ms(10);	

			led_out(1);
			ring_set_color_all_same(0x51, 0x11, 0xf1);
			led_out(29);
			ring_set_color_all_same(0x51, 0x11, 0xf1);
			led_out(1);
			
			Timer1_Delay10ms(10);	
			led_out(2);
			ring_set_color_all_same(0x51, 0x11, 0xf1);
			led_out(27);
			ring_set_color_all_same(0x51, 0x11, 0xf1);
			led_out(2);
			Timer1_Delay10ms(10);
#endif		

#if 1
			ring_set_color_all_same(0x51, 0x11, 0xf1);
			led_out(31);
			ring_set_color_all_same(0x51, 0x11, 0xf1);
//			Timer1_Delay10ms(10);	
			
			for(wait_wifi = 0;wait_wifi<15;wait_wifi++)
			{
						Timer1_Delay10ms(10);											//两边向中间靠拢
						led_out(wait_wifi+1);
						ring_set_color_all_same(0x51, 0x11, 0xf1);
						led_out(29-wait_wifi*2);
						ring_set_color_all_same(0x51, 0x11, 0xf1);
						led_out(wait_wifi+1);
						Timer1_Delay10ms(10);
			}
#endif		

#if 0
			for(wait_wifi = 0; wait_wifi< 28;wait_wifi++)//n个灯一起跑
			{
						Timer1_Delay10ms(5);	
						led_out(wait_wifi);
						led_open(5,0x51,0x11,0xf1);
						led_out(27-wait_wifi);
						Timer1_Delay10ms(5);	
			
			}
#endif			


#if 0	
			for(wait_wifi = 0;wait_wifi<16;wait_wifi++)
			{
						ring_display_clear();
						ring_set_color_all_same(0x51, 0x11, 0xf1);			
			}
						Timer1_Delay10ms(20);																		//旋转灯第一种颜色
			for(wait_wifi = 0;wait_wifi<16;wait_wifi++)
			{
						ring_set_color_all_same(0x51, 0x11, 0xf1);
						ring_display_clear();			
			}
						Timer1_Delay10ms(20);	
///////////////////////////////////////////	
						for(wait_wifi = 0;wait_wifi<16;wait_wifi++)
						{
									ring_display_clear();
									ring_set_color_all_same(0xf1, 0xa8, 0xba);			
						}
									Timer1_Delay10ms(20);																		//旋转灯第二种颜色
						for(wait_wifi = 0;wait_wifi<16;wait_wifi++)
						{
									ring_set_color_all_same(0xf1, 0xa8, 0xba);
									ring_display_clear();			
						}
									Timer1_Delay10ms(20);	
///////////////////////////////////////////	
						for(wait_wifi = 0;wait_wifi<16;wait_wifi++)
						{
									ring_display_clear();
									ring_set_color_all_same(0xa3, 0xff, 0xda);			
						}
									Timer1_Delay10ms(20);																		//旋转灯第3种颜色
						for(wait_wifi = 0;wait_wifi<16;wait_wifi++)
						{
									ring_set_color_all_same(0xa3, 0xff, 0xda);
									ring_display_clear();			
						}
									Timer1_Delay10ms(20);	
#endif			
						
#if 0

//////////////////////////////////////////////////////////
						for(wait_wifi=0; wait_wifi< 6;wait_wifi++)
						{
								Timer1_Delay10ms(20);
																	led_out(7-wait_wifi);
										ring_set_color_all_same(0xf1, 0xa8, 0xba);
																	led_out(7-wait_wifi);
										ring_set_color_all_same(0xf1, 0xa8, 0xba);
																	led_out(7-wait_wifi);
										ring_set_color_all_same(0xf1, 0xa8, 0xba);
																	led_out(7-wait_wifi);
										ring_set_color_all_same(0xf1, 0xa8, 0xba);
								Timer1_Delay10ms(20);						
						}
						
						
#endif												
				
#if 0
						
#endif 						
}


/************************ (C) COPYRIGHT OAZON ******************END OF FILE****/


