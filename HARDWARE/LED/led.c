#include "led.h"
#include "delay.h"
#include "key.h"

//初始化PB5和PE5为输出口.并使能这两个口的时钟
//LED IO初始化

/*
该.c文件说明

1、闪光灯控制 ：
	上下一组是IO口PD11
	左右一组是IO口PD12
	外围四组 IO PF 4,5,6,7
	共6组，需要提供6个函数
2、led灯的指示
*/

void LED_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB,PE端口时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 端口配置

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;     //下拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    //**************************************************
}
void LED_Init2(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 //使能PB,PE端口时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 端口配置

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1-->PE.5 端口配置, 推挽输出
    GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
//GPIO_SetBits(GPIOE,GPIO_Pin_5); 						 //PE.5 输出高
    GPIO_ResetBits(GPIOE, GPIO_Pin_5);

}


void LowAngleLightInit(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);	 //使能PF端口时钟
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	
	//GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;	  
	 GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz					 //PE.5 输出高
	 GPIO_ResetBits(GPIOE,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14);
}


void Flash_Trigger_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOF, ENABLE);	 //使能PD,PF端口时钟
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     //输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;				 //PD11 - 12 端口配置
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOD, GPIO_Pin_11 | GPIO_Pin_12);
	
	//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;	  
	GPIO_Init(GPIOF, &GPIO_InitStructure);	  				
	GPIO_ResetBits(GPIOF , GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
} 

//
//需要一个函数，当一个输出
void UVOpen(void)
{
	UVLight = 1;
}

void WhiteOpen(void){
	
	WhiteLight = 1;
}

void FillLampOpen(){

	FillLampLeft = 1;
	FillLampRight = 1;
}


void LowAngle1Open(void)
{
	LowAngleLight1 = 1;	
}
void LowAngle2Open(void)
{
	
	LowAngleLight2 = 1;
}
void LowAngle3Open(void)
{

	LowAngleLight3 = 1;
	//LightClose();
}
void LowAngle4Open(void)
{
	

	LowAngleLight4 = 1;
	//LightClose();
}

void LightClose(void)
{
	if(FillLampLeft ==1){
		FillLampLeft = 0;
	}
	if(FillLampRight ==1){
		FillLampRight = 0;
	}
	if(	UVLight == 1){
		UVLight = 0;
	}
	if(	WhiteLight == 1)
	{
		WhiteLight = 0;
	}	
	if(LowAngleLight1 == 1){
		LowAngleLight1 = 0;
	}
	if(LowAngleLight2 ==1){
		LowAngleLight2 = 0;
	}
	if(LowAngleLight3 == 1){
		LowAngleLight3 = 0;
	}
	if(LowAngleLight4 == 1){
		LowAngleLight4 = 0;
	}
	
	
}



