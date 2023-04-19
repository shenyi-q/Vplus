#include "timer.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTE��ӢSTM32������
//��ʱ�� ��������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/4
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.1 20120904
//1,����TIM3_PWM_Init������
//2,����LED0_PWM_VAL�궨�壬����TIM3_CH2����
//////////////////////////////////////////////////////////////////////////////////

//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!

static u32 countA;							//�������
static u16 PWM_PULSE_COUNTA = 0;

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;


//TIM3 PWM���ֳ�ʼ��
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM3_PWM_Init(u16 arr, u16 psc)                        //TIM3���ƴ����ٱȵĵ��
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
    //PA7������      PA5������������ת
    //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	GPIOA.7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM_CH2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO

    //PA5/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //PA.5 �˿�����, �������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //��ʼ��TIM3
    TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    //��ʼ��TIM3 Channel2 PWMģʽ
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
    TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3

}

void ROTATION_ANGLE(char c, u16 angle, u16 pwm_peak)
{
    START_ROTATION(c);
    countA = 0;

    if(c  == 'A')
    {

        switch(angle)
        {
            case 45:
                PWM_PULSE_COUNTA = 100;
                break;

            case 90:
                PWM_PULSE_COUNTA  = 200;
                break;

            case 180:
                PWM_PULSE_COUNTA = 400;
                break;

            case 360:
                PWM_PULSE_COUNTA = 800;
                break;

        }

        TIM_SetCompare2(TIM3, pwm_peak);
    }

}

void START_ROTATION(char c)
{
    if(c == 'A')
    {
        TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
    }
    else if(c == 'B')
    {
        TIM_Cmd(TIM4, ENABLE);
    }

}

void STOP_ROTATION(char c)
{
    if(c == 'A')
    {
        TIM_Cmd(TIM3, DISABLE);  //ʹ��TIM3
    }
    else if(c == 'B')
    {
        TIM_Cmd(TIM4, DISABLE);
    }
}

//������0��1
void CHANGE_DIRECTION(u8 direction)
{
    //Ĭ����ת
    ROTATION = direction;
}

void CHANGE_DIRECTIONB(u8 direction)
{
//Ĭ����ת
    ROTATIONB = direction;
}




//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
//	//ÿ200hz�����һ���жϼ�һ��������������200������200�����壬��ת��90�ȣ�����һ������ת��90��
//    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ
//    {
//        countA++;
//		//�ж����治��Ƕ���ж�
//		//printf("�ж������count = %d\n",count);
//        if(countA >= (PWM_PULSE_COUNTA * 80))
//        {
//            countA = 0;
//			STOP_ROTATION('A');
//        }
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ
//    }

    //��Ҫд���μ���,ÿ�μ�������һ�����壬������400ϸ��.���ٱ�80����Ҫ����32000תһȦ
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        countA++;


//        if((countA * 1) <= 16000)
//        {

//            TIM_Cmd(TIM3, DISABLE);
//            TIM_PrescalerConfig(TIM3, (800 / (countA /  ) ), TIM_PSCReloadMode_Immediate);
//            TIM_Cmd(TIM3, ENABLE);
//        }
        if(countA >= (32000 / 6))
        {

            //תһȦ
            STOP_ROTATION('A');
        }

        TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ

    }
}

