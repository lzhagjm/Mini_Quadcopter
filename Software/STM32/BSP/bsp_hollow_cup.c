#include "bsp_hollow_cup.h"

void HOLLOW_CUP_GPIO_Config(void)
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	/*开启相关的GPIO外设时钟*/
	RCC_AHB1PeriphClockCmd (CupGroup1_TIM_CH1_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd (CupGroup1_TIM_CH2_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd (CupGroup1_TIM_CH3_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd (CupGroup1_TIM_CH4_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd (CupGroup2_TIM_CH1_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd (CupGroup2_TIM_CH2_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd (CupGroup2_TIM_CH3_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd (CupGroup2_TIM_CH4_CLK, ENABLE);	
  /* 定时器通道引脚复用 */
	GPIO_PinAFConfig(CupGroup1_TIM_CH1_PORT,CupGroup1_TIM_CH1_PINSOURCE,CupGroup1_TIM_AF); 
  GPIO_PinAFConfig(CupGroup1_TIM_CH2_PORT,CupGroup1_TIM_CH2_PINSOURCE,CupGroup1_TIM_AF);
	GPIO_PinAFConfig(CupGroup1_TIM_CH3_PORT,CupGroup1_TIM_CH3_PINSOURCE,CupGroup1_TIM_AF);
	GPIO_PinAFConfig(CupGroup1_TIM_CH4_PORT,CupGroup1_TIM_CH4_PINSOURCE,CupGroup1_TIM_AF);
	GPIO_PinAFConfig(CupGroup2_TIM_CH1_PORT,CupGroup2_TIM_CH1_PINSOURCE,CupGroup2_TIM_AF);
	GPIO_PinAFConfig(CupGroup2_TIM_CH2_PORT,CupGroup2_TIM_CH2_PINSOURCE,CupGroup2_TIM_AF);
	GPIO_PinAFConfig(CupGroup2_TIM_CH3_PORT,CupGroup2_TIM_CH3_PINSOURCE,CupGroup2_TIM_AF);
	GPIO_PinAFConfig(CupGroup2_TIM_CH4_PORT,CupGroup2_TIM_CH4_PINSOURCE,CupGroup2_TIM_AF);
	/* 定时器通道引脚配置 */															   		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	
	GPIO_InitStructure.GPIO_Pin = CupGroup1_TIM_CH1_PIN;
	GPIO_Init(CupGroup1_TIM_CH1_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CupGroup1_TIM_CH2_PIN;
	GPIO_Init(CupGroup1_TIM_CH2_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CupGroup1_TIM_CH3_PIN;
	GPIO_Init(CupGroup1_TIM_CH3_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CupGroup1_TIM_CH4_PIN;
	GPIO_Init(CupGroup1_TIM_CH4_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CupGroup2_TIM_CH1_PIN;
	GPIO_Init(CupGroup2_TIM_CH1_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CupGroup2_TIM_CH2_PIN;
	GPIO_Init(CupGroup2_TIM_CH2_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CupGroup2_TIM_CH3_PIN;
	GPIO_Init(CupGroup2_TIM_CH3_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CupGroup2_TIM_CH4_PIN;
	GPIO_Init(CupGroup2_TIM_CH4_PORT, &GPIO_InitStructure);
}

void HOLLOW_CUP_TIM_Mode_Config(u32 arr, int psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  				TIM_OCInitStructure;
	/***********************组1定时器初始化***************************/
	
	// 开启TIMx_CLK,x[2,3,4,5,12,13,14] 
  RCC_APB1PeriphClockCmd(CupGroup1_TIM_CLK, ENABLE); 
  /* 累计 TIM_Period个后产生一个更新或者中断*/		
  //当定时器从0计数到8399，即为8400次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Period = arr-1;       	
	// 通用控制定时器时钟源TIMxCLK = HCLK/2=84MHz 
	// 设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=100KHz
  TIM_TimeBaseStructure.TIM_Prescaler = psc-1;	
  // 采样时钟分频
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  // 计数方式
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	// 初始化定时器TIMx, x[2,3,4,5,12,13,14] 
	TIM_TimeBaseInit(CupGroup1_TIM, &TIM_TimeBaseStructure);
	/*PWM模式配置*/
	/* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  	  //当定时器计数值小于CCR1_Val时为高电平
  TIM_OC1Init(CupGroup1_TIM, &TIM_OCInitStructure);	 //使能通道1 
	TIM_OC2Init(CupGroup1_TIM, &TIM_OCInitStructure);	 //使能通道1
	TIM_OC3Init(CupGroup1_TIM, &TIM_OCInitStructure);	 //使能通道1
	TIM_OC4Init(CupGroup1_TIM, &TIM_OCInitStructure);	 //使能通道1
	/*使能通道1重载*/
	TIM_OC1PreloadConfig(CupGroup1_TIM, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(CupGroup1_TIM, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(CupGroup1_TIM, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(CupGroup1_TIM, TIM_OCPreload_Enable);	
	// 使能定时器
	TIM_Cmd(CupGroup1_TIM, ENABLE);
	
	
	/***********************组2定时器初始化***************************/
	
	// 开启TIMx_CLK,x[2,3,4,5,12,13,14] 
  RCC_APB1PeriphClockCmd(CupGroup2_TIM_CLK, ENABLE); 
  /* 累计 TIM_Period个后产生一个更新或者中断*/		
  //当定时器从0计数到8399，即为8400次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Period = arr-1;       	
	// 通用控制定时器时钟源TIMxCLK = HCLK/2=84MHz 
	// 设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=100KHz
  TIM_TimeBaseStructure.TIM_Prescaler = psc-1;	
  // 采样时钟分频
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  // 计数方式
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	
	// 初始化定时器TIMx, x[2,3,4,5,12,13,14] 
	TIM_TimeBaseInit(CupGroup2_TIM, &TIM_TimeBaseStructure);	
	/*PWM模式配置*/
	/* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  	  //当定时器计数值小于CCR1_Val时为高电平
  TIM_OC1Init(CupGroup2_TIM, &TIM_OCInitStructure);	 //使能通道1  
	TIM_OC2Init(CupGroup2_TIM, &TIM_OCInitStructure);	 //使能通道1
	TIM_OC3Init(CupGroup2_TIM, &TIM_OCInitStructure);	 //使能通道1
	TIM_OC4Init(CupGroup2_TIM, &TIM_OCInitStructure);	 //使能通道1
	/*使能通道1重载*/
	TIM_OC1PreloadConfig(CupGroup2_TIM, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(CupGroup2_TIM, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(CupGroup2_TIM, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(CupGroup2_TIM, TIM_OCPreload_Enable);	
	// 使能定时器
	TIM_Cmd(CupGroup2_TIM, ENABLE);
}

void HOLLOW_CUP_Init(u32 arr, int psc)
{
	HOLLOW_CUP_GPIO_Config();
	HOLLOW_CUP_TIM_Mode_Config(arr, psc);
}

void SetSpeedHollowCup1(int speed)
{
	if(speed>0)
	{
		TIM_SetCompare1(CupGroup1_TIM,speed);
		TIM_SetCompare2(CupGroup1_TIM,0);
	}
	else
	{
		TIM_SetCompare1(CupGroup1_TIM,0);
		TIM_SetCompare2(CupGroup1_TIM,-speed);
	}
}
void SetSpeedHollowCup2(int speed)
{
	if(speed>0)
	{
		TIM_SetCompare3(CupGroup1_TIM,speed);
		TIM_SetCompare4(CupGroup1_TIM,0);
	}
	else
	{
		TIM_SetCompare3(CupGroup1_TIM,0);
		TIM_SetCompare4(CupGroup1_TIM,-speed);
	}
}
void SetSpeedHollowCup3(int speed)
{
	if(speed>0)
	{
		TIM_SetCompare1(CupGroup2_TIM,speed);
		TIM_SetCompare2(CupGroup2_TIM,0);
	}
	else
	{
		TIM_SetCompare1(CupGroup2_TIM,0);
		TIM_SetCompare2(CupGroup2_TIM,-speed);
	}
}
void SetSpeedHollowCup4(int speed)
{
	if(speed>0)
	{
		TIM_SetCompare3(CupGroup2_TIM,speed);
		TIM_SetCompare4(CupGroup2_TIM,0);
	}
	else
	{
		TIM_SetCompare3(CupGroup2_TIM,0);
		TIM_SetCompare4(CupGroup2_TIM,-speed);
	}
}
