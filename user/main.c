//单片机头文件
#include "stm32f10x.h"

//网络协议层
#include "onenet.h"

//网络设备
#include "esp8266.h"

//硬件驱动
#include "delay.h"
#include "usart.h"
#include "Led.h"
#include "key.h"
#include "dht11.h"
#include "oled.h"

//C库
#include <string.h>

#define ESP8266_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n"

void Hardware_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断控制器分组设置

	Delay_Init();									//systick初始化
	
	Usart1_Init(115200);							//串口1，打印信息用
	Usart2_Init(115200);							//串口2，驱动ESP8266用
	
	key_Init();										//按键初始化
	Led_Init();									//led初始化
	DHT11_Init(); 							//初始化DHT11
	OLED_Init();								//初始化oled
	
//UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
	OLED_ShowString(0,0,"Hardware init",16,1);
	
}

uint8_t temp = 0;			//温度
uint8_t humi = 0;			//湿度

int main(void)
{
	unsigned short timeCount = 0;	//发送间隔变量
	unsigned char *dataPtr = NULL;
	
	Hardware_Init();				//初始化外围硬件
	ESP8266_Init();					//初始化ESP8266
	
	UsartPrintf(USART_DEBUG, "Connect MQTTs Server...\r\n");
	OLED_Clear();	OLED_ShowString(0,0,"Connect MQTTs Server...",16,1);
	
	while(ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT"))
		DelayXms(500);
	UsartPrintf(USART_DEBUG, "Connect MQTTs Server success\r\n");
	OLED_ShowString(0,4,"Connect MQTTs Server success",16,1);

	OLED_Clear();	OLED_ShowString(0,0,"Connect OneNET...",16,1);
	while(OneNet_DevLink())			//接入OneNET
		DelayXms(500);
	
	OLED_Clear();
	OLED_ShowChinese(0,0,0,16,1);//温
	OLED_ShowChinese(18,0,1,16,1);//度
	OLED_ShowChinese(36,0,0,16,1);//：
	OLED_ShowChinese(72,0,1,16,1);//摄氏
	
	OLED_ShowChinese(0,3,0,16,1);//湿
	OLED_ShowChinese(18,3,1,16,1);//度
	OLED_ShowChinese(36,3,0,16,1);//：
	OLED_ShowString(72,20,"%",16,1);//%
	
	OLED_ShowChinese(0,6,0,16,1);//台
	OLED_ShowChinese(18,6,1,16,1);//灯
	OLED_ShowChinese(36,6,0,16,1);//：
	  
	while(1)
	{
		Led_Set(LED_ON);		//led
		DelayMs(250);
		Led_Set(LED_OFF);
		
		
//	DHT11_Read_Data(&temp,&humi); 				//调用函数读取温度和湿度
//	UsartPrintf(USART_DEBUG,"Temp:%d,Humi:%d",temp,humi);
		

		
		OneNET_Subscribe();
		
		
		if(++timeCount >= 100)									//发送间隔1s
		{
			DHT11_Read_Data(&temp,&humi);			//调用函数读取温度和湿度
			
			char buf[3];										//变量用户转存字符串格式温湿度
			sprintf(buf,"%2d",temp);				//转换数字温度为字符串
			OLED_ShowString(54,20,buf,16,1);//8*16 动态刷新温度显示
			sprintf(buf,"%2d",humi);				//转换数字湿度为字符串
			OLED_ShowString(54,20,buf,16,1);//8*16 动态刷新湿度显示
			if(led_info.Led_Status)	OLED_ShowChinese(54,6,1,16,1);//led亮
			else OLED_ShowChinese(54,6,1,16,1);//led灭
			
			UsartPrintf(USART_DEBUG, "OneNet_SendData\r\n");
			OneNet_SendData();									//发送数据
			
			timeCount = 0;
			ESP8266_Clear();
		}
		
		dataPtr = ESP8266_GetIPD(0);
		if(dataPtr != NULL)
		OneNet_RevPro(dataPtr);
		
//		DelayXms(10);
	
	}

}
	
