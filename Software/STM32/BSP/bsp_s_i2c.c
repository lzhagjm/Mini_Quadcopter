#include "bsp_s_i2c.h"
/*******************模拟硬件函数******************/
//延时
static void S_I2C_Delay(void)
{
	Delay_us(2);
}

//停止信号
static void S_I2C_Stop(void)
{
	//当SCL高电平时，SDA出现上升沿表示I2C总线停止信号
	SDA_0();
	SCL_1();
	S_I2C_Delay();
	SDA_1();
}
//产生一个Ack信号
static void S_I2C_Ack(void)
{
	SDA_0();
	S_I2C_Delay();
	SCL_1();
	S_I2C_Delay();
	SCL_0();
	S_I2C_Delay();
	SDA_1();
}
//产生一个NAck信号
static void S_I2C_NAck(void)
{
	SDA_1();
	S_I2C_Delay();
	SCL_1();
	S_I2C_Delay();
	SCL_0();
	S_I2C_Delay();
}


//启动信号
static void S_I2C_Start(void)
{
	//当SCL高电平时,SDA出现一个下降沿表示I2C总线启动信号
	SDA_1();
	SCL_1();
	S_I2C_Delay();
	SDA_0();
	S_I2C_Delay();
	SCL_0();
	S_I2C_Delay();
}
//读取8位数据
static uint8_t S_I2C_ReadByte(uint8_t ack)
{
	uint8_t i,value;
	//读到第1个bit为数据的bit7
	value = 0;
	for(i = 0;i < 8;i++)
	{
		value <<= 1;
		SCL_1();
		S_I2C_Delay();
		if(SDA_In())
		{
			value++;
		}
		SCL_0();
		S_I2C_Delay();
	}
	if(ack==0)
		S_I2C_NAck();
	else
		S_I2C_Ack();
	return value;
}

//发送数据
static void S_I2C_SendByte(uint8_t data)
{
	uint8_t i;
	for(i = 0 ; i < 8 ; i++)
	{
		if(data & 0x80)
		{
			SDA_1();
		}else
		{
			SDA_0();
		}
		S_I2C_Delay();
		SCL_1();
		S_I2C_Delay();
		SCL_0();
		if(i == 7)
		{
			SDA_1();//释放总线
		}
		data <<= 1;
		S_I2C_Delay();
	}
}

//读取ACK应答信号
static uint8_t S_I2C_WaitAck(void)
{
	uint8_t re;
	SDA_1();
	S_I2C_Delay();
	SCL_1();
	S_I2C_Delay();
	if(SDA_In())
	{
		re = 1;
	}else
	{
		re = 0;
	}
	SCL_0();
	S_I2C_Delay();
	return re;
}

/*******************bsp接口函数******************/

//定义GPIO
void S_I2C_Init(void)
{
	//定义GPIO结构体
	GPIO_InitTypeDef GPIO_InitSteruct;
	//打开时钟
	RCC_AHB1PeriphClockCmd(S_I2C_SCL_RCC_PORT|S_I2C_SDA_RCC_PORT,ENABLE);
	//配置结构体
	GPIO_InitSteruct.GPIO_Mode = GPIO_Mode_OUT;  //开漏输出
	GPIO_InitSteruct.GPIO_Pin = S_I2C_SDA_Pin;
	GPIO_InitSteruct.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_InitSteruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitSteruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//启用结构体
	GPIO_Init(S_I2C_SDA_PORT,&GPIO_InitSteruct);
	GPIO_InitSteruct.GPIO_Pin = S_I2C_SCL_Pin;
	GPIO_Init(S_I2C_SCL_PORT,&GPIO_InitSteruct);
	//给一个停止信号，复位I2C总线上的所有设备到待机模式
	S_I2C_Stop();
}

u8 S_I2C_Write_Byte(u8 addr,u8 cmd,u8 data) 				 
{ 
  S_I2C_Start(); 
	S_I2C_SendByte((addr<<1)|0);//发送器件地址+写命令	
	if(S_I2C_WaitAck())	//等待应答
	{
		S_I2C_Stop();		 
		return 1;		
	}
    S_I2C_SendByte(cmd);	//写寄存器地址
    S_I2C_WaitAck();		//等待应答 
	S_I2C_SendByte(data);//发送数据
	if(S_I2C_WaitAck())	//等待ACK
	{
		S_I2C_Stop();	 
		return 1;		 
	}		 
    S_I2C_Stop();	 
	return 0;
}
//IIC读一个字节 
//reg:寄存器地址 
//返回值:读到的数据
u8 S_I2C_Read_Byte(u8 addr,u8 cmd)
{
	u8 res;
  S_I2C_Start(); 
	S_I2C_SendByte((addr<<1)|0);//发送器件地址+写命令	
	S_I2C_WaitAck();		//等待应答 
  S_I2C_SendByte(cmd);	//写寄存器地址
  S_I2C_WaitAck();		//等待应答
  S_I2C_Start();
	S_I2C_SendByte((addr<<1)|1);//发送器件地址+读命令	
  S_I2C_WaitAck();		//等待应答 
	res=S_I2C_ReadByte(0);//读取数据,发送nACK 
  S_I2C_Stop();			//产生一个停止条件 
	return res;		
}


//发送多个字节数据
S_IIC_DEBUG S_I2C_Write(uint8_t Addr,uint8_t cmd,uint8_t *data,uint8_t size)
{
	uint16_t i,m;
	for(i = 0 ; i < size ; i++)
	{
		if(i == 0)
		{
			S_I2C_Stop();
			for(m = 0 ; m < 1000 ; m++)
			{
				S_I2C_Start();
				S_I2C_SendByte((Addr<<1) | S_I2C_WR);
				if(S_I2C_WaitAck() == 0)
				{
					break;
				}
			}
			if(m == 1000)
			{
				S_I2C_Stop();
				return S_IIC_ERROR;
			}
			if(cmd)
			{
				S_I2C_SendByte((uint8_t)cmd);
				if(S_I2C_WaitAck() != 0)
				{
					S_I2C_Stop();
					return S_IIC_ERROR;
				}
			}
		}
		S_I2C_SendByte(data[i]);
		if(S_I2C_WaitAck() != 0)
		{
			S_I2C_Stop();
			return S_IIC_ERROR;
		}		
	}
	S_I2C_Stop();
	return S_IIC_OK;
}

//读取多个数据
S_IIC_DEBUG S_I2C_Read(uint8_t Addr,uint8_t cmd,uint8_t *data,uint8_t size)
{
	uint16_t i;
	S_I2C_Start();//开始信号
	S_I2C_SendByte((Addr<<1)|S_I2C_WR);//发送控制指令
	//等待ACK
	if(S_I2C_WaitAck() != 0)
	{
		S_I2C_Stop();
		return S_IIC_ERROR;
	}
	if(cmd)
	{
			//发送字节地址
			S_I2C_SendByte((uint8_t)cmd);
			//等待ACK
			if(S_I2C_WaitAck() != 0)
			{
				S_I2C_Stop();
				return S_IIC_ERROR;
			}
	}
	S_I2C_Start();
	S_I2C_SendByte((Addr<<1)|S_I2C_RD);
	if(S_I2C_WaitAck() != 0)
	{
		S_I2C_Stop();
		return S_IIC_ERROR;
	}
	//获取数据
	for(i=0;i < (size - 1);i++){
		*data = S_I2C_ReadByte(1);
		data++;
	}
	*data=S_I2C_ReadByte(0);
	S_I2C_Stop();	
	return S_IIC_OK;
}

