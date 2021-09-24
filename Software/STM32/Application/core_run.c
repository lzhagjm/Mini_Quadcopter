#include "core_run.h"


void core_run_init(uint16_t cycle)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = cycle * 1000 - 1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 72-1;  	
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_TIM4_P;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_TIM4_S;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	
	TIM_Cmd(TIM4, ENABLE);
}


u16 core_run_time;
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		Core_Run();
		core_run_time = TIM_GetCounter(TIM4);
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
}
