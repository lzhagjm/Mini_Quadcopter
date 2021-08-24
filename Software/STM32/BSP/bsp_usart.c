#include "bsp_usart.h"

void NoUse(u8 data){}
//串口接收解析函数
#define U1GetOneByte	NoUse
#define U2GetOneByte	NoUse
#define U3GetOneByte	data_analysis
#define U6GetOneByte	k210_data_analysis
	
#if USE_USART1
//====uart1
void usart1_init(u32 br_num)
{
    USART_InitTypeDef USART_InitStructure;
    USART_ClockInitTypeDef USART_ClockInitStruct;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    USART_StructInit(&USART_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //开启USART1时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    //串口中断优先级
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_UART1_P;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_UART1_S;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

    //配置PA9作为USART1　Tx
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //配置PA10作为USART1　Rx
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_DeInit(USART1);
    //配置USART1
    //中断被屏蔽了
    USART_InitStructure.USART_BaudRate = br_num;                                    //波特率可以通过地面站配置
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //8位数据
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //在帧结尾传输1个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                             //禁用奇偶校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流控制失能
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;                 //发送、接收使能
    //配置USART1时钟
   USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;     //时钟低电平活动
   USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;           //SLCK引脚上时钟输出的极性->低电平
   USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;         //时钟第二个边沿进行数据捕获
   USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable; //最后一位数据的时钟脉冲不从SCLK输出

    USART_Init(USART1, &USART_InitStructure);
    USART_ClockInit(USART1, &USART_ClockInitStruct);
		USART_Cmd(USART1, ENABLE);
    //使能USART1接收中断
		USART_ClearFlag(USART1, USART_FLAG_TC);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    //使能USART1
}


//发送函数
u8 Tx1Buffer[256];
u8 Tx1Counter = 0;
u8 count1 = 0;
void DrvUart1SendBuf(unsigned char *DataToSend, u8 data_num)
{
    u8 i;
    for (i = 0; i < data_num; i++)
    {
        Tx1Buffer[count1++] = *(DataToSend + i);
    }

    if (!(USART1->CR1 & USART_CR1_TXEIE))
    {
        USART_ITConfig(USART1, USART_IT_TXE, ENABLE); //打开发送中断
    }
}

//接受处理
u8 U1RxDataTmp[100];
u8 U1RxInCnt = 0;
u8 U1RxoutCnt = 0;
void drvU1GetByte(u8 data)
{
	U1RxDataTmp[U1RxInCnt++] = data;
	if(U1RxInCnt >= 100)
		U1RxInCnt = 0;
}
void drvU1DataCheck(void)
{
	while(U1RxInCnt!=U1RxoutCnt)
	{
		U1GetOneByte(U1RxDataTmp[U1RxoutCnt++]);
		if(U1RxoutCnt >= 100)
			U1RxoutCnt = 0;
	}
}

void USART1_IRQHandler(void)
{
    u8 com_data;

    if (USART1->SR & USART_SR_ORE) //ORE中断
    {
        com_data = USART1->DR;
    }
    //接收中断
    if (USART_GetITStatus(USART1, USART_IT_RXNE))
    {
        USART_ClearITPendingBit(USART1, USART_IT_RXNE); //清除中断标志
        com_data = USART1->DR;
				USART_SendData(USART3, com_data);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET){};
        drvU1GetByte(com_data);
    }
    //发送（进入移位）中断
    if (USART_GetITStatus(USART1, USART_IT_TXE))
    {
        USART1->DR = Tx1Buffer[Tx1Counter++]; //写DR清除中断标志
        if (Tx1Counter == count1)
        {
            USART1->CR1 &= ~USART_CR1_TXEIE; //关闭TXE（发送中断）中断
        }
    }
}
#endif


#if USE_USART6
//===usart6
void usart6_init(u32 br_num)
{
    USART_InitTypeDef USART_InitStructure;
    USART_ClockInitTypeDef USART_ClockInitStruct;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    USART_StructInit(&USART_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE); //开启USART1时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    //串口中断优先级
    NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_UART6_P;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_UART6_S;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);

    //配置PA9作为USART6　Tx
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    //配置PA10作为USART6　Rx
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    USART_DeInit(USART6);
    //配置USART1
    //中断被屏蔽了
    USART_InitStructure.USART_BaudRate = br_num;                                    //波特率可以通过地面站配置
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //8位数据
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //在帧结尾传输1个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                             //禁用奇偶校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流控制失能
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;                 //发送、接收使能
    //配置USART1时钟
   USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;     //时钟低电平活动
   USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;           //SLCK引脚上时钟输出的极性->低电平
   USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;         //时钟第二个边沿进行数据捕获
   USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable; //最后一位数据的时钟脉冲不从SCLK输出

    USART_Init(USART6, &USART_InitStructure);
    USART_ClockInit(USART6, &USART_ClockInitStruct);
		USART_Cmd(USART6, ENABLE);
    //使能USART1接收中断
		USART_ClearFlag(USART6, USART_FLAG_TC);
    USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
    //使能USART1
}


//发送函数
u8 Tx6Buffer[256];
u8 Tx6Counter = 0;
u8 count6 = 0;
void DrvUart6SendBuf(unsigned char *DataToSend, u8 data_num)
{
    u8 i;
    for (i = 0; i < data_num; i++)
    {
        Tx6Buffer[count6++] = *(DataToSend + i);
    }

    if (!(USART6->CR1 & USART_CR1_TXEIE))
    {
        USART_ITConfig(USART6, USART_IT_TXE, ENABLE); //打开发送中断
    }
}

//接受处理
u8 U6RxDataTmp[100];
u8 U6RxInCnt = 0;
u8 U6RxoutCnt = 0;
void drvU6GetByte(u8 data)
{
	U6RxDataTmp[U6RxInCnt++] = data;
	if(U6RxInCnt >= 100)
		U6RxInCnt = 0;
}
void drvU6DataCheck(void)
{
	while(U6RxInCnt!=U6RxoutCnt)
	{
		U6GetOneByte(U6RxDataTmp[U6RxoutCnt++]);
		if(U6RxoutCnt >= 100)
			U6RxoutCnt = 0;
	}
}

void USART6_IRQHandler(void)
{
    u8 com_data;

    if (USART6->SR & USART_SR_ORE) //ORE中断
    {
        com_data = USART6->DR;
    }
    //接收中断
    if (USART_GetITStatus(USART6, USART_IT_RXNE))
    {
        USART_ClearITPendingBit(USART6, USART_IT_RXNE); //清除中断标志
        com_data = USART6->DR;
        drvU6GetByte(com_data);
    }
    //发送（进入移位）中断
    if (USART_GetITStatus(USART6, USART_IT_TXE))
    {
        USART6->DR = Tx6Buffer[Tx6Counter++]; //写DR清除中断标志
        if (Tx6Counter == count6)
        {
            USART6->CR1 &= ~USART_CR1_TXEIE; //关闭TXE（发送中断）中断
        }
    }
}

#endif



#if USE_USART2
//====uart2
void usart2_init(u32 br_num)
{
    USART_InitTypeDef USART_InitStructure;
    USART_ClockInitTypeDef USART_ClockInitStruct;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    USART_StructInit(&USART_InitStructure);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //开启USART2时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    //串口中断优先级
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_UART2_P;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_UART2_S;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);

    //配置PD5作为USART2　Tx
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    //配置PD6作为USART2　Rx
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    //配置USART2
    //中断被屏蔽了
    USART_InitStructure.USART_BaudRate = br_num;                                    //波特率可以通过地面站配置
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //8位数据
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //在帧结尾传输1个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                             //禁用奇偶校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流控制失能
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;                 //发送、接收使能
    //配置USART2时钟
    USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;     //时钟低电平活动
    USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;           //SLCK引脚上时钟输出的极性->低电平
    USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;         //时钟第二个边沿进行数据捕获
    USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable; //最后一位数据的时钟脉冲不从SCLK输出

    USART_Init(USART2, &USART_InitStructure);
    USART_ClockInit(USART2, &USART_ClockInitStruct);

    //使能USART2接收中断
		USART_ClearFlag(USART2, USART_FLAG_TC);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    //使能USART2
    USART_Cmd(USART2, ENABLE);
}

u8 TxBuffer[256];
u8 TxCounter = 0;
u8 count = 0;
void DrvUart2SendBuf(unsigned char *DataToSend, u8 data_num)
{
    u8 i;
    for (i = 0; i < data_num; i++)
    {
        TxBuffer[count++] = *(DataToSend + i);
    }

    if (!(USART2->CR1 & USART_CR1_TXEIE))
    {
        USART_ITConfig(USART2, USART_IT_TXE, ENABLE); //打开发送中断
    }
}
u8 U2RxDataTmp[100];
u8 U2RxInCnt = 0;
u8 U2RxoutCnt = 0;
void drvU2GetByte(u8 data)
{
	U2RxDataTmp[U2RxInCnt++] = data;
	if(U2RxInCnt >= 100)
		U2RxInCnt = 0;
}
void drvU2DataCheck(void)
{
	while(U2RxInCnt!=U2RxoutCnt)
	{
		U2GetOneByte(U2RxDataTmp[U2RxoutCnt++]);
		if(U2RxoutCnt >= 100)
			U2RxoutCnt = 0;
	}
}
void USART2_IRQHandler(void)
{
    u8 com_data;

    if (USART2->SR & USART_SR_ORE) //ORE中断
    {
        com_data = USART2->DR;
    }

    //接收中断
    if (USART_GetITStatus(USART2, USART_IT_RXNE))
    {
        USART_ClearITPendingBit(USART2, USART_IT_RXNE); //清除中断标志
        com_data = USART2->DR;
				drvU2GetByte(com_data);
    }
    //发送（进入移位）中断
    if (USART_GetITStatus(USART2, USART_IT_TXE))
    {
        USART2->DR = TxBuffer[TxCounter++]; //写DR清除中断标志
        if (TxCounter == count)
        {
            USART2->CR1 &= ~USART_CR1_TXEIE; //关闭TXE（发送中断）中断
        }
    }
}
#endif

#if USE_USART3
//====uart3
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void usart3_init(u32 br_num)
{
    USART_InitTypeDef USART_InitStructure;
    USART_ClockInitTypeDef USART_ClockInitStruct;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    USART_StructInit(&USART_InitStructure);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //开启USART2时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    //串口中断优先级
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_UART3_P;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_UART3_S;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

    //配置PD5作为USART2　Tx
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    //配置PD6作为USART2　Rx
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //配置USART2
    //中断被屏蔽了
    USART_InitStructure.USART_BaudRate = br_num;                                    //波特率可以通过地面站配置
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //8位数据
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //在帧结尾传输1个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                             //禁用奇偶校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流控制失能
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;                 //发送、接收使能
    //配置USART2时钟
    USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;     //时钟低电平活动
    USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;           //SLCK引脚上时钟输出的极性->低电平
    USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;         //时钟第二个边沿进行数据捕获
    USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable; //最后一位数据的时钟脉冲不从SCLK输出

    USART_Init(USART3, &USART_InitStructure);
    USART_ClockInit(USART3, &USART_ClockInitStruct);

    //使能USART2接收中断
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    //使能USART2
    USART_Cmd(USART3, ENABLE);
}

u8 Tx3Buffer[256];
u8 Tx3Counter = 0;
u8 count3 = 0;
void DrvUart3SendBuf(unsigned char *DataToSend, u8 data_num)
{
    u8 i;
    for (i = 0; i < data_num; i++)
    {
        Tx3Buffer[count3++] = *(DataToSend + i);
    }
    if (!(USART3->CR1 & USART_CR1_TXEIE))
    {
        USART_ITConfig(USART3, USART_IT_TXE, ENABLE); //打开发送中断
    }
}
u8 U3RxDataTmp[100];
u8 U3RxInCnt = 0;
u8 U3RxoutCnt = 0;
void drvU3GetByte(u8 data)
{
	U3RxDataTmp[U3RxInCnt++] = data;
	if(U3RxInCnt >= 100)
		U3RxInCnt = 0;
}
void drvU3DataCheck(void)
{
	while(U3RxInCnt!=U3RxoutCnt)
	{
		U3GetOneByte(U3RxDataTmp[U3RxoutCnt++]);
		if(U3RxoutCnt >= 100)
			U3RxoutCnt = 0;
	}
}
void USART3_IRQHandler(void)
{
    u8 com_data;

    if (USART3->SR & USART_SR_ORE) //ORE中断
        com_data = USART3->DR;

    //接收中断
    if (USART_GetITStatus(USART3, USART_IT_RXNE))
    {
        USART_ClearITPendingBit(USART3, USART_IT_RXNE); //清除中断标志
        com_data = USART3->DR;
		drvU3GetByte(com_data);
    }
    //发送（进入移位）中断
    if (USART_GetITStatus(USART3, USART_IT_TXE))
    {
        USART3->DR = Tx3Buffer[Tx3Counter++]; //写DR清除中断标志
        if (Tx3Counter == count3)
        {
            USART3->CR1 &= ~USART_CR1_TXEIE; //关闭TXE（发送中断）中断
        }
    }
}
#endif

//重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch,FILE *f)
{
	//发送一个字节数据到串口
	USART_SendData(USART3,(uint8_t) ch);
	//等待发送完毕
	while(USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET);
	return (ch);
}

//重定向c库函数scanf到串口，重定向后可使用scanf,getchar等函数
int fgetc(FILE *f)
{
	//等待串口输入数据
	while(USART_GetFlagStatus(USART3,USART_FLAG_RXNE) == RESET);
	return (int)USART_ReceiveData(USART3);
}


void DrvUartDataCheck(void)
{
	//drvU1DataCheck();
	//drvU2DataCheck();
	drvU3DataCheck();
}
