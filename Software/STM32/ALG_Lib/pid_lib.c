#include "pid_lib.h"


void PID_set_P(PID_TypeDef* PID , float Kp)
{
	PID->Kp = Kp;
}

void PID_set_I(PID_TypeDef* PID , float Ki)
{
	PID->Ki = Ki;
}

void PID_set_D(PID_TypeDef* PID , float Kd)
{
	PID->Kd = Kd;
}

//设置目标值
void Set_target_val(PID_TypeDef* PID,float target_val)
{
	PID->target_val = target_val;
}






/*            标准位置式PD         */
float Compute_Position_PD(PID_TypeDef* PID)
{
	PID->err = PID->observed_val - PID->target_val;
	//计算PID
	PID->output_val = PID->Kp * PID->err + PID->Kd * (PID->err - PID->last_err);
	if(PID->output_val > PID->output_limit_up)
		PID->output_val = PID->output_limit_up;
	if(PID->output_val < PID->output_limit_down)
		PID->output_val = PID->output_limit_down;
	//误差传递
	PID->last_last_err = PID->last_err;
	PID->last_err = PID->err;
	return PID->output_val;
}


extern float pitch,roll;
extern short gx,gy;

/*            标准位置式PID         */
float Compute_Position_PID(PID_TypeDef* PID)
{
	PID->err = PID->target_val - PID->observed_val;
	if((my_abs(pitch) >= 0.6f || my_abs(roll)>= 0.6f) && (my_abs(gx) > 10 || my_abs(gy) > 10))
		PID->integral +=  PID->err;
	else
		PID->integral = 0;
  if(PID->integral >= PID->Integral_limit_up)
		PID->integral = PID->Integral_limit_up;
	if(PID->integral <= PID->Integral_limit_down)
		PID->integral = PID->Integral_limit_down;
	//计算PID
	PID->output_val = PID->Kp * PID->err + PID->Ki * PID->integral + PID->Kd * (PID->err - PID->last_err);
	if(PID->output_val > PID->output_limit_up)
		PID->output_val = PID->output_limit_up;
	if(PID->output_val < PID->output_limit_down)
		PID->output_val = PID->output_limit_down;
	//误差传递
	PID->last_last_err = PID->last_err;
	PID->last_err = PID->err;
	return PID->output_val;
}


/*            标准增量式PID         */
float Compute_Incremental_PID(PID_TypeDef* PID)
{
	PID->err = PID->target_val - PID->observed_val;
	PID->output_val = PID->Kp * (PID->err - PID->last_err) + PID->Ki * (PID->err) + PID->Kd * (PID->err - 2 * PID->last_err + PID->last_last_err);
	PID->last_last_err = PID->last_err;
	PID->last_err = PID->err;
	if(PID->output_val > PID->output_limit_up)
		PID->output_val = PID->output_limit_up;
	if(PID->output_val < PID->output_limit_down)
		PID->output_val = PID->output_limit_down;
	return PID->output_val;
}





