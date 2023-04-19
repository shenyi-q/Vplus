#ifndef __TIMER_H
#define __TIMER_H
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
