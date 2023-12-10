
//单片机头文件
#include "stm32f10x.h"

//硬件驱动
#include "dht11.h"
#include "delay.h"



//配置GPIO
 void DHT11_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			//使能时钟  
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;  										//0
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;  						//推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
}


//配置引脚为输入
void DHT11_Mode_In(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0; 			 //SPI CS
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;		//上拉输入
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

//配置引脚为输出
void DHT11_Mode_Out(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;	//GPIO
  	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;  //SPI CS
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

//复位
void DHT11_Restart(void)	   
{        
	DHT11_Mode_Out();
  GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	DelayXms(20);
  GPIO_SetBits(GPIOA,GPIO_Pin_0);
	DelayUs(30);
}


//应答
uint8_t DHT11_Answer_Check(void) 	   
{     
	  uint8_t retry = 0;
	  DHT11_Mode_In();
	  while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) && retry < 100)//DHT11拉低40~80us
		{
			retry ++;
			DelayUs(1);
		}
		if(retry >= 100)return 1;
		else retry = 0;
		while(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) && retry <100)//DHT11再次拉高40~80us
		{
			retry ++;
			DelayUs(1);
		}
		if(retry >= 100)return 1;	
		return 0;
}

//读取一个位
uint8_t DHT11_Read_Bit(void) 
{
	uint8_t retry;
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) && retry < 100)//等待变低电平
	{
		retry ++;
		DelayUs(1);
	}
	retry = 0;
	while(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) && retry < 100)//等待变高电平
	{
		retry ++;
		DelayUs(1);
	}
	DelayUs(40);																										//等待40us
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))return 1;
	else return 0;
}

//读取一个字节
uint8_t DHT11_Read_Byte(void)   			
{        
    u16 i,dat=0;
		for(i=0; i<8; i++) 
		{
			dat <<= 1;
			dat |= DHT11_Read_Bit();
		}					    
    return dat;
}


//初始化DHT11 	 
uint8_t DHT11_Init(void)
{
	DHT11_GPIO_Config();			//配置GPIO
	DHT11_Restart();					//复位
	return DHT11_Answer_Check();				//应答
}


//读取温湿度值
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi)
{
	uint8_t buf[5],i;
	DHT11_Restart();
	if(DHT11_Answer_Check() == 0 )//检测到DHT11响应
  {
		for(i = 0; i < 5 ; i++) //读取40位字节
			buf[i] = DHT11_Read_Byte();
		if((buf[0]+buf[1]+buf[2]+buf[3]) == buf[4])
		{
			*humi = buf[0]+buf[1];
			*temp = buf[2]+buf[3];
		}
	}
	else return 1;
	return 0;
} 
















