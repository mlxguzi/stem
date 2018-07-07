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


UINT8 data_received[7], data_num = 0 ,j = 0;
UINT8 data_slave[34];
UINT8 state = 0,i = 0,state_data = 0;

#define LED_1  P10 = 0;P10 = 1;
#define LED_0  P10 = 1;P10 = 0;
void Find(UINT8 num)
{
		for(j=0;j<34;j++)
		{
			if(data_received[j] == num)
						return ;
		}
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
		/*while(1)
		{
			if( data_numT == 7 )
			{
				for(i = 0;i<34;i++)
				{
					if(data_received[i] == 0x03)
							break;
				}
		  }
		}*/
		while(AA);									//判断主机是否发送完数据
		for( i=state; i<7 ; i++ )
		{
			data_slave[i] = data_received[i];
			data_num = 0;
			state++;
		}
		
		
		Find(0x03);	
		if( (data_slave[j+1]==0x51) && (data_slave[j+2]==0x52) )
		{
						P10_PushPull_Mode;
						LED_1;																				//判断接收的数据并执行要求
						Timer0_Delay1ms(30);
						LED_0;
		}
/*
		while(  I2STAT==0xA8 );
		switch( data_received[0] )
		{
			case 0x01:
								state_data = 0;
								break;
			case 0x02:
								state_data = 2;
								break;
			case 0x03:
								state_data = 4;	
								break;
		}
 */ 
		while (1);
/* =================== */
}

