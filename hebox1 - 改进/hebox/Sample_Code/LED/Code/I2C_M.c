
#include "N76E003.h"
#include "WS281X.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "intrins.h"
#include "WS281X.h"
#include "string.h"


/* Private variables ---------------------------------------------------------*/
#define TH0_INIT        1300 
#define TL0_INIT        1300					//定时器


#define I2C_CLOCK               13									//iic传输速率
#define EEPROM_SLA              0x40								//从机地址	

U8 xdata data_received[100], data_num = 0,num = 0,data_flag = 0,ll = 0;
U8 xdata data_store[350];
U8 i=0,j=0,flag=0,begin_flag = 0;
U8 u8TH0_Tmp,u8TL0_Tmp;
	
void I2C_ISR(void) interrupt 6
{
    switch (I2STAT)
    {
#if 0
        case 0x00:
            STO = 1;
            break;
#endif
        case 0x60:
						data_num = 0;

						memset(data_received,0x00,sizeof(data_received));
						data_flag = 1;
            AA = 1;

            break;
#if 0				
        case 0x68:
						P02 = 0;
            while(1);
            break;
#endif
        case 0x80:																				//接收数据
            data_received[data_num] = I2DAT;
            data_num++;
//						if(data_received[27]==0x10)
//							clr_GPIO1;

            AA = 1;
            break;
#if 0
        case 0x88:																				//错误时归零
            data_received[data_num] = I2DAT;
            data_num = 0;
						clr_GPIO1;
            AA = 1;
            break;
#endif					
        case 0xA0:
/*******************************************************************************
主机发送停止位，程序每次都会执行但是有时候判断不到，所以改为判断数据值
*******************************************************************************/
						
						begin_flag = 1;	
            AA = 1;
						
            break;

        case 0xA8:												
						ll = 0;
//					I2DAT =	data_received[data_num];
						I2DAT = data_store[ll];   
						ll++;
            AA = 1;
            break;
        
        case 0xB8:
//						I2DAT =	data_received[data_num];					
						I2DAT = data_store[ll];
            ll++;
            AA = 1;
            break;

        case 0xC0:							
						data_num = 0;													//为下一次数据做准备
				
						memset(data_received,0x00,sizeof(data_received));	
						memset(data_store,0x00,sizeof(data_store));				
						i = 0;
						ll = 0;
            AA = 1;

            break; 
#if 0
        case 0xC8:
            AA = 1;
            break;  
#endif				
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
void Timer0_ISR (void) interrupt 1  //interrupt address is 0x000B
{
    TH0 = u8TH0_Tmp;
    TL0 = u8TL0_Tmp;    

		j++;
                    // GPIO1 toggle when interrupt
}
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
void time_d(U8 a)
{
		set_TR0;
		while( a != j );
		clr_TR0;
		j = 0;
}
void main(void)
{
		
    Set_All_GPIO_Quasi_Mode;
#if 1		
		TIMER0_MODE1_ENABLE;	
	  u8TH0_Tmp = (65536-TH0_INIT)/256;
    u8TL0_Tmp = (65536-TL0_INIT)%256; 
	
		TH0 = u8TH0_Tmp;
    TL0 = u8TL0_Tmp;
	
	  set_ET0;                                    //enable Timer0 interrupt
    set_EA;                                     //enable interrupts
#endif	
		memset(data_received,0x00,sizeof(data_received));				//清零，防止后面处理数据时拿取的数据出错
		memset(data_store,0x00,sizeof(data_store));
		
		
		Init_I2C();
//		set_TR0;

		while (1)
		{
#if 1	

				if( begin_flag == 1)//data_num == 28)//(I2STAT == 0xA0) && (data_flag==1) )												//主机发送数据结束标志
				{	
								
								
								for( num=0;num<22;num++ )
								{
										data_store[i] = data_received[num+3];	
										i++;
								}
								if(data_received[2]>0x80)
								{
									flag = 1;
									clr_GPIO1;
								}

								data_num = 0;			
								begin_flag = 0;
				}					
				
#if 1			
	
				if( flag == 1 )                 					
				{					
						data_flag = 0;	
					
//效果灯期间有新数据发送过来则退出	
						while(!data_flag)
						{														
//处理数据			
									for(i=0;i<(data_received[2]-0x80);i++)
									{		
//多加一个data_flag判断，否则每次跳出会出现灯乱闪										
										if(data_flag!=1)
										{																								
												WS_frame_asyn(&(data_store+i*21+i));
												time_d(data_store[(i+1)*21+i]);
										}											
									}						
															
						}	
												ring_display_clear();	
//清空数据，为下次数据做准备
												memset(data_store,0x00,sizeof(data_store));
												i = 0;
												flag=0;		
										
				}

#endif				
				
#endif
		}
}


/************************ (C) COPYRIGHT OAZON ******************END OF FILE****/


