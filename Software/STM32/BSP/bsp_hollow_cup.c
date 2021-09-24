#include "bsp_hollow_cup.h"

void TIMx_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//定义PWM模式结构体
	TIM_OCInitTypeDef TIM_OCInitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//启开定时器时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	//定义模式配置结构体
	TIM_TimeBaseInitTypeDef TIM_ModeStruct;
	//设置模式
	//选择预分频为1（72Mhz）
	TIM_ModeStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	//设置向上计数模式
	TIM_ModeStruct.TIM_CounterMode = TIM_CounterMode_Up;
	//设置计数上限为0~4999 共计数5000次
	TIM_ModeStruct.TIM_Period = 1000-1;
	//设置频率 = TIMx_CLK/（TIM_Prescaler+1） = 10000Hz
	//==>TIM_Prescaler = 7200-1
	//每计数一次时间：t = 1/10000Hz = 100us
	//计数5000次0.5s
	TIM_ModeStruct.TIM_Prescaler = 72-1;
	//初始化定时器
	TIM_TimeBaseInit(TIM2, &TIM_ModeStruct);

	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;   //配置pwm模式1
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 0;
	//启用结构体
	TIM_OC1Init(TIM2, &TIM_OCInitStruct);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	TIM_OCInitStruct.TIM_Pulse = 0;
	//启用结构体
	TIM_OC2Init(TIM2, &TIM_OCInitStruct);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	TIM_OCInitStruct.TIM_Pulse = 0;
	//启用结构体
	TIM_OC3Init(TIM2, &TIM_OCInitStruct);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

	TIM_OCInitStruct.TIM_Pulse = 0;
	//启用结构体
	TIM_OC4Init(TIM2, &TIM_OCInitStruct);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM2,ENABLE);
}


void SetSpeedHollowCup1(uint16_t speed)
{
	TIM_SetCompare3(TIM2,speed);
}
void SetSpeedHollowCup2(uint16_t speed)
{
	TIM_SetCompare4(TIM2,speed);
}
void SetSpeedHollowCup3(uint16_t speed)
{
	TIM_SetCompare1(TIM2,speed);
}
void SetSpeedHollowCup4(uint16_t speed)
{
	TIM_SetCompare2(TIM2,speed);
}

