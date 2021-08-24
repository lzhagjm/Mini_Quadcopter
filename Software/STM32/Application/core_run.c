#include "core_run.h"


void core_run_init(uint16_t cycle)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); 
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = cycle * 1000 - 1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 84-1;  	
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStruct);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_TIM7_P;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_TIM7_S;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
	TIM_ClearITPendingBit(TIM7,TIM_IT_Update);
	
	TIM_Cmd(TIM7, ENABLE);
}


u16 core_run_time;
void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
	{
		Core_Run();
		core_run_time = TIM_GetCounter(TIM7);
	}
	TIM_ClearITPendingBit(TIM7,TIM_IT_Update);
}
