#include "math_lib.h"

union Ufloat transition_data;
float GET_float(u32 hex)
{
	transition_data.by4[0] = BYTE0(hex);
	transition_data.by4[1] = BYTE1(hex);
	transition_data.by4[2] = BYTE2(hex);
	transition_data.by4[3] = BYTE3(hex);
	return transition_data.f;
}


float my_abs(float num)
{
	if(num >= 0 )
		return num;
	else 
		return -num;
}


u8 get_symbol(float num)
{
	if(num < 0)
		return 0;
	else
		return 1;
}
