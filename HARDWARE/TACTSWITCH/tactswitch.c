#include "tactswitch.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "stepmotor1.h"
#include "stepmotor2.h"
#include "stepmotor3.h"
#include "stepmotor_big.h"
#include "string.h"

extern u8 step1;
extern u8 step2;
extern u8 step3;
extern u8 runover;
extern char message[512];  //���ڽ��յ����ַ���


void TASTSWITCH_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE); //ʹ��PORTA,PORTEʱ��

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ; //PE0-2
	//�����أ�����ģʽ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó��������룬����ȡ�ָߵ͵�ƽ�� ��������ֻ�ܶ����ߵ�ƽ
    GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��GPIOE 0 ,1,2
	GPIO_ResetBits(GPIOF,GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);    //��0
}
//��������λ����   �����ش��������½���
void PA4_Init(void)               //PA6���ź�
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_0 | GPIO_Pin_1;				 //PA4 �˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;     //�������룿����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA, GPIO_Pin_4 |GPIO_Pin_0 | GPIO_Pin_1);
}

void TACTSWITCH_Init(void)
{

    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    TASTSWITCH_IO_Init();	 //	�����˿ڳ�ʼ��
    //GPIO�ȵ�ʱ���Ѿ��ڶ�Ӧ��.c�ļ��������ˣ�����ֻ��Ҫ���ⲿ�ж���Ҫ�õ��ĸ��ù��ܽ���ʱ��ʹ��

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//ʹ�ܸ��ù���ʱ��
}




void PA4_EXTI_Init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    PA4_Init();	 //	�����˿ڳ�ʼ��
    //GPIO�ȵ�ʱ���Ѿ��ڶ�Ӧ��.c�ļ��������ˣ�����ֻ��Ҫ���ⲿ�ж���Ҫ�õ��ĸ��ù��ܽ���ʱ��ʹ��
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//ʹ�ܸ��ù���ʱ��
	
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4); //��PA.6���ⲿ�ж���6ӳ������
    EXTI_InitStructure.EXTI_Line = EXTI_Line4;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;    //�Ͻ��ش���??�½���
    EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	//    //GPIOA.0	  �ж����Լ��жϳ�ʼ������  �½��ش���	//KEY0
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

   //GPIOA.1	  �ж����Լ��жϳ�ʼ������  �½��ش���	//KEY0
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


    
	
	  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//ʹ�ܰ���KEY1���ڵ��ⲿ�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//�����ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//ʹ�ܰ���KEY1���ڵ��ⲿ�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//�����ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//ʹ�ܰ���KEY0���ڵ��ⲿ�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//�����ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}


//�ⲿ�ж�3�������   KEY1
void EXTI0_IRQHandler(void)
{
	delay_ms(10);
	if(bigStepMotorSwitch_State_PA0 == 0)
	{
		TIM_Cmd(TIM5, DISABLE); 
		Beep = 1;
		delay_ms(500);
		Beep = 0;
		runover = 1;
		printf("0");
	}
	EXTI_ClearITPendingBit(EXTI_Line0); 
}

//void EXTI1_IRQHandler(void)
//{
//   delay_ms(10);
//	if(bigStepMotorSwitch_State_PA1 == 1)
//	{
//		TIM_Cmd(TIM5, DISABLE);  //ʹ��TIM3
//		Beep = 1;
//		delay_ms(100);
//		Beep = 0;
//		runover = 1;
//		printf("1");
//	}
//	EXTI_ClearITPendingBit(EXTI_Line1); 
//}

////��������λ����
//void EXTI4_IRQHandler(void)
//{
//    //�����ִ���߼������Ż����ж�������ͣ������Ʊ�
//	delay_ms(10);
//	if(bigStepMotorSwitch_State_PA4 == 1)
//	{
//		TIM_Cmd(TIM5, DISABLE);  //ʹ��TIM3
//		Beep = 1;
//		delay_ms(100);
//		Beep = 0;
//		runover = 1;
//		printf("2");
//	}
//	EXTI_ClearITPendingBit(EXTI_Line4); //���LINE3�ϵ��жϱ�־λ
//}