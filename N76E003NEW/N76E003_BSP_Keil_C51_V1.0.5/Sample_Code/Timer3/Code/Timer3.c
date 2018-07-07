/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2017 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

//***********************************************************************************************************
//  Nuvoton Technoledge Corp. 
//  Website: http://www.nuvoton.com
//  E-Mail : MicroC-8bit@nuvoton.com
//  Date   : Apr/21/2017
//***********************************************************************************************************

//***********************************************************************************************************
//  File Function: N76E003 Timer3 demo code
//***********************************
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"

#define RELOAD_VALUE_H  (65536-1500)/256
#define RELOAD_VALUE_L  (65536-1500)%256

/************************************************************************************************************
*    TIMER 2 interrupt subroutine
************************************************************************************************************/
UINT8 a = 0;
void Timer3_ISR (void) interrupt 16 
{
    clr_TF3;
 P12 = ~P12 ;                                //P0.2 toggle when TF3 interrupt
	
	printf("\n TM3 INT 0x%bX", RH3);
}

/************************************************************************************************************
*    Main function 
************************************************************************************************************/
void main (void)
{
    
    Set_All_GPIO_Quasi_Mode;
		InitialUART0_Timer1(115200);

       
    RH3 = RELOAD_VALUE_H;                       //initial counter values 
    RL3 = RELOAD_VALUE_L;    
    
    set_ET3;                                    //enable Timer3 interrupt
    set_EA;                                     //enable interrupts
    set_TR3;                                    //Timer3 run

    while(1);
}

