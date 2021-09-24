#include "bsp_adc.h"
	 

__IO uint16_t ADC_ConvertedValue[RHEOSTAT_NOFCHANEL]={0}; 

													   
void adc1_init(void)
{    
  GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_InitTypeDef   ADC_InitStructure;
	DMA_InitTypeDef   DMA_InitStructure;
	
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能GPIOB时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 
	
	
  //先初始化ADC1通道10 IO口
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//PC0 通道10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化  
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 
	ADC_TempSensorVrefintCmd(ENABLE);//使能内部温度传感器
	
	DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr =  ((uint32_t)0x4001244C);
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

  //             tow
  DMA_InitStructure.DMA_BufferSize = 2;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;

  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  DMA_Cmd(DMA1_Channel1, ENABLE);
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;


  //             tow
  ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;
  ADC_InitStructure.ADC_NbrOfChannel = 2;
  
  ADC_Init(ADC1, &ADC_InitStructure);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_55Cycles5);
  ADC_DMACmd(ADC1, ENABLE);
  ADC_Cmd(ADC1, ENABLE);
  ADC_ResetCalibration(ADC1);
  while(ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}				  



float Get_temperature(void)   
{
	float temperate;
	u8 i;
	u32 adcx;
	for(i = 0 ; i < 10 ; i++)
		{
			Delay_us(5);
			adcx += ADC_ConvertedValue[0];	//读取通道16内部温度传感器通道,10次取平均
		}
		
	temperate=(float)adcx*(3.3/40960);		//电压值
	temperate=(1.43f - temperate)/0.0043f + 25; //转换为温度值 
	return temperate;	//返回最近一次ADC1规则组的转换结果
}


