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

	while( code1 || code2)
	{
		code1 = MPU_Init();
		code2 = mpu_dmp_init();
	}

	HOLLOW_CUP_Init(10000,1);  //8.4k

	
	
	
	
	
/****************系统末尾初始化******************/
	core_run_init(CYCLE);

	//初始化完成，返回0
	return (0);
}






