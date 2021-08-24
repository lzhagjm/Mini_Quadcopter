#ifndef _BSP_ADC_H_
#define _BSP_ADC_H_	
#include "sys.h" 

#define RHEOSTAT_NOFCHANEL      2
 							   
void adc1_init(void); 				//ADC通道初始化
//u16  Get_Adc(u8 ch); 				//获得某个通道值 
//u16 Get_Adc_Average(u8 ch,u8 times);//得到某个通道给定次数采样的平均值  
float Get_temperature(void);

#endif 
