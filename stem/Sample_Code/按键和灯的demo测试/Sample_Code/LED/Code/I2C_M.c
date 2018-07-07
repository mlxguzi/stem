
#include "N76E003.h"
#include "WS281X.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "intrins.h"
#include "WS281X.h"
#include <string.h>
#include "key.h"

/* Private variables ---------------------------------------------------------*/
#define TH0_INIT        10000 
#define TL0_INIT        10000					//定时器

U8 rgb8[3]={0x01,0x01,0x49};
U8 rgb9[3]={0x55,0x65,0x49};
#define I2C_CLOCK               13									//iic传输速率
#define EEPROM_SLA              0x40								//从机地址	

U8 xdata data_received[64], data_num = 0,num = 0,data_flag = 0,ll = 0;
U8 xdata data_store[64];
U8 xdata data_read[64];

U8 i=0,j=0,flag=0,begin_flag = 0;
U8 u8TH0_Tmp,u8TL0_Tmp;
	
////////////////////////////按键
U8 timer_10ms_flag = 0;
U8 key1;
///////////////////////////
U8 TypeID = 0xaa;

void I2C_ISR(void) interrupt 6
{
    switch (I2STAT)
    {
        case 0x00:
            STO = 1;
            break;

        case 0x60:
						data_num = 0;

						memset(data_received,0x00,sizeof(data_received));
						data_flag = 1;
            AA = 1;

            break;
				
        case 0x68:
						P02 = 0;
            while(1);
            break;

        case 0x80:																				//接收数据
            data_received[data_num] = I2DAT;
            data_num++;

            AA = 1;
            break;

        case 0x88:																				//错误时归零
            data_received[data_num] = I2DAT;
            data_num = 0;

            AA = 1;
            break;
					
        case 0xA0:
/*******************************************************************************
主机发送停止位，程序每次都会执行但是有时候判断不到，所以改为判断数据值
*******************************************************************************/
						
						begin_flag = 1;	
				
            AA = 1;
						
            break;

        case 0xA8:												
						ll = 0;

						I2DAT = data_read[ll];   
						ll++;
            AA = 1;
					
            break;
        
        case 0xB8:
				
						I2DAT = data_read[ll];
            ll++;
            AA = 1;
            break;

        case 0xC0:							
						data_num = 0;													//为下一次数据做准备
						
						memset(data_received,0x00,sizeof(data_received));	
						memset(data_read,0x00,sizeof(data_read));				
						i = 0;
						ll = 0;
            AA = 1;
//事件被读取后EventFlag置0
									data_read[0] = TypeID;
									data_read[1] = 0x00;
            break; 

        case 0xC8:
						data_num = 0;													//为下一次数据做准备
				
						memset(data_received,0x00,sizeof(data_received));	
//						memset(data_read,0x00,sizeof(data_read));				
						i = 0;
						ll = 0;
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
        } 
    }   
    while(STO);
}
void Timer0_ISR (void) interrupt 1  //interrupt address is 0x000B
{
    TH0 = u8TH0_Tmp;
    TL0 = u8TL0_Tmp;    
		j++;
		timer_10ms_flag |= key_flag;
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
		j = 0;
		while( a != j );
		j = 0;
}
void main(void)
{
		
    Set_All_GPIO_Quasi_Mode;
		P11_Quasi_Mode;
		P11 = 1;	
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
		memset(data_read,0x00,sizeof(data_read));
	
		Init_I2C();
		set_TR0;
	
		while (1)
		{
//按键			
				if(timer_10ms_flag&key_flag == 1)
				{
						timer_10ms_flag &= clear_key_flag;						
						key1 = key_read();
					
						switch(key1)
						{
								case key_click:
									ring_display_clear();
									time_d(5);								
									WS_frame_sync(rgb8);

									data_read[0] = TypeID;
									data_read[1] = 0x00;
									data_read[2] = 0x01;   //灯被点亮
									memcpy(&(data_read+3),rgb8,3);
								
									break;
								case key_double:
									ring_display_clear();
								
									data_read[0] = TypeID;
									data_read[1] = 0x00;
									data_read[2] = 0x01;   //灯被点亮
									memcpy(&(data_read+3),rgb9,3);
									break;
								case key_long:
									
									break;
						}
				}


#if 1	

				if( begin_flag == 1)//data_num == 28)//(I2STAT == 0xA0) && (data_flag==1) )												//主机发送数据结束标志
				{	
												
								for( num=0;num<=data_num;num++ )
								{
										data_store[i] = data_received[num+1];	
										i++;
								}
								
								if(data_received[0]==0x30)
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
//						while(!data_flag)
//						{				
																
							//		if(data_flag!=1)
							//		{			
												time_d(500);
												WS_frame_sync(&(data_store+1));
												time_d(500);												
												
							//		}																					
								//				ring_display_clear();			
//						}	
//						ring_display_clear();	
						memset(data_received,0x00,sizeof(data_received));					
						memset(data_store,0x00,sizeof(data_store));					
												i = 0;
												flag=0;		
										
				}

#endif				
				
#endif
		}
}


/************************ (C) COPYRIGHT OAZON ******************END OF FILE****/


