#ifndef _FIR_LIB_H_
#define _FIR_LIB_H_
#include "sys.h"
typedef struct{
	float Q;
	float R;
	float x_last;
	float p_last;
	float x_now;
}FIR_TypeDef;

float KalmanFilter(FIR_TypeDef* FIR,float ADC_Value);

#endif



