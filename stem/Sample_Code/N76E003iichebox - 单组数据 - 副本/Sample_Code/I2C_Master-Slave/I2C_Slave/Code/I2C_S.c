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
#include "string.h"
#include "led1.h"

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
#define EEPROM_SLA              0x40


UINT8 xdata data_received[100], data_num = 0;
UINT8 xdata data_store[100],i=0,j=0;
UINT8 flag = 0;
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
            //P3 = 0x60;
            break;
				
        case 0x68:
						P02 = 0;
            while(1);
            break;

        case 0x80:
            //P3 = 0x80;
            data_received[data_num] = I2DAT;

            data_num++;

            /*if (data_num == 34)
                AA = 0;
            else*/
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
            //P3 = 0xA0;//从这里开始读取需要的字符串
						data_num = 0;
						I2DAT = data_store[data_num];   
						data_num++;

            AA = 1;
            break;
        
        case 0xB8:
            //P3 = 0xB8;

            I2DAT = data_store[data_num];
            data_num++;
            AA = 1;
            break;

        case 0xC0:
						data_num = 0;													//为下一次数据做准备
            AA = 1;

            break; 

        case 0xC8:
            //P3 = 0xC8;

            AA = 1;
            break;        
    }

    SI = 0;
    
    while(SI != 0)
    {
        if(I2STAT == 0x00)
        {
            set_STO;
        }
        SI = 0;
        if(SI != 0)
        {
            clr_I2CEN;
            set_I2CEN;
            clr_SI;
            //clr_I2CEN
        } 
    }
    
    while(STO);
}

//========================================================================================================
void Init_I2C(void)
{
		P13_OpenDrain_Mode;
		P14_OpenDrain_Mode;	

	
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
		memset(data_received,0x00,sizeof(data_received));				//清零，防止后面处理数据时拿取的数据出错
		memset(data_store,0x00,sizeof(data_store));

    Init_I2C();                                 //initial I2C circuit	
	
    while (1)
		{

				if(I2STAT == 0xA0)//if( data_num == 7 )
				{			
#if 0
								if( data_received[0]!=0x5A )
									break; 																					//判断数据是否正确

								if( data_received[1]!=0x01 )
									break;
								
#endif
								
						for(	i=0;i<25;i++	)
						{
										data_store[i] 	= data_received[i+2];											//第一组数据
						}																	
						flag = 1;

				}
				
				if( flag == 1 )                 //处理数据
				{
						for( i=0 ; i< 8 ;i++ )
						{
								ring_set_color_all_same(data_store[3*i],data_store[3*i+1],data_store[3*i+2]);
								
						}
						
						
						Timer0_Delay1ms(data_store[24]);
						
				}

		}

}