

//单片机头文件
#include "stm32f10x.h"

//硬件驱动
#include "key.h"
#include "delay.h"
#include "led.h"



void key_Init(void)
{
	GPIO_InitTypeDef gpio_initstruct;
	EXTI_InitTypeDef exit_initstruct;
	NVIC_InitTypeDef nvic_initstruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//打开GPIOa的时钟 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		//打开EXIT时钟
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1); /* 连接 EXTI 中断源 到 */
	
	gpio_initstruct.GPIO_Mode = GPIO_Mode_IPU;				//设置为上拉输入
	gpio_initstruct.GPIO_Pin = GPIO_Pin_1;						//将初始化的Pin脚
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;				//可承载的最大频率
	
	exit_initstruct.EXTI_Line = EXTI_Line1;		/* 选择 EXTI 中断源 */
	exit_initstruct.EXTI_LineCmd = ENABLE;		 /* 使能中断/事件线 */
	exit_initstruct.EXTI_Mode = EXTI_Mode_Interrupt;		/* 中断模式 */
	exit_initstruct.EXTI_Trigger = 	EXTI_Trigger_Falling;		/* 沿触发 */
	
	nvic_initstruct.NVIC_IRQChannel = EXTI1_IRQn;
	nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;
	nvic_initstruct.NVIC_IRQChannelPreemptionPriority = 2;
	nvic_initstruct.NVIC_IRQChannelSubPriority = 2;
	
	GPIO_Init(GPIOA, &gpio_initstruct);							//初始化GPIO
	EXTI_Init(&exit_initstruct);										//初始化EXIT
	NVIC_Init(&nvic_initstruct);										//初始化NVIC


}



void EXTI1_IRQHandler(void)
{
	DelayXms(10);
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1 == 0))
	{
		if(led_info.Led_Status == LED_ON)
		{
			Led_Set(LED_OFF);
		}else
		{
			Led_Set(LED_ON);
		}
	}
	
	EXTI_ClearITPendingBit(EXTI_Line1);//清除中断标志位

}
