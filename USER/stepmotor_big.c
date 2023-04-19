#include "timer.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTE精英STM32开发板
//定时器 驱动代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/4
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.1 20120904
//1,增加TIM3_PWM_Init函数。
//2,增加LED0_PWM_VAL宏定义，控制TIM3_CH2脉宽
//////////////////////////////////////////////////////////////////////////////////

//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!

static u32 countA;							//脉冲计数
static u16 PWM_PULSE_COUNTA = 0;

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;


//TIM3 PWM部分初始化
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM3_PWM_Init(u16 arr, u16 psc)                        //TIM3控制带减速比的电机
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
    //PA7产生波      PA5用于设置正反转
    //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形	GPIOA.7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM_CH2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO

    //PA5/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //PA.5 端口配置, 推挽输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //初始化TIM3
    TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    //初始化TIM3 Channel2 PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE ); //使能指定的TIM3中断,允许更新中断
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
    TIM_Cmd(TIM3, ENABLE);  //使能TIM3

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
        TIM_Cmd(TIM3, ENABLE);  //使能TIM3
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
        TIM_Cmd(TIM3, DISABLE);  //使能TIM3
    }
    else if(c == 'B')
    {
        TIM_Cmd(TIM4, DISABLE);
    }
}

//参数传0或1
void CHANGE_DIRECTION(u8 direction)
{
    //默认正转
    ROTATION = direction;
}

void CHANGE_DIRECTIONB(u8 direction)
{
//默认正转
    ROTATIONB = direction;
}




//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
//	//每200hz会产生一个中断记一次数，这样计数200个就是200个脉冲，旋转了90度，且是一秒内旋转了90度
//    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源
//    {
//        countA++;
//		//中断里面不能嵌套中断
//		//printf("中断里面的count = %d\n",count);
//        if(countA >= (PWM_PULSE_COUNTA * 80))
//        {
//            countA = 0;
//			STOP_ROTATION('A');
//        }
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源
//    }

    //需要写梯形加速,每次计数都是一个脉冲，现在是400细分.减速比80，需要计数32000转一圈
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

            //转一圈
            STOP_ROTATION('A');
        }

        TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源

    }
}

