
//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//Ӳ������
#include "dht11.h"
#include "delay.h"



//����GPIO
 void DHT11_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			//ʹ��ʱ��  
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;  										//0
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;  						//�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
}


//��������Ϊ����
void DHT11_Mode_In(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0; 			 //SPI CS
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;		//��������
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

//��������Ϊ���
void DHT11_Mode_Out(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;	//GPIO
  	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;  //SPI CS
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

//��λ
void DHT11_Restart(void)	   
{        
	DHT11_Mode_Out();
  GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	DelayXms(20);
  GPIO_SetBits(GPIOA,GPIO_Pin_0);
	DelayUs(30);
}


//Ӧ��
uint8_t DHT11_Answer_Check(void) 	   
{     
	  uint8_t retry = 0;
	  DHT11_Mode_In();
	  while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) && retry < 100)//DHT11����40~80us
		{
			retry ++;
			DelayUs(1);
		}
		if(retry >= 100)return 1;
		else retry = 0;
		while(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) && retry <100)//DHT11�ٴ�����40~80us
		{
			retry ++;
			DelayUs(1);
		}
		if(retry >= 100)return 1;	
		return 0;
}

//��ȡһ��λ
uint8_t DHT11_Read_Bit(void) 
{
	uint8_t retry;
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) && retry < 100)//�ȴ���͵�ƽ
	{
		retry ++;
		DelayUs(1);
	}
	retry = 0;
	while(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) && retry < 100)//�ȴ���ߵ�ƽ
	{
		retry ++;
		DelayUs(1);
	}
	DelayUs(40);																										//�ȴ�40us
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))return 1;
	else return 0;
}

//��ȡһ���ֽ�
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


//��ʼ��DHT11 	 
uint8_t DHT11_Init(void)
{
	DHT11_GPIO_Config();			//����GPIO
	DHT11_Restart();					//��λ
	return DHT11_Answer_Check();				//Ӧ��
}


//��ȡ��ʪ��ֵ
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi)
{
	uint8_t buf[5],i;
	DHT11_Restart();
	if(DHT11_Answer_Check() == 0 )//��⵽DHT11��Ӧ
  {
		for(i = 0; i < 5 ; i++) //��ȡ40λ�ֽ�
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
















