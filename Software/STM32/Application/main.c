#include "sys.h"


int main(void)
{
	//进行所有设备的初始化，并将初始化结果保存
	All_Init();
	//调度器初始化，系统为裸奔，这里人工做了一个时分调度器
	Scheduler_Setup();
	while (1)
	{
		//运行任务调度器，所有系统功能，除了中断服务函数，都在任务调度器内完成
		Scheduler_Run();
			
	}
}

