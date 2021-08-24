#include "bsp_bluetooth.h"




static u8 buff_temp[RX_BUFFER_SIZE];
//初始化串口GPIO
static void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(USARTx_TX_CLK|USARTx_RX_CLK,ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin = USARTx_RX_PIN;
	GPIO_Init(USARTx_RX_PORT,&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin = USARTx_TX_PIN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USARTx_TX_PORT,&GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
}


//初始化串口
static void USART_Config(void)
{
	USART_InitTypeDef USART_InitStruct;
	RCC_APB2PeriphClockCmd(USARTx_CLK,ENABLE);
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USARTx,&USART_InitStruct);
	USART_DMACmd(USARTx,USART_DMAReq_Rx,ENABLE);
	USART_DMACmd(USARTx,USART_DMAReq_Tx,ENABLE);
	USART_Cmd(USARTx,ENABLE);
}

BlueTooth Debug;

static void USART_DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	DMA_InitStruct.DMA_BufferSize = RX_BUFFER_SIZE;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
	
	DMA_InitStruct.DMA_Channel = DMA_Channel_4;
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)(Debug.rx_buffer);
	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&(USARTx->DR);
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
	DMA_Init(DMA2_Stream5,&DMA_InitStruct);
	DMA_InitStruct.DMA_BufferSize = TX_BUFFER_SIZE;
	DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)&(Debug.head);
	DMA_Init(DMA2_Stream7,&DMA_InitStruct);
	DMA_Cmd(DMA2_Stream7,ENABLE);
	DMA_Cmd(DMA2_Stream5,ENABLE);
}

void BlueTooth_Init(void)
{
	GPIO_Config();
	USART_Config();
	USART_DMA_Config();
	Debug.head = BUFFER_HEAD;
	Debug.end = BUFFER_END;
}

void calculate_cs(void)
{
	uint8_t i;
	uint8_t *temp;
	Debug.cs = 0;
	temp = (uint8_t *)&Debug.head;
	for(i = 0 ; i < TX_BUFFER_SIZE - 3;i++)
	{
		temp++;
		Debug.cs += *temp;
	}
}

/*寻找帧头帧尾,检查包完整性*/
static uint8_t find_head_end(u8* buff)
{
	uint8_t i,j,start = RX_BUFFER_SIZE,end = RX_BUFFER_SIZE;
	uint16_t sum = 0;
	for(i = 0 ; i < RX_BUFFER_SIZE; i++)
	{
		if(buff[i] == BUFFER_HEAD)
		{
			start = i;
			if(buff[(i + RX_BUFFER_SIZE - 1) % RX_BUFFER_SIZE] == BUFFER_END)
			{
				end = (i + RX_BUFFER_SIZE - 1) % RX_BUFFER_SIZE;
				for(j = 0 ; j < RX_BUFFER_SIZE - 3;j++)
				{
					sum += buff[(start + 1 +j) % RX_BUFFER_SIZE];
				}
				if((uint8_t)sum != buff[(start + 1 +j) % RX_BUFFER_SIZE])
				{
					end = RX_BUFFER_SIZE;
					sum = 0;
				}
			}else
			{
				start = RX_BUFFER_SIZE;
			}
		}
	}
	if(start == RX_BUFFER_SIZE || end == RX_BUFFER_SIZE)
		return RX_BUFFER_SIZE;
	else return start;
}
/*写入数据*/
uint8_t BlueTooth_Write_data(BlueTooth* Debug)				//添加数据需改
{
	for(u16 i = 0 ; i < RX_BUFFER_SIZE ; i++)
		buff_temp[i] = Debug->rx_buffer[i];
	uint16_t start = find_head_end(buff_temp);
	u8 _cnt = 1;
	if(start != RX_BUFFER_SIZE)
	{
//			Debug->pwm1[0] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
//			Debug->pwm1[1] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
//			Debug->pwm1[2] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
//			Debug->pwm1[3] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
//			Debug->pwm2[0] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
//			Debug->pwm2[1] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
//			Debug->pwm2[2] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
//			Debug->pwm2[3] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
				
		
//			Debug->s1[0] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
//			Debug->s1[1] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
//			Debug->s1[2] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
//			Debug->s1[3] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
//			Debug->s2[0] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
//			Debug->s2[1] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
//			Debug->s2[2] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
//			Debug->s2[3] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
//			Debug->s3[0] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
//			Debug->s3[1] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
//			Debug->s3[2] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
//			Debug->s3[3] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
//			Debug->s4[0] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
//			Debug->s4[1] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
//			Debug->s4[2] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
//			Debug->s4[3] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];

		
		Debug->P1[0] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
		Debug->P1[1] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
		Debug->P1[2] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
		Debug->P1[3] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
		Debug->I1[0] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
		Debug->I1[1] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
		Debug->I1[2] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
		Debug->I1[3] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
		Debug->D1[0] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
		Debug->D1[1] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
		Debug->D1[2] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
		Debug->D1[3] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
		Debug->P2[0] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
		Debug->P2[1] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
		Debug->P2[2] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
		Debug->P2[3] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
		Debug->I2[0] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
		Debug->I2[1] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
		Debug->I2[2] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
		Debug->I2[3] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
		Debug->D2[0] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
		Debug->D2[1] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
		Debug->D2[2] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
		Debug->D2[3] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
//		
//				Debug->x[0] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
//				Debug->x[1] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
//				Debug->x[2] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
//				Debug->x[3] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
//				Debug->y[0] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
//				Debug->y[1] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
//				Debug->y[2] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
//				Debug->y[3] = buff_temp[(start + _cnt++) % RX_BUFFER_SIZE];
		return 1;
	}else return 0;
}

