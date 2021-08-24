#include "FIR_lib.h"

float KalmanFilter(FIR_TypeDef* FIR,float ADC_Value)
{
	float x_k1_k1,x_k_k1;
	float Z_k;

	float Kg = 0;
	float P_k_k1;

	float kalman_adc;

	Z_k = ADC_Value;

	if (my_abs(FIR->x_last-ADC_Value)>=0.5f)
	{ 
			x_k1_k1= ADC_Value*0.382f + FIR->x_last*0.618f;
	}else
	{
			x_k1_k1 = FIR->x_last;
	}
	x_k_k1 = x_k1_k1;
	P_k_k1 = FIR->p_last + FIR->Q;

	Kg = P_k_k1/(P_k_k1 + FIR->R);

	kalman_adc = x_k_k1 + Kg * (Z_k - FIR->x_last);
	FIR->p_last = (1 - Kg)*P_k_k1;
	P_k_k1 = FIR->p_last;

	FIR->x_last = kalman_adc;

	return kalman_adc;
}




///*-------------------------------------------------------------------------------------------------------------*/
///*        
//        Q:过程噪声，Q增大，动态响应变快，收敛稳定性变坏
//        R:测量噪声，R增大，动态响应变慢，收敛稳定性变好        
//*/

//float KalmanFilter(FIR_TypeDef* FIR,float ResrcData)
//{
//        float x_mid = FIR->x_last;

//        float p_mid;
//        float p_now;
//        float kg;        

//        x_mid = FIR->x_last; //x_last=x(k-1|k-1),x_mid=x(k|k-1)
//        p_mid = FIR->p_last + FIR->Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=噪声
//        kg = p_mid / ( p_mid + FIR->R); //kg为kalman filter，R为噪声
//        FIR->x_now = x_mid +kg * (ResrcData - x_mid);//估计出的最优值
//                
//        p_now=(1-kg)*p_mid;//最优值对应的covariance        

//        FIR->p_last = p_now; //更新covariance值
//        FIR->x_last = FIR->x_now; //更新系统状态值

//        return FIR->x_now;                
//}

