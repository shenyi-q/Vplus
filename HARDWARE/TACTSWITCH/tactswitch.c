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
extern char message[512];  //串口接收到的字符串


void TASTSWITCH_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE); //使能PORTA,PORTE时钟

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ; //PE0-2
	//上升沿，下拉模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //设置成下拉输入，可以取分高低电平。 上拉输入只能读出高电平
    GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIOE 0 ,1,2
	GPIO_ResetBits(GPIOF,GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);    //置0
}
//大电机的限位开关   上升沿触发？？下降沿
void PA4_Init(void)               //PA6读信号
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PB,PE端口时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_0 | GPIO_Pin_1;				 //PA4 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;     //上拉输入？？下拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA, GPIO_Pin_4 |GPIO_Pin_0 | GPIO_Pin_1);
}

void TACTSWITCH_Init(void)
{

    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    TASTSWITCH_IO_Init();	 //	按键端口初始化
    //GPIO等的时钟已经在对应的.c文件中设置了，这里只需要对外部中断需要用到的复用功能进行时钟使能

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//使能复用功能时钟
}




void PA4_EXTI_Init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    PA4_Init();	 //	按键端口初始化
    //GPIO等的时钟已经在对应的.c文件中设置了，这里只需要对外部中断需要用到的复用功能进行时钟使能
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//使能复用功能时钟
	
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4); //将PA.6和外部中断线6映射起来
    EXTI_InitStructure.EXTI_Line = EXTI_Line4;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;    //上降沿触发??下降沿
    EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	//    //GPIOA.0	  中断线以及中断初始化配置  下降沿触发	//KEY0
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

   //GPIOA.1	  中断线以及中断初始化配置  下降沿触发	//KEY0
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


    
	
	  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//使能按键KEY1所在的外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//使能按键KEY1所在的外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//使能按键KEY0所在的外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}


//外部中断3服务程序   KEY1
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
//		TIM_Cmd(TIM5, DISABLE);  //使能TIM3
//		Beep = 1;
//		delay_ms(100);
//		Beep = 0;
//		runover = 1;
//		printf("1");
//	}
//	EXTI_ClearITPendingBit(EXTI_Line1); 
//}

////大电机的限位开关
//void EXTI4_IRQHandler(void)
//{
//    //下面的执行逻辑可以优化，中断遇到就停，不多逼逼
//	delay_ms(10);
//	if(bigStepMotorSwitch_State_PA4 == 1)
//	{
//		TIM_Cmd(TIM5, DISABLE);  //使能TIM3
//		Beep = 1;
//		delay_ms(100);
//		Beep = 0;
//		runover = 1;
//		printf("2");
//	}
//	EXTI_ClearITPendingBit(EXTI_Line4); //清除LINE3上的中断标志位
//}