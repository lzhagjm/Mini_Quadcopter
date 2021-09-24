#include "bsp_uc_debug.h"

static void call_back_send_char(u8 c)
{   	
	USART_SendData(USART1,c);  
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
} 



u8 uc_buff_temp[100];
/*                    发送数据包                   */
static void send_data_package(u8 addr, u8 fun, u8 data_len, u8*data)
{
	u8 i,_cnt = 0,sc = 0,ac = 0;
	uc_buff_temp[_cnt++]=0XAA;	//帧头
	uc_buff_temp[_cnt++]=addr;  //地址
	uc_buff_temp[_cnt++]=fun;	  //功能字
	uc_buff_temp[_cnt++]=data_len;	  //数据长度
	for(i=0;i<data_len;i++)uc_buff_temp[_cnt++]=data[i];			//复制数据
	for(i=0 ; i < uc_buff_temp[3] + 4 ; i++)            //计算校验
	{
			sc += uc_buff_temp[i];
      ac += sc;
	}	
	uc_buff_temp[_cnt++] = sc;
	uc_buff_temp[_cnt++] = ac;
	for(i=0;i<data_len+6;i++)call_back_send_char(uc_buff_temp[i]);	//发送数据到串口1 
}


/******************************参数设置******************************/

/*                    向上位机返回验证                   */
static void return_verification(u8 id_get, u8 sc_get , u8 ac_get)
{
	u8 uc_data_buff[10];
	uc_data_buff[0] = id_get;
	uc_data_buff[1] = sc_get;
	uc_data_buff[2] = ac_get;
	send_data_package(0xAF, 0x00,3, uc_data_buff);
}


/*                    向上位机返回参数                   */
static void return_parameter(u16 id , u32 data)
{
	u8 uc_data_buff[10];
	uc_data_buff[0] = BYTE0(id);
	uc_data_buff[1] = BYTE1(id);
	uc_data_buff[2] = BYTE0(data);
	uc_data_buff[3] = BYTE1(data);
	uc_data_buff[4] = BYTE2(data);
	uc_data_buff[5] = BYTE3(data);
	send_data_package(0xAF, 0xE2,6, uc_data_buff);
}


/*                    使用接受的数据包                   */
static void Use_packets(u8* buff)
{
	static u8 addr,fun;
	static u16 id;
	addr = buff[1];
	fun = buff[2];
	
	switch(addr)
	{
		case 0xFF:    //广播
		{
			switch(fun)  //功能码
			{
				case 0xE1:
				{
					id = buff[5]<<8 | buff[4];
					switch(id)
					{
						case 0x01:
							return_parameter(id,0x05);
							break;
					 default : return_parameter(id,id);
					}
					break;
				}
	
			}
			break;
		}
		case 0x05:    //飞控
		{
			switch(fun)  //功能码
			{
				case 0xE1:   //读取参数
				{
					id = buff[5]<<8 | buff[4];
					switch(id)
					{
						case 0x01:  //硬件种类
							return_parameter(id,0x05);
							break;
						case 11:  //PID_1P
							//return_parameter(id,(u32)(PID1.Kp*1000));
							break;
						case 12: //PID_1I
							//return_parameter(id,(u32)(PID1.Ki*1000));
							break;
						case 13: //PID_1D
							//return_parameter(id,(u32)(PID1.Kd*1000));
							break;
						case 14:  //PID_2P
							//return_parameter(id,(u32)(PID1.target_val));
							break;
						case 15: //PID_2I
							return_parameter(id,0);
							break;
						case 16: //PID_2D
							return_parameter(id,0);
							break;
					 default : return_parameter(id,0);
					}
					break;
				}
				case 0xE2:  //写入参数  
				{
						id = buff[5]<<8 | buff[4];
					switch(id)
					{
						case 11:  //PID_1P
							//PID1.Kp = (float)((buff[6]) | (buff[7] << 8) | (buff[8] << 16) | (buff[9] << 24))/1000;
							return_verification(fun, buff[10] , buff[11]);
							break;
						case 12: //PID_1I
							//PID1.Ki = (float)((buff[6]) | (buff[7] << 8) | (buff[8] << 16) | (buff[9] << 24))/1000;
							return_verification(fun, buff[10] , buff[11]);
							break;
						case 13: //PID_1D
							//PID1.Kd = (float)((buff[6]) | (buff[7] << 8) | (buff[8] << 16) | (buff[9] << 24))/1000;
							return_verification(fun, buff[10] , buff[11]);
							break;
						case 14:  //PID_2P
							//PID1.target_val = (float)((buff[6]) | (buff[7] << 8) | (buff[8] << 16) | (buff[9] << 24));
							return_verification(fun, buff[10] , buff[11]);
							break;
						case 15: //PID_2I
							//pid2[1] = (buff[6]) | (buff[7] << 8) | (buff[8] << 16) | (buff[9] << 24);
							return_verification(fun, buff[10] , buff[11]);
							break;
						case 16: //PID_2D
							//pid2[2] = (buff[6]) | (buff[7] << 8) | (buff[8] << 16) | (buff[9] << 24);
							return_verification(fun, buff[10] , buff[11]);
							break;
					 default : return_verification(fun, buff[10] , buff[11]);
					}
				}
			}
			break;
		}
	}
}



/*                    解析接受的数据包                   */
void data_analysis(u8 data)
{
	static u8 sta = 0,i;
	static u8 _cnt = 0 ,len = 0,sc_temp,ac_temp,sc,ac;
	uc_buff_temp[_cnt++] = data;
	if(sta == 0)
	{
		if(data == 0xAA)
			sta = 1;
		else
			_cnt = 0;
	}else if(sta == 1)
	{
		 sta = 2;           //地址
	}else if(sta == 2)
	{
		  sta = 3;               //功能码
	}else if(sta == 3)
	{
		   if(data > 50)                       //数据长度
			 {
					sta = 0;
				 _cnt = 0;
			 }else
			{
				sta = 4;
				len = data;
			}
	}
	else if(sta == 4)            //数据
	{
		len--;
		if(len == 0)
			sta = 5;
	}else if(sta == 5)        //sc
	{
		sta = 6;
		sc_temp = data;
	}else if(sta == 6)    //ac
	{
		sta = 0;
		_cnt = 0;
		ac_temp = data;
		sc = 0;
		ac = 0;
		for(i = 0 ; i < uc_buff_temp[3] + 4 ; i++)            //计算校验
		{
			sc += uc_buff_temp[i];
      ac += sc;
		}
		if((sc == sc_temp ) && (ac == ac_temp))
		{
			Use_packets(uc_buff_temp);
		}
	}
}


/**************************状态上报********************************/

//上报任意数组
void Reported_data(u8 fun,u8 len,u8 *data)
{
	send_data_package(0xAF, fun, len, data);
}

//上报PID 目标值、实际值、PID调节值
void Reported_PID_data(float target_value,float actual_value,float adjustable_value)
{
	u8 buff[12],_cnt = 0;
	s32 target_value_temp = (s32)(target_value * 1000);
	s32 actual_value_temp = (s32)(actual_value * 1000);
	s32 adjustable_value_temp = (s32)(adjustable_value * 1000);
	buff[_cnt++] = BYTE0(target_value_temp);
	buff[_cnt++] = BYTE1(target_value_temp);
	buff[_cnt++] = BYTE2(target_value_temp);
	buff[_cnt++] = BYTE3(target_value_temp);
	buff[_cnt++] = BYTE0(actual_value_temp);
	buff[_cnt++] = BYTE1(actual_value_temp);
	buff[_cnt++] = BYTE2(actual_value_temp);
	buff[_cnt++] = BYTE3(actual_value_temp);
	buff[_cnt++] = BYTE0(adjustable_value_temp);
	buff[_cnt++] = BYTE1(adjustable_value_temp);
	buff[_cnt++] = BYTE2(adjustable_value_temp);
	buff[_cnt++] = BYTE3(adjustable_value_temp);
	Reported_data(0xF1,_cnt,buff);
}

//上报MPU6050 欧拉角 横滚、俯仰、航向
void Reported_MPU6050_data(float rol,float pit,float yaw)
{
	u8 buff[12],_cnt = 0;
	s16 rol_temp = (s16)(rol * 100);
	s16 pit_temp = (s16)(pit * 100);
	s16 yaw_temp = (s16)(yaw * 100);
	buff[_cnt++] = BYTE0(rol_temp);
	buff[_cnt++] = BYTE1(rol_temp);
	buff[_cnt++] = BYTE0(pit_temp);
	buff[_cnt++] = BYTE1(pit_temp);
	buff[_cnt++] = BYTE0(yaw_temp);
	buff[_cnt++] = BYTE1(yaw_temp);
	buff[_cnt++] = 1;
	Reported_data(0x03,_cnt,buff);
}

//上报 0x02  罗盘、气压、温度 传感器数据
void Reported_HMC5883L_data(s16 x,s16 y,s16 z, s32 h , s16 T, u8 BAR_STA , u8 MAG_STA)
{
	u8 buff[18],_cnt = 0;
	buff[_cnt++] = BYTE0(x);
	buff[_cnt++] = BYTE1(x);
	buff[_cnt++] = BYTE0(y);
	buff[_cnt++] = BYTE1(y);
	buff[_cnt++] = BYTE0(z);
	buff[_cnt++] = BYTE1(z);
	buff[_cnt++] = BYTE0(h);
	buff[_cnt++] = BYTE1(h);
	buff[_cnt++] = BYTE2(h);
	buff[_cnt++] = BYTE3(h);
	buff[_cnt++] = BYTE0(T);
	buff[_cnt++] = BYTE1(T);
	buff[_cnt++] = BAR_STA;
	buff[_cnt++] = MAG_STA;
	Reported_data(0x02,_cnt,buff);
}

//上报 1个 float 数据
void Reported_one_data(float data)
{
	u8 buff[12],_cnt = 0;
	s32 data_temp = (s32)(data * 100);
	buff[_cnt++] = BYTE0(data_temp);
	buff[_cnt++] = BYTE1(data_temp);
	buff[_cnt++] = BYTE2(data_temp);
	buff[_cnt++] = BYTE3(data_temp);
	Reported_data(0xF1,_cnt,buff);
}




