#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "intrins.h"

#define uchar unsigned char
#define iMAX_LEN 21684
#define iMIN_LEN 109
#define DOUBLE_CRYSTAL_FREQ  16
sbit RX =P1^1; 
sbit TX =P1^2; 

void InitUltrasonicDistance(void);
unsigned int getDistance(void);
unsigned int getDistanceState(void);
unsigned char refreshDistance(void);
void StartModule();
	
unsigned int miDistance=0; 
uchar mcDistanceErr=0; 

bit mbDelayOverFlg = 1;	
uchar mbDelay10H, mbDelay10L; 

void InitUltrasonicDistance(void)
{
	unsigned int iTmp;

  TMOD |=0x01;
	TH0=0;
	TL0=0; 
	TR0=0; 
	ET0=1;
	EA=1; 
	iTmp = (unsigned int)(65536-(7500/(12/DOUBLE_CRYSTAL_FREQ)));
	mbDelay10H = iTmp >> 8; 
	mbDelay10L = iTmp &0x0F; 
}
void tim0_()interrupt 1 
{	
	TR0 = 0; 
	mbDelayOverFlg = 1; 
}
unsigned int getDistance(void)
{
	return miDistance;
}
unsigned int getDistanceState(void)
{
	return mcDistanceErr;
}
unsigned char refreshDistance(void)
{
	unsigned int i; 
	unsigned int iCycle; 

	if (1 == mbDelayOverFlg)
	{	
		i = iMAX_LEN; 
		StartModule(); 
		while(!RX && i-->0);
		if (i>0) 
		{
			TR0=1; 
			while(RX);
			TR0=0; 
			iCycle = (TH0 * 256 + TL0) + 1;
			Send_Data_To_UART0(iCycle);			
			TH0=0;
			TL0=0;	
			if (iCycle <= iMIN_LEN)
			{
				mcDistanceErr = 1;
				mbDelayOverFlg = 0; 
				TH0 = mbDelay10H; 
				TL0 = mbDelay10L; 
				TR0 = 1; 
			}
			else
			{
				miDistance = (unsigned int)(iCycle * 0.01844670);
				mcDistanceErr = 0;
				mbDelayOverFlg = 0; 
				TH0 = 0; 
				TL0 = 0; 
				TR0=1;
			}
		}
		else 
			mcDistanceErr = 2; 
		return 0; 
	}
	else 
		return 1;
}

void main(void)
{
  char pcOutBuf[30];
	unsigned int ild = 0;
	InitUltrasonicDistance();
	Set_All_GPIO_Quasi_Mode;
	InitialUART0_Timer3(115200);	
	while(1)
	{
			if( 0==refreshDistance() )
			{
					if( 0==getDistanceState() && (ild != getDistance()) )
					{
								ild = getDistance();
//								Send_Data_To_UART0();
					}
			}

	}
}




















void StartModule()
{
	TX = 1;
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();_nop_();
	TX=0;
}


































/*#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "intrins.h"

#define uint unsigned int 
#define	uchar unsigned char
	
sbit Tx = P1^0;
sbit Rx = P3^0;

bit TimeUp = 0;
long Th0,Tl0;
unsigned long time0 = 0;
uint Measureresult = 0;
void delay_20us();
uint Measure(void)
{
		char De120us = 0;
		char RxBack  = 1;
#if 1	
		TMOD 	= 0x01 ;
		Tx 		= 0;
		Th0 	= 0;
		Tl0 	= 0;
		TimeUp= 0;
		EA 		= 1;
		ET0 	= 1;
	
		TR0 = 0;
		TH0 = 0;
		TL0 = 0;
#endif	
		Tx = 1;
		delay_20us();
		Tx = 0;
	
		while(Rx==0);
		TH0 = 0;
		TL0 = 0;
		TR0 = 1;	
		while(Rx==1);
#if 1		
		while(RxBack)
		{
				if(Rx==0)//|| TimeUp==1)
				{
						TR0 = 0;
						Th0 = TH0;
						Tl0 = TL0;
						TR0 = 1;
						RxBack =0;
				}
		}
#endif
		while(!TimeUp);
		
		Send_Data_To_UART0(Th0);
		Send_Data_To_UART0(Tl0);
		time0 = Th0*256+Tl0;
		Measureresult = ((unsigned long)344*time0)/2000;
		
		if(Measureresult<30||Measureresult>900)
		{
			Measureresult = 0;
		}
				return Measureresult;
}

void T0_time()interrupt 1
{
		TH0 = 0;
		TL0 = 0;
		TimeUp = 1;

}


void main()
{
	uchar flag = 0;
	uint j = 0;
	uint ge,shi,bai;
	Set_All_GPIO_Quasi_Mode;
	P30_Quasi_Mode;
	P10_PushPull_Mode;
//	P10_Quasi_Mode;

	InitialUART0_Timer3(115200);
	while(1)
	{
				ge 	= Measure()%100%10;
				shi = Measure()%100/10;
				ge 	= Measure()/100;
		Measure();
//				Send_Data_To_UART0(ge);
//				Send_Data_To_UART0(shi);
//				Send_Data_To_UART0(bai);
		Timer1_Delay10ms(8);		
	}
}

void delay_20us()
{
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();



}*/