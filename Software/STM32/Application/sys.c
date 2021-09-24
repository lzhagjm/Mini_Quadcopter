#include "sys.h"






u8 All_Init(void)
{
	u8 code1 = 1, code2 = 1;
	/****************系统固有******************/
	//中断优先级组别设置
	NVIC_PriorityGroupConfig(NVIC_GROUP);		
	//滴答定时器
	SysTick_Init(); 	
	//延时
	Delay_ms(100);
	led_init();
	adc1_init();


	
	
/****************安需选择******************/

	BlueTooth_Init();
	//usart1_init(500000);
	while( code1 || code2)
	{
		code1 = MPU_Init();
		code2 = mpu_dmp_init();
		
		code1 ? (LED1 = 0) : (LED1 = 1);
		code2 ? (LED2 = 0) : (LED2 = 1);
	}
	//hmc5883l_init();
	LED1 = 1;
	LED2 = 1;
	TIMx_Config();

	
	
	
	
	
/****************系统末尾初始化******************/
	core_run_init(CYCLE);

	//初始化完成，返回0
	return (0);
}






