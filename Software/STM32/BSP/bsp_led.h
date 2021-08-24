#ifndef _BSP_LED_H_
#define _BSP_LED_H_
#include "sys.h"

#define LED1 PBout(3)	  // D1
#define LED2 PBout(4)	// D2
#define LED_PWR PAout(15)
#define LED1_T  digitalToggle(GPIOB,GPIO_Pin_3)	       // D1
#define LED2_T  digitalToggle(GPIOB,GPIO_Pin_4)	       // D2	
#define LED_PWR_T  digitalToggle(GPIOA,GPIO_Pin_15)	       
void led_init(void);//≥ı ºªØ





#endif  



