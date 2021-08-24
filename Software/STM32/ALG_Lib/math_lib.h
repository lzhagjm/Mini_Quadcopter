#ifndef _MATH_LIB_H_
#define _MATH_LIB_H_
#include "sys.h"

//取位操作
#define BYTE0(dwTemp) (*((char *)(&dwTemp)))
#define BYTE1(dwTemp) (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp) (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp) (*((char *)(&dwTemp) + 3))

//数据转换
union Ufloat{
	float f;
	u8 by4[4];
};

float my_abs(float num);
float GET_float(u32 hex);
u8 get_symbol(float num);
#endif



