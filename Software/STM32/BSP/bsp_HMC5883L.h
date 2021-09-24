#ifndef _BSP_HMC5883L_H_
#define _BSP_HMC5883L_H_
#include "sys.h"


#define	SlaveAddress   0x1E	  //定义器件在IIC总线中的从地址






u8 hmc5883l_init(void);
int HMC5883L_I2C_ReadRegister(unsigned char slave_addr, unsigned char reg_addr,unsigned char length, unsigned char *data);

void HMC5883_Get_Data(s16 *x, s16 *y, s16 *z );


#endif

