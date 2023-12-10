#ifndef _DHT11_H_
#define _DHT11_H_

//单片机头文件
#include "stm32f10x.h"



void DHT11_GPIO_Config(void);
void DHT11_Mode_In(void);
void DHT11_Mode_Out(void);
uint8_t DHT11_Init(void);
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi);
uint8_t DHT11_Read_Byte(void);
uint8_t DHT11_Read_Bit(void);
uint8_t DHT11_Answer_Check(void);
void DHT11_Restart(void);



#endif
