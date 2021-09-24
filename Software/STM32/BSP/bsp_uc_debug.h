#ifndef 	_DRAW_WAVE_H_
#define		_DRAW_WAVE_H_
#include "sys.h"


void data_analysis(u8 data);
void Reported_data(u8 fun,u8 len,u8 *data);
void Reported_PID_data(float target_value,float actual_value,float adjustable_value);
void Reported_MPU6050_data(float rol,float pit,float yaw);
void Reported_HMC5883L_data(s16 x,s16 y,s16 z, s32 h , s16 T, u8 BAR_STA , u8 MAG_STA);
void Reported_one_data(float data);
#endif

