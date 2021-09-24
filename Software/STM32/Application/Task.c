#include "Task.h"

/*****************固有变量*************/
extern u16 core_run_time;
extern BlueTooth Debug;



/***************自定义变量***************/



float pitch,roll,yaw;
short gx,gy,gz;
s16 x,y,z;



/*************核心代码   精确周期调用****************/
void Core_Run(void)
{
	
}


void display_core_run_time(void)
{
	
}


void mpu6050_test(void)
{
		while(mpu_dmp_get_data(&pitch,&roll,&yaw) != 0);
		HMC5883_Get_Data(&x, &y,&z);
		Reported_MPU6050_data(roll,pitch,yaw);
		Reported_HMC5883L_data(x,y,z,0,0,1,1);
}


void display(void)
{
	float pwm;
	BlueTooth_Write_data(&Debug);
	pwm = GET_float(*(u32*)Debug.s1);
	for(u8 i = 0 ; i < 7 ; i++)
		printf("%d\t",Debug.rx_buffer[i]);
	printf("\n");
	SetSpeedHollowCup1((u16)(pwm * 1000));
	SetSpeedHollowCup2((u16)(pwm * 1000));
	SetSpeedHollowCup3((u16)(pwm * 1000));
	SetSpeedHollowCup4((u16)(pwm * 1000));
}


