

//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//Ӳ������
#include "key.h"
#include "delay.h"
#include "led.h"



void key_Init(void)
{
	GPIO_InitTypeDef gpio_initstruct;
	EXTI_InitTypeDef exit_initstruct;
	NVIC_InitTypeDef nvic_initstruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//��GPIOa��ʱ�� 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		//��EXITʱ��
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1); /* ���� EXTI �ж�Դ �� */
	
	gpio_initstruct.GPIO_Mode = GPIO_Mode_IPU;				//����Ϊ��������
	gpio_initstruct.GPIO_Pin = GPIO_Pin_1;						//����ʼ����Pin��
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;				//�ɳ��ص����Ƶ��
	
	exit_initstruct.EXTI_Line = EXTI_Line1;		/* ѡ�� EXTI �ж�Դ */
	exit_initstruct.EXTI_LineCmd = ENABLE;		 /* ʹ���ж�/�¼��� */
	exit_initstruct.EXTI_Mode = EXTI_Mode_Interrupt;		/* �ж�ģʽ */
	exit_initstruct.EXTI_Trigger = 	EXTI_Trigger_Falling;		/* �ش��� */
	
	nvic_initstruct.NVIC_IRQChannel = EXTI1_IRQn;
	nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;
	nvic_initstruct.NVIC_IRQChannelPreemptionPriority = 2;
	nvic_initstruct.NVIC_IRQChannelSubPriority = 2;
	
	GPIO_Init(GPIOA, &gpio_initstruct);							//��ʼ��GPIO
	EXTI_Init(&exit_initstruct);										//��ʼ��EXIT
	NVIC_Init(&nvic_initstruct);										//��ʼ��NVIC


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
	
	EXTI_ClearITPendingBit(EXTI_Line1);//����жϱ�־λ

}
