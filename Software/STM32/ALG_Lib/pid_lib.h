#ifndef _PID_LIB_H_
#define _PID_LIB_H_
#include "sys.h"

typedef struct{
	float Kp;           //初始P
	float Ki;						//初始P
	float Kd;							//初始P
	float err;						//初始误差
	float last_err;      //初始上一次误差
	float last_last_err;  //初始上上一次误差
	float observed_val;    //实际值
	float target_val;    //目标值
	float output_val;    //输出值
	float integral;      //累计误差
	float Integral_limit_up; //积分上限幅
	float Integral_limit_down; //积分下限幅
	float output_limit_up; 		//输出上限幅
	float output_limit_down; 		//输出下限幅
}PID_TypeDef;


void PID_set_P(PID_TypeDef* PID , float Kp);
void PID_set_I(PID_TypeDef* PID , float Ki);
void PID_set_D(PID_TypeDef* PID , float Kd);

void Set_target_val(PID_TypeDef* PID,float target_val);

/*            标准位置式PD         */
float Compute_Position_PD(PID_TypeDef* PID);
/*            标准位置式PID         */
float Compute_Position_PID(PID_TypeDef* PID);
/*            标准增量式PID         */
float Compute_Incremental_PID(PID_TypeDef* PID);

#endif
