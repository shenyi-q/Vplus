#include "delay.h"
#include "stepmotor_big.h"
#include "delay.h"
#include "time.h"


/*
1、使用定时器进行IO口的反转产生波形
*/
speedRampData srd;
/*
0 : stop
1 : accel 加速
2 : decel 减速
3 : run   峰值正常运行
*/

void TIM4_Int_Init(u16 arr, u16 psc);
void TIM5_Int_Init(u16 arr, u16 psc);

void PWM_IO_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	 //PA7产生波      PA5用于设置正反转
   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形	GPIOA.7
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO

    //PA5/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //PA.5 端口配置, 推挽输出
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void speed_cntr_Move(signed int angle, unsigned int accel, unsigned int decel, unsigned int speed){
	
	signed int step = 128000*angle/3600;
	srd.maxStep = step;
	srd.startSpeed = 100;  //100HZ开始加，加到2000Hz    最大先设置为1000hz
	if(step>1500){
		srd.decel_start = step-1500;
	}else{
		srd.decel_start = step;
	}
	srd.currentSpeed = srd.startSpeed;
	srd.decelSpeed = 500;
	srd.speedAdd = accel;
	srd.maxSpeed = speed;
	srd.stepCount = 0;
	srd.run_state = ACCEL;
	
	//72000000/45 = 1600000/16  = 100KHZ  ,启动频率10K HZ
	TIM4_Int_Init(449, 1599);   //100hz = 10ms //speed
	TIM5_Int_Init(4499,159);  //IO  100hz = 10ms

}





