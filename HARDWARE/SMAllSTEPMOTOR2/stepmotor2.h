#ifndef __STEPMOTOR2_H
#define __STEPMOTOR2_H

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



void StepMotor2_Init(void);

void StepMotor2_Clockwise_Digree(u16 digree);
void StepMotor2_AntiClockwise_Digree(u16 digree);

void StepMotor2_Stop(void);
#endif
