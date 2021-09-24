#include "bsp_led.h"

void led_init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);//使能GPIOF时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
  //GPIOF9,F10初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//普通输出模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//关灯
	LED1 = 1;
	LED2 = 1;
	LED_PWR = 1;
}

