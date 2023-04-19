#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//��ʱ�� ��������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/4
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************


#define  ROTATIONB PAout(6)  // PA6	
void TIM4_PWM_Init(u16 arr, u16 psc);

void STOP_ROTATION(char c);
void START_ROTATION(char c);



void TIM3_TIME_Init(u16 arr, u16 psc);

void TIM4_Int_Init(u16 arr, u16 psc);
void TIM5_Int_Init(u16 arr, u16 psc);
void startRotation(void);
void startRotationWithMessage(u16 step,u8 dir,u8 speed);
void stopRotation(void);

void changeDirection(u8 direction);

void changeSpeed(void);


#endif
