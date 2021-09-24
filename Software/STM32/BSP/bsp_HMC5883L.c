#include "bsp_HMC5883L.h"



//初始化HMC5883，根据需要请参考pdf进行修改****
u8 hmc5883l_init(void)
{
	u8 res = 0;
#if I2C_CHOOSE
	I2cMaster_Init();//初始化IIC总线
	res = H_I2C_Write_Byte(SlaveAddress,0x02,0x00);
	return res;
#else
	S_I2C_Init();//初始化IIC总线
	res = S_I2C_Write_Byte(SlaveAddress,0x02,0x00);	
	return res;
#endif
}



int HMC5883L_I2C_ReadRegister(unsigned char slave_addr, unsigned char reg_addr,unsigned char length, unsigned char *data)
{
#if I2C_CHOOSE
	return H_I2C_Read(slave_addr,reg_addr,length,data);
#else
	return S_I2C_Read(slave_addr,reg_addr,data,length);
#endif
}



//******************************************************
//
//连续读出HMC5883内部原始数据，地址范围0x3~0x5
//
//******************************************************
void HMC5883_Get_Data(s16 *x, s16 *y, s16 *z )
{   
	u8 data[6];
	HMC5883L_I2C_ReadRegister(SlaveAddress, 0x03,6, data);
	*x = data[0] << 8 | data[1]; //Combine MSB and LSB of X Data output register
  *z = data[2] << 8 | data[3]; //Combine MSB and LSB of Z Data output register
  *y = data[4] << 8 | data[5]; //Combine MSB and LSB of Y Data output register
}


