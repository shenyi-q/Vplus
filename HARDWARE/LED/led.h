#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED1 PEout(5)// PE5	

//六组灯光  上下紫外，左右白光，四周低角度
#define UVLight 		PDout(11)   
#define WhiteLight 		PDout(12)
#define LowAngleLight1	PEout(7)         //左上
#define LowAngleLight2  PEout(8)         //左下
#define LowAngleLight3  PEout(9)         //右上
#define LowAngleLight4  PEout(10)					//右下
#define FillLampLeft    PEout(13)              //补光灯左
#define FillLampRight   PEout(14)        //补光灯右


#define READLED0   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)//


void LED_Init(void);//初始化
void LED_Init2(void);
void Flash_Trigger_Init(void);


void UVOpen(void);

void WhiteOpen(void);

void LowAngleLightInit(void);
void LowAngle1Open(void);
void LowAngle2Open(void);
void LowAngle3Open(void);
void LowAngle4Open(void);

void FillLampOpen(void);
void LightClose(void);
	 				    
#endif

