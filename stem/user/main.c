#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "TIME01.h"
#include "PWM1.h"
#include "I2C_M.h"
#include "ADC1.h"

////////////////////////ADC
#define ADCstart  Enable_ADC_AIN3;clr_ADCF;set_ADCS;

#define uchar unsigned char //????????
#define uint  unsigned int
#define ulong unsigned long
//***********************************************
sfr   CLK_DIV = 0x97; //?STC?????,??????
                     //?STC????IO???????

//sfr   P0M1   = 0X93;
sfr   P0M0   = 0X94;
//sfr   P1M1   = 0X91;
sfr   P1M0   = 0X92;
sfr	  P2M1   = 0X95;
sfr	  P2M0   = 0X96;

//***********************************************
sbit Trig  = P1^0; //??????
sbit Echo  = P3^2; //????
sbit test  = P1^1; //?????

uchar code SEG7[10]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};//???0-9
uint distance[4];  //???????
uchar ge,shi,bai,temp,flag,outcomeH,outcomeL,i;  //??????
bit succeed_flag;  //??????
//********????
void conversion(uint temp_data);
void delay_20us();
//void pai_xu();

void main(void)   // ???
{  
		uint distance_data,a,b;
		uchar CONT_1;   
		CLK_DIV=0X03; //?????1/8??(pdf-45?) 
     P0M1 = 0;   //?io????????
     P1M1 = 0;
     P2M1 = 0;
     P0M0 = 0XFF;
     P1M0 = 0XFF;
     P2M0 = 0XFF;
   i=0;
   flag=0;
	test =0;
	Trig=0;       //??????????
	TMOD=0x11;    //???0,???1,16?????
	TR0=1;	     //?????0
   IT0=0;        //????????,??????
	ET0=1;        //?????0??
 //ET1=1;        //?????1??
	EX0=0;        //??????
	EA=1;         //?????0	
  
	
while(1)         //????
	{
		EA=0;
	    Trig=1;
        delay_20us();
        Trig=0;         //????20us???,?Trig??  
        while(Echo==0); //??Echo????????
	     succeed_flag=0; //???????
	     EX0=1;          //??????
	 	  TH1=0;          //???1??
        TL1=0;          //???1??
	     TF1=0;          //
        TR1=1;          //?????1
		EA=1;

      while(TH1 < 30);//???????,??65.535??(??????)  
		  TR1=0;          //?????1
        EX0=0;          //??????

    if(succeed_flag==1)
	     { 	
		   distance_data=outcomeH;                //??????8?
           distance_data<<=8;                   //??16???8?
		     distance_data=distance_data|outcomeL;//??8?????16?????
            distance_data*=12;                  //????????12??
           distance_data/=58;                   //???????58????
         }                                      //?????58????,  Y?=(X?*344)/2
			                                       // X?=( 2*Y?)/344 ==»X?=0.0058*Y? ==»??=??/58 
    if(succeed_flag==0)
		   {
            distance_data=0;                    //???????
		   	test = !test;                       //?????
           }

     ///       distance[i]=distance_data; //?????????????
     ///        i++;
  	  ///	 if(i==3)
	  ///	     {
	  ///	       distance_data=(distance[0]+distance[1]+distance[2]+distance[3])/4;
     ///        pai_xu();
     ///        distance_data=distance[1];

      
	   a=distance_data;
       if(b==a) CONT_1=0;
       if(b!=a) CONT_1++;
       if(CONT_1>=3)
		   { CONT_1=0;
			  b=a;
			  conversion(b);
			}       
	  ///		 i=0;
 	  ///		}	     
	 }
}
//***************************************************************
//????0,????????
INTO_()  interrupt 0   // ?????0?
 {    
     outcomeH =TH1;    //???????
     outcomeL =TL1;    //???????
     succeed_flag=1;   //????????
     EX0=0;            //??????
  }
//****************************************************************
//???0??,????
timer0() interrupt 1  // ???0???1?
   {
 	 TH0=0xfd; //?????0???
	 TL0=0x77;	 	
	 switch(flag)   
      {case 0x00:P0=ge; P2=0xfd;flag++;break;
	    case 0x01:P0=shi;P2=0xfe;flag++;break;
	    case 0x02:P0=bai;P2=0xfb;flag=0;break;
      }
   }
//*****************************************************************
/*
//???1??,?????????
timer1() interrupt 3  // ???0???1?
    {
TH1=0;
TL1=0;
     }
*/
//******************************************************************
//????????
void conversion(uint temp_data)  
 {  
    uchar ge_data,shi_data,bai_data ;
    bai_data=temp_data/100 ;
    temp_data=temp_data%100;   //????
    shi_data=temp_data/10 ;
    temp_data=temp_data%10;   //????
    ge_data=temp_data;

    bai_data=SEG7[bai_data];
    shi_data=SEG7[shi_data];
    ge_data =SEG7[ge_data];

    EA=0;
    bai = bai_data;
    shi = shi_data;
    ge  = ge_data ; 
	 EA=1;
 }
//******************************************************************
void delay_20us()
 {  uchar bt ;
    for(bt=0;bt<100;bt++);
 }