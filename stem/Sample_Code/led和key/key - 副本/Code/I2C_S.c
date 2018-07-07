/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2015 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

//***********************************************************************************************************
//  Nuvoton Technoledge Corp. 
//  Website: http://www.nuvoton.com
//  E-Mail : MicroC-8bit@nuvoton.com
//  Date   : May/1/2015
//***********************************************************************************************************

//***********************************************************************************************************
//  File Function: N76E003 I2C Slave demo code
//***********************************************************************************************************

#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"

//***********************************************************************************************************
//  N76E885-series I2C slave mode demo code, the Slave address = 0xA4
//
//   ____________            _____________ 
//  |            |   SDA    |             |
//  |            |<-------->|             |
//  |            |          |             |
//  |N76E003(M) |          | N76E003(S) |
//  |(I2C_Master)|          | (I2C_Slave) |
//  |            |   SCL    |             |
//  |            |--------->|             |
//  |____________|          |_____________|
//
//  The protocol of I2C is same the "24LC64"
//***********************************************************************************************************



///定时器参数
#define TH0_INIT        10000 
#define TL0_INIT        10000
#define TH1_INIT        25000 
#define TL1_INIT        25000
UINT8 u8TH0_Tmp,u8TL0_Tmp,u8TH1_Tmp,u8TL1_Tmp;
//UINT8 num1 = 0,key_num = 0,numm = 0 ,i = 0;

//sbit KEY = P1^1;
///////////////////////////////////////////
sbit key_input = P1^1;

#define key_no	   0    //no keys
#define key_click  1    //click keys
#define key_double 2    //double click
#define key_long   3    //long click

#define key_state_0		0
#define key_state_1   1
#define key_state_2   2
#define key_state_3   3 //key states define

#define key_flag	(1<<0)
#define clear_key_flag  (~(1<<0))

#define key_init()	(key_input = 1)
extern UINT8 key_read();

UINT8 timer_10ms_flag = 0;
UINT8 key1;

static unsigned char key_driver(void)
{
	static unsigned char key_state_buffer1 = key_state_0;
	static unsigned char key_timer_cnt1 = 0;
	unsigned char key_return = key_no;
	unsigned char key;
	
	key = key_input;  //read the I/O states
	
	switch(key_state_buffer1)
	{
		case key_state_0:
			if(key == 0)
				key_state_buffer1 = key_state_1; 
				
			break;
		case key_state_1:
			if(key == 0)
			{
				key_timer_cnt1 = 0;
				key_state_buffer1 = key_state_2;
				
				
				
			}
			else
				key_state_buffer1 = key_state_0;
				
			break;  
		case key_state_2:
			if(key == 1) 
			{
				key_return = key_click;  
				key_state_buffer1 = key_state_0;  
			}
			else if(++key_timer_cnt1 >= 100)  
			{
				key_return = key_long;  
				key_state_buffer1 = key_state_3;  
			}
			break;
		case key_state_3:  
			if(key == 1) 
				key_state_buffer1 = key_state_0;  
			break;
	}
	return key_return;
}

unsigned char key_read(void)
{
	static unsigned char key_state_buffer2 = key_state_0;
	static unsigned char key_timer_cnt2 = 0;
	unsigned char key_return = key_no;
	unsigned char key;
	
	key = key_driver();
	
	switch(key_state_buffer2)
	{
		case key_state_0:
			if(key == key_click)
			{
				key_timer_cnt2 = 0;  
				key_state_buffer2 = key_state_1;
			}
			else 
				key_return = key;  
			break;
		case key_state_1:
			if(key == key_click)  
			{
				key_return = key_double;  
				key_state_buffer2 = key_state_0;
			}
			else if(++key_timer_cnt2 >= 50)
			{
									
				key_return = key_click;  
				key_state_buffer2 = key_state_0;  
					
			}
			break;
	}
	
	return key_return;
}

void Timer0_ISR (void) interrupt 1  //interrupt address is 0x000B
{
	
    TH0 = u8TH0_Tmp;
    TL0 = u8TL0_Tmp;    
    timer_10ms_flag |= key_flag;

}



//========================================================================================================
void main(void)
{
		
    Set_All_GPIO_Quasi_Mode;
		P11_Quasi_Mode;
		P11 = 1;	
	
//////////////////////////////////////定时器0
		TIMER0_MODE1_ENABLE;
		TIMER1_MODE1_ENABLE;
		
		clr_T0M;
		clr_T1M;	
	
		u8TH0_Tmp = (65536-TH0_INIT)/256;
    u8TL0_Tmp = (65536-TL0_INIT)%256;    
		u8TH1_Tmp = (65536-TH1_INIT)/256;
    u8TL1_Tmp = (65536-TL1_INIT)%256;
	
		TH0 = u8TH0_Tmp;
    TL0 = u8TL0_Tmp;
		TH1 = u8TH1_Tmp;
    TL1 = u8TL1_Tmp;
    
    set_ET0;                                    //enable Timer0 interrupt
		set_ET1;                                    //enable Timer1 interrupt
    set_EA;                                     //enable interrupts
	
		set_TR0;                                    //Timer0 run
		
		
		
		
/////////////////////////////////////////////////////////////
#if 1	
	while(1)
		{
				
				if(timer_10ms_flag&key_flag == 1)
				{
						timer_10ms_flag &= clear_key_flag;						
						key1 = key_read();
					
						switch(key1)
						{
								case key_click:
									clr_GPIO1;
									break;
								case key_double:
									set_GPIO1;
									break;
								case key_long:
									P12 = ~P12;
									break;
						}
				}
				
		
		}
#endif			
			
			
}

