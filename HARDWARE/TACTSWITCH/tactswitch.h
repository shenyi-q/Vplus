#ifndef __TACTSWITCH_H
#define __TACTSWITCH_H
#include "sys.h"
//////////////////

//#define tachswitch1  GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_5) 
//#define tachswitch2  GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_6)
//#define tachswitch3   GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_7)




#define bigStepMotorSwitch_State_PA4 PAin(4)

#define bigStepMotorSwitch_State_PA0 PAin(0)

#define bigStepMotorSwitch_State_PA1 PAin(1)

void TACTSWITCH_Init(void);//外部中断初始化

void PA4_EXTI_Init(void);
void TASTSWITCH_IO_Init(void);


#endif

