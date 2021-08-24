#ifndef	_HOLLOW_CUP_H_
#define _HOLLOW_CUP_H_
#include "sys.h"
#define CupGroup1_TIM										TIM2
#define CupGroup1_TIM_CLK     					RCC_APB1Periph_TIM2
#define CupGroup1_TIM_AF								GPIO_AF_TIM2

#define CupGroup2_TIM										TIM3
#define CupGroup2_TIM_CLK     					RCC_APB1Periph_TIM3
#define CupGroup2_TIM_AF								GPIO_AF_TIM3

#define CupGroup1_TIM_CH1_CLK						RCC_AHB1Periph_GPIOA				   
#define CupGroup1_TIM_CH1_PORT    	 		GPIOA      
#define CupGroup1_TIM_CH1_PIN      			GPIO_Pin_5
#define CupGroup1_TIM_CH1_PINSOURCE			GPIO_PinSource5


#define CupGroup1_TIM_CH2_CLK						RCC_AHB1Periph_GPIOA				   
#define CupGroup1_TIM_CH2_PORT     			GPIOA      
#define CupGroup1_TIM_CH2_PIN      			GPIO_Pin_1
#define CupGroup1_TIM_CH2_PINSOURCE			GPIO_PinSource1

#define CupGroup1_TIM_CH3_CLK						RCC_AHB1Periph_GPIOA				   
#define CupGroup1_TIM_CH3_PORT     			GPIOA      
#define CupGroup1_TIM_CH3_PIN      			GPIO_Pin_2 
#define CupGroup1_TIM_CH3_PINSOURCE			GPIO_PinSource2

#define CupGroup1_TIM_CH4_CLK						RCC_AHB1Periph_GPIOA				   
#define CupGroup1_TIM_CH4_PORT     			GPIOA      
#define CupGroup1_TIM_CH4_PIN      			GPIO_Pin_3 
#define CupGroup1_TIM_CH4_PINSOURCE			GPIO_PinSource3

#define CupGroup2_TIM_CH1_CLK						RCC_AHB1Periph_GPIOA				   
#define CupGroup2_TIM_CH1_PORT    		 	GPIOA      
#define CupGroup2_TIM_CH1_PIN      			GPIO_Pin_6 
#define CupGroup2_TIM_CH1_PINSOURCE			GPIO_PinSource6

#define CupGroup2_TIM_CH2_CLK						RCC_AHB1Periph_GPIOA				   
#define CupGroup2_TIM_CH2_PORT     			GPIOA      
#define CupGroup2_TIM_CH2_PIN      			GPIO_Pin_7 
#define CupGroup2_TIM_CH2_PINSOURCE			GPIO_PinSource7

#define CupGroup2_TIM_CH3_CLK						RCC_AHB1Periph_GPIOC				   
#define CupGroup2_TIM_CH3_PORT     			GPIOC      
#define CupGroup2_TIM_CH3_PIN      			GPIO_Pin_8 
#define CupGroup2_TIM_CH3_PINSOURCE			GPIO_PinSource8

#define CupGroup2_TIM_CH4_CLK						RCC_AHB1Periph_GPIOC				   
#define CupGroup2_TIM_CH4_PORT    	 		GPIOC      
#define CupGroup2_TIM_CH4_PIN      			GPIO_Pin_9 
#define CupGroup2_TIM_CH4_PINSOURCE			GPIO_PinSource9




void HOLLOW_CUP_Init(u32 arr, int psc);
void SetSpeedHollowCup1(int speed);
void SetSpeedHollowCup2(int speed);
void SetSpeedHollowCup3(int speed);
void SetSpeedHollowCup4(int speed);


#endif

