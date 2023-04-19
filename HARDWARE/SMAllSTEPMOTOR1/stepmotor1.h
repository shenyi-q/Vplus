#ifndef __STEPMOTOR1_H
#define __STEPMOTOR1_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//定时器 驱动代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/4
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************



void StepMotor1_Init(void);

//@pwm_peak PWM波的占峰比  

void StepMotor1_Clockwise_Digree(u16 digree);
void StepMotor1_AntiClockwise_Digree(u16 digree);
void StepMotor1_Stop(void);

#endif
