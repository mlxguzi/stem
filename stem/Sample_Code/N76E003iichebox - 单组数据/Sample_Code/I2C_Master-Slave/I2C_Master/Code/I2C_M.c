/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2017 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

//***********************************************************************************************************
//  Website: http://www.nuvoton.com
//  E-Mail : MicroC-8bit@nuvoton.com
//  Date   : Jan/21/2017
//***********************************************************************************************************

//***********************************************************************************************************
//  File Function: N76E003 I2C master mode demo code, the Slave address = 0xA4
//
//   ____________            _____________ 
//  |            |   SDA    |             |
//  |            |<-------->|             |
//  |            |          |             |
//  |N76E003(M)  |          | N76E003(S)  |
//  |(I2C_Master)|          | (I2C_Slave) |
//  |            |   SCL    |             |
//  |            |--------->|             |
//  |____________|          |_____________|
//
//  The protocol of I2C is same the "24LC64"
//***********************************************************************************************************
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"

//***********************************************************************************************************
#define I2C_CLOCK               13
#define EEPROM_SLA              0x4A
#define EEPROM_WR               0
#define EEPROM_RD               1
#define ERROR_CODE              0x78
#define PAGE_SIZE               10

UINT8 data_slave[11] = {0x01,0x11,0x02,0xAA,0x03,0x51,0x04,0x81,0x82,0x05,0xf1};
UINT8 data_1[3] = {0x51,0x10,0xaa};
UINT8 i = 0;

//========================================================================================================
void Init_I2C(void)
{
//    /* Set I2C clock rate */
    I2CLK = I2C_CLOCK; 

    /* Enable I2C */
    set_I2CEN;                                   
}
//========================================================================================================
void I2C_Error(void)
{
//    P3 = I2STAT;
//    P3 = ERROR_CODE;
    while (1);    
}

bit Wait_SI(void)
{
    uint32_t count = 0;
    while(!SI)
    {
        if((I2STAT==0x00))
        {
            set_STO;
            return 0;
        }
        count++;
        
        if(count > 100000)
        {
            set_STO;
            return 0;
        }	
    }
    
    return 1;
}

bit Wait_STO(void)
{
    uint32_t count = 0;
    
    while(STO)
    {
        count++;
        
        if(count > 100000)
        {
            return 0;
        }
    }
    
    return 1;
}


//========================================================================================================
void I2C_Process(UINT8 u8DAT)
{
    UINT32 u32Count;

//--------------------------------------------------------------------------------------------
//----  Page Write----------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
    /* Step1 */
    set_STA;                                    /* Send Start bit to I2C EEPROM */
    clr_SI;
    //while (!SI);                                //Check SI set or not
    if(Wait_SI() == 0)
    {
        return;
    }
    if (I2STAT != 0x08)                         //Check status value after every step
        return;
    
    /* Step2 */
    clr_STA;                                    //STA=0
    I2DAT = (EEPROM_SLA | EEPROM_WR);
    clr_SI;
    //while (!SI);                                //Check SI set or not
    if(Wait_SI() == 0)
    {
        return;
    }
    if (I2STAT != 0x18)              
        return;

//////////////±Í÷æ
    /* Step3 */
    I2DAT = 0x5A;                               //address high for I2C EEPROM
    clr_SI;
    //while (!SI);                                //Check SI set or not
    if(Wait_SI() == 0)
    {
        return;
    }
    if (I2STAT != 0x28)              
        return;
                    
    /* Step4 */
    I2DAT = 0x01;                               //address low for I2C EEPROM
    clr_SI;
    //while (!SI);                                //Check SI set or not
    if(Wait_SI() == 0)
    {
        return;
    }
    if (I2STAT != 0x28)              
        return;
		
/////////////////////////////
		for(u32Count = 0; u32Count <8; u32Count++)
		{
//				for( i=0 ;i<8 ;i++ )
				
		
		    I2DAT = data_1[0]+i*10;
        clr_SI;
        //while (!SI);                                //Check SI set or not
        if(Wait_SI() == 0)
        {
            return;
        }
        if (I2STAT != 0x28)              
            return;
				
				I2DAT = data_1[1]+i*10;
        clr_SI;
        //while (!SI);                                //Check SI set or not
        if(Wait_SI() == 0)
        {
            return;
        }
        if (I2STAT != 0x28)              
            return;

				I2DAT = data_1[2]+i*10;
        clr_SI;
        //while (!SI);                                //Check SI set or not
        if(Wait_SI() == 0)
        {
            return;
        }
        if (I2STAT != 0x28)              
            return;
		
		}
			  I2DAT = 0x80;
        clr_SI;
        //while (!SI);                                //Check SI set or not
        if(Wait_SI() == 0)
        {
            return;
        }
        if (I2STAT != 0x28)              
            return;
/////////////////////

//--------------------------------------------------------------------------------------------
//----  Waitting the ready for I2C write------------------------------------------------------
//--------------------------------------------------------------------------------------------
    /* Step6 */
    do{
        set_STO;
        clr_SI;
        
        set_STA;                                //Check if no ACK is returned by EEPROM, it is under timed-write cycle
        clr_SI;
        //while (!SI);                                //Check SI set or not
        if(Wait_SI() == 0)
        {
            return;
        }
        if (I2STAT != 0x08)                     //Check status value after every step
            return;

        clr_STA;
        I2DAT = (EEPROM_SLA | EEPROM_WR);
        clr_SI;
        //while (!SI);                                //Check SI set or not
        if(Wait_SI() == 0)
        {
            return;
        }
    }while (I2STAT != 0x18);
    
    /* Step7 */
    set_STO;
    clr_SI;
    //while (STO);                                /* Check STOP signal */
    if(Wait_STO() == 0)
    {
        return;
    }
//--------------------------------------------------------------------------------------------
//----  Page Read ----------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------

#if 1
    /* Step12 */
    /* Repeated START */
    set_STA;                       
    clr_SI;
    //while (!SI);                                //Check SI set or not
    if(Wait_SI() == 0)
    {
        return;
    }
    if (I2STAT != 0x08)                         //Check status value after every step
        return;
    
    /* Step13 */
    clr_STA;                                    //STA needs to be cleared after START codition is generated
    I2DAT = (EEPROM_SLA | EEPROM_RD);
    clr_SI;
    //while (!SI);                                //Check SI set or not
    if(Wait_SI() == 0)
    {
        return;
    }
    if (I2STAT != 0x40)              
        return;
    
    /* Step14 */
#if 1 
		for (u32Count = 0; u32Count <PAGE_SIZE; u32Count++)
    {
        set_AA;
        clr_SI;        
        //while (!SI);                                //Check SI set or not
        if(Wait_SI() == 0)
        {
            return;
        }

        if (I2STAT != 0x50)              
            return;
        
				if (I2DAT != data_slave[u32Count])  {}           
 //           return;
				
//       u8DAT = ~u8DAT; 
    } 
 #endif  
    /* Step15 */
    clr_AA;
    clr_SI;
    //while (!SI);                                //Check SI set or not
    if(Wait_SI() == 0)
    {
        return;
    }
    if (I2STAT != 0x58)              
        return;

    /* Step16 */
    set_STO;
    clr_SI;
    //while (STO);                                /* Check STOP signal */
    if(Wait_STO() == 0)
    {
        return;
    }
    
    printf("OK \n");
#endif
}
//========================================================================================================
void main(void)
{    
    Set_All_GPIO_Quasi_Mode;	
    InitialUART0_Timer1(115200); 
    Init_I2C();                                 //initial I2C circuit

	
    while(1)
    {

				I2C_Process(0x55);                          /* I2C Master will send 0x55,0xAA,.... to slave */
        clr_I2CEN; 
        set_I2CEN; 
        Timer0_Delay1ms(300);
    }
}

