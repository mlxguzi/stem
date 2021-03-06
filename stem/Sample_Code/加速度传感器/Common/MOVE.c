#include "buzzer.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "N76E003.h"
#include "Common.h"
#include "MOVE.h"
#include "intrins.h"

//**************************************************************************************************
//定义类型及变量
//**************************************************************************************************
uchar dis[6];					//显示数字(-511至512)的字符数组
int	dis_data;					//变量


//********************************************************************************
//整数转字符串
//********************************************************************************
void lcd_printf(uchar *s,int temp_data)
{
	if(temp_data<0)
	{
		temp_data=-temp_data;
		*s='-';
	}
	else *s=' ';

	*++s =temp_data/10000+0x30;
	temp_data=temp_data%10000;     //取余运算

	*++s =temp_data/1000+0x30;
	temp_data=temp_data%1000;     //取余运算

	*++s =temp_data/100+0x30;
	temp_data=temp_data%100;     //取余运算
	*++s =temp_data/10+0x30;
	temp_data=temp_data%10;      //取余运算
	*++s =temp_data+0x30; 	
}

//******************************************************************************************************
//串口初始化
//*******************************************************************************************************
void init_uart()
{
	TMOD=0x21;				
//	TH1=0xfd;		//实现波特率9600（系统时钟11.0592MHZ）		
InitialUART0_Timer1(9600);
	TL1=0xfd;		
		
	SCON=0x50;
	PS=1;      //串口中断设为高优先级别
	TR0=1;	   //启动定时器			
	TR1=1;
	ET0=1;     //打开定时器0中断			
	ES=1;	
	EA=1;
}

//*************************************************************************************************
//串口发送函数
//*************************************************************************************************
void  SeriPushSend(uchar send_data)
{
/////////////	
	TI = 0;
//////////////
    SBUF=send_data;  
	while(!TI);TI=0;	  
}
//*************************************************************************************************
//************************************延时*********************************************************
//*************************************************************************************************
void delay(unsigned int k)	
{						
	unsigned int i,j;				
	for(i=0;i<k;i++)
	{			
		for(j=0;j<121;j++);
	}						
}

void Delay5us()
{
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();

}
//*************************************************************************************************
//I2C起始信号
//*************************************************************************************************
void I2C_Start()
{
    SDA = 1;                    //拉高数据线
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    SDA = 0;                    //产生下降沿
    Delay5us();                 //延时
    SCL = 0;                    //拉低时钟线
}
//*************************************************************************************************
//I2C停止信号
//*************************************************************************************************
void I2C_Stop()
{
    SDA = 0;                    //拉低数据线
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    SDA = 1;                    //产生上升沿
    Delay5us();                 //延时
}
//**************************************************************************************************
//I2C发送应答信号
//入口参数:ack (0:ACK 1:NAK)
//**************************************************************************************************
void I2C_SendACK(bit ack)
{
    SDA = ack;                  //写应答信号
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    SCL = 0;                    //拉低时钟线
    Delay5us();                 //延时
}
//****************************************************************************************************
//I2C接收应答信号
//****************************************************************************************************
bit I2C_RecvACK()
{
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    CY = SDA;                   //读应答信号
    SCL = 0;                    //拉低时钟线
    Delay5us();                 //延时
    return CY;
}
//*****************************************************************************************************
//向I2C总线发送一个字节数据
//*****************************************************************************************************
void I2C_SendByte(uchar dat)
{
    uchar i;
    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;              //移出数据的最高位
        SDA = CY;               //送数据口
        SCL = 1;                //拉高时钟线
        Delay5us();             //延时
        SCL = 0;                //拉低时钟线
        Delay5us();             //延时
    }
    I2C_RecvACK();
}
//*****************************************************************************************************
//从I2C总线接收一个字节数据
//******************************************************************************************************
uchar I2C_RecvByte()
{
    uchar i;
    uchar dat = 0;
    SDA = 1;                    //使能内部上拉,准备读取数据,
    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;
        SCL = 1;                //拉高时钟线
        Delay5us();             //延时
        dat |= SDA;             //读数据               
        SCL = 0;                //拉低时钟线
        Delay5us();             //延时
    }
    return dat;
}
//*****************************************************************************************************
//向I2C设备写入一个字节数据
//*****************************************************************************************************
void Single_WriteI2C(uchar REG_Address,uchar REG_data)
{
    I2C_Start();                  //起始信号
    I2C_SendByte(SlaveAddress);   //发送设备地址+写信号
    I2C_SendByte(REG_Address);    //内部寄存器地址，
    I2C_SendByte(REG_data);       //内部寄存器数据，
    I2C_Stop();                   //发送停止信号
}
//*******************************************************************************************************
//从I2C设备读取一个字节数据
//*******************************************************************************************************
uchar Single_ReadI2C(uchar REG_Address)
{
	uchar REG_data;
	I2C_Start();                   //起始信号
	I2C_SendByte(SlaveAddress);    //发送设备地址+写信号
	I2C_SendByte(REG_Address);     //发送存储单元地址，从0开始	
	I2C_Start();                   //起始信号
	I2C_SendByte(SlaveAddress+1);  //发送设备地址+读信号
	REG_data=I2C_RecvByte();       //读出寄存器数据
	I2C_SendACK(1);                //接收应答信号
	I2C_Stop();                    //停止信号
	return REG_data;
}
//******************************************************************************************************
//初始化MPU6050
//******************************************************************************************************
void InitMPU6050()
{
	Single_WriteI2C(PWR_MGMT_1, 0x00);	//解除休眠状态
	Single_WriteI2C(SMPLRT_DIV, 0x07);
	Single_WriteI2C(CONFIG, 0x06);
	Single_WriteI2C(GYRO_CONFIG, 0x18);
	Single_WriteI2C(ACCEL_CONFIG, 0x01);
}
//******************************************************************************************************
//合成数据
//******************************************************************************************************
int GetData(uchar REG_Address)
{
	uchar H,L;
	H=Single_ReadI2C(REG_Address);
	L=Single_ReadI2C(REG_Address+1);
	return ((H<<8)+L);   //合成数据
}
//******************************************************************************************************
//超级终端（串口调试助手）上显示10位数据
//******************************************************************************************************
void Display10BitData(int value)
{  uchar i;
//	value/=64;							//转换为10位数据
	lcd_printf(dis, value);			//转换数据显示
	for(i=0;i<6;i++)
	{
    SeriPushSend(dis[i]);
  }

  // 	DisplayListChar(x,y,dis,4);	//启始列，行，显示数组，显示长度
}








