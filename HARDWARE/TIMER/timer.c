#include "timer.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "stepmotor_big.h"
#include "main.h"
#include "sys.h"

/*
Time5：新的计算方法， 翻转IO口，负责计算步数

Time4 负责定时计算速度
Time3：旧的方法

*/

//通用定时器4中断初始化，计算速度
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数


extern u8 runover; 
u16 time;; //值代表100ms

void TIM3_TIME_Init(u16 arr, u16 psc)                        //TIM3控制带减速比的电机
{
    
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3时钟
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟

    //初始化TIM3
    TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE ); //使能指定的TIM3中断,允许更新中断
   
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
    TIM_Cmd(TIM3, DISABLE);  //使能TIM3
}



void TIME3_START_WITHTIME(void){
	time =0;
	//开始计时
	TIM_Cmd(TIM3, ENABLE);
}

void TIME3_CLOSE_WITHTIME(void){
	//停止计时
	TIM_Cmd(TIM3,DISABLE);
}


void TIM4_Int_Init(u16 arr, u16 psc)
{
    RCC->APB1ENR |= 1 << 2;	//TIM4时钟使能
    TIM4->ARR = arr;  		//设定计数器自动重装值
    TIM4->PSC = psc;  		//预分频器设置
    TIM4->DIER |= 1 << 0; //允许更新中断
    TIM4->CR1 |= 0x01;  //使能定时器4
    MY_NVIC_Init(1, 3, TIM4_IRQn, 2); //抢占1，子优先级3，组2
}
void TIM5_Int_Init(u16 arr, u16 psc)
{
    RCC->APB1ENR |= 1 << 3;	//TIM4时钟使能
    TIM5->ARR = arr;  		//设定计数器自动重装值
    TIM5->PSC = psc;  		//预分频器设置
    TIM5->DIER |= 1 << 0; //允许更新中断
    TIM5->CR1 |= 0x01;  //使能定时器5
    MY_NVIC_Init(1, 3, TIM5_IRQn, 2); //抢占1，子优先级3，组2
}

//每10ms计算一次速度
void MotorCalculateSpeed()
{
    switch(srd.run_state)
    {
        case STOP:
            status.running = FALSE;
            break;

        case ACCEL:
            srd.currentSpeed += srd.speedAdd;

            if(srd.currentSpeed > srd.maxSpeed)
            {
                srd.currentSpeed = srd.maxSpeed;
            }

            break;

        case RUN:
            srd.currentSpeed = srd.maxSpeed;
            break;

        case DECEL:

            srd.currentSpeed -= srd.speedAdd;
			//如果速度已经达到最低速，就设置为最低速
            if(srd.currentSpeed < srd.decelSpeed){
				
				 srd.currentSpeed = srd.decelSpeed;
			}
            break;
    }
}
//设置定时器值，用来产生指定频率的脉冲信号
void Motor0TimeSet(unsigned int ARR)
{	
	TIM5->ARR = (uint16_t)ARR;   
	//if(TimeEnable) TIM_Cmd(TIM3, ENABLE);
	//else TIM_Cmd(TIM3, DISABLE);
}

void MotorRunState()
{	
    switch(srd.run_state)
    {
		case STOP://开始
		break;
		case ACCEL://加速状态
			
			//如果步数已经达到了减速的步数，就直接减速
			//如果没有，就判断速度
			if(srd.stepCount >= srd.decel_start){
				srd.run_state = DECEL;
			}else if(srd.currentSpeed >= srd.maxSpeed){
				srd.run_state = RUN;
			}
		break;
		case RUN://最大速度阶段
			if(srd.stepCount >= srd.decel_start){
				srd.run_state = DECEL;
			}
		break;
		case DECEL://减速阶段
			if(srd.stepCount >= srd.maxStep)//脉冲输出完成
			{
                TIM5->CR1 &= 0xFE;;//关闭定时器，停止脉冲信号输出
				srd.run_state = STOP;
				//串口发送旋转完成的标志
				runover = 1;
			}else{
				
			}
			break;
	}
	if(srd.run_state != STOP){
		Motor0TimeSet(72*1000000 /150 /srd.currentSpeed);
	}
		
 }


//用来计算运行时间，防止死循环一直执行。  500ms执行一次中断，
  void TIM3_IRQHandler(void)
{
    if(TIM3->SR & 0X0001) //溢出中断
    {
		time++;
		
    }

    TIM3->SR &= ~(1 << 0); //清除中断标志位
}



//定时器4中断服务程序
void TIM4_IRQHandler(void)
{
    if(TIM4->SR & 0X0001) //溢出中断
    {
        MotorCalculateSpeed();
    }

    TIM4->SR &= ~(1 << 0); //清除中断标志位
}

void TIM5_IRQHandler(void)
{
    if(TIM5->SR & 0X0001) //溢出中断
    {
          //翻转IO口
        PA7_PWM = !PA7_PWM;
		srd.stepCount++;
		MotorRunState();
    }

    TIM5->SR &= ~(1 << 0); //清除中断标志位
}



