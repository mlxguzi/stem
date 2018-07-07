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


#define I2C_CLOCK               13
#define EEPROM_SLA              0xA4

///定时器参数
#define TH0_INIT        50000 
#define TL0_INIT        50000
#define TH1_INIT        25000 
#define TL1_INIT        25000
UINT8 u8TH0_Tmp,u8TL0_Tmp,u8TH1_Tmp,u8TL1_Tmp;
UINT8 num1 = 0,key_num = 0;

///
///长按按键判断
UINT8	trg = 0 ;
UINT8 cont = 0 ,readdata = 0;

///
UINT8 data_received[7], data_num = 0 ,j = 0;
UINT8 data_slave[34];
UINT8 state = 0,i = 0,state_data = 0;

#define LED_1  P10 = 0;P10 = 1;
#define LED_0  P10 = 1;P10 = 0;
sbit KEY=P1^1;

void Find(UINT8 num)
{
		for(j=0;j<34;j++)
		{
			if(data_received[j] == num)
						return ;
		}
}

void keyread(void)
{
		readdata = KEY ^ 0xff;
		trg = readdata &(readdata^cont);
		cont = readdata;
}

//========================================================================================================
void I2C_ISR(void) interrupt 6
{
    switch (I2STAT)
    {
        case 0x00:
            STO = 1;
            break;

        case 0x60:
            AA = 1;
            break;
				
        case 0x68:
						P02 = 0;
            while(1);
            break;

        case 0x80:
            //P3 = 0x80;
						data_received[data_num] = I2DAT;
            data_num++;
				
            if (data_num == 7)
                AA = 0;
            else
                AA = 1;
            break;
        case 0x88:
            //P3 = 0x88;
            data_received[data_num] = I2DAT;
            data_num = 0;
            AA = 1;
            break;

        case 0xA0:
            //P3 = 0xA0;
            AA = 1;
            break;
 
        case 0xA8:
            //P3 = 0xA0;
           I2DAT = 0x00;//data_received[data_num];
           data_num++;
            AA = 1;
            break;
        
        case 0xB8:
            
						I2DAT = data_slave[state_data];  //根据主机发送的信号返回相应数据
            
						state_data++;
				
//					data_num++;

            AA = 1;
            break;

        case 0xC0:
            AA = 1;
            break; 

        case 0xC8:
            //P3 = 0xC8;
            AA = 1;
            break;        
    }

    SI = 0;
//    while(STO);
}


void Timer0_ISR (void) interrupt 1  //interrupt address is 0x000B
{
    TH0 = u8TH0_Tmp;
    TL0 = u8TL0_Tmp;    
    
    num1++;                     // GPIO1 toggle when interrupt

}

void Timer1_ISR (void) interrupt 3  //interrupt address is 0x001B
{
    TH1 = u8TH1_Tmp;
    TL1 = u8TL1_Tmp;   

//    P03 = ~P03;                     //P0.3 toggle when interrupt
}


//========================================================================================================
void Init_I2C(void)
{
    P13_Quasi_Mode;                         //set SCL (P13) is Quasi mode
    P14_Quasi_Mode;                         //set SDA (P14) is Quasi mode
    
    SDA = 1;                                //set SDA and SCL pins high
    SCL = 1;
    
    set_P0SR_6;                             //set SCL (P06) is  Schmitt triggered input select.
    
    set_EI2C;                               //enable I2C interrupt by setting IE1 bit 0
    set_EA;

    I2ADDR = EEPROM_SLA;                    //define own slave address
    set_I2CEN;                              //enable I2C circuit
    set_AA;
}

//========================================================================================================
void main(void)
{
		
    Set_All_GPIO_Quasi_Mode;
		
    /* Initial I2C function */
    Init_I2C();                                 //initial I2C circuit

	
	
//////////////////////////////////////定时器0
		TIMER0_MODE1_ENABLE;
		TIMER1_MODE1_ENABLE;
		
		u8TH0_Tmp = (65536-TH0_INIT)/256;
    u8TL0_Tmp = (65536-TL0_INIT)%256;    
    u8TH1_Tmp = (65536-TH1_INIT)/256;
    u8TL1_Tmp = (65536-TL1_INIT)%256;
		
		TH0 = u8TH0_Tmp;
    TL0 = u8TL0_Tmp;
    TH1 = u8TH1_Tmp;
    TL1 = u8TL1_Tmp;
    
    set_ET0;                                    //enable Timer0 interrupt
    set_ET1;                                    //enable Timer1 interrupt开启中断
    set_EA;                                     //enable interrupts

    set_TR1;                                    //Timer1 run
		set_TR0;                                    //Timer0 run
/////////////////////////////////////////////////////////////
		while(1)
		{
			keyread();																//判断长按
			if(KEY)    															//是否按下					
			{
				Timer0_Delay1ms(20);										//延时20ms，防抖
				if(KEY)
				{
						key_num++;													//按下按键，开始记录次数	
						num1 = 0;
				}
			
			}
			
			if( (num1>40) && (key_num ==1) )					//2秒后检测按键按下次数。为1则为Press
			{
					num1 = 0;
					key_num = 0; 
																								//将事件存入
			}
			
			if( num1<40 && key_num == 2 )												//检测按键按下次数。2则为Double Click
			{
					num1 = 0;
					key_num = 0; 
																								//将事件存入
			}
			
			if( readdata == 0xff && trg == 0x00 && num1>40 )		//Hold
			{
					num1 = 0;
				  key_num = 0;													//写入事件
				
			}
		
		}
/////////////////////////////////////////////////////////////////		
		
		while (1);
/* =================== */
}

