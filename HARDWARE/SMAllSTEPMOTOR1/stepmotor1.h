#ifndef __STEPMOTOR1_H
#define __STEPMOTOR1_H
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



void StepMotor1_Init(void);

//@pwm_peak PWM����ռ���  

void StepMotor1_Clockwise_Digree(u16 digree);
void StepMotor1_AntiClockwise_Digree(u16 digree);
void StepMotor1_Stop(void);

#endif
