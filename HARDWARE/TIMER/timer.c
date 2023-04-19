#include "timer.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "stepmotor_big.h"
#include "main.h"
#include "sys.h"

/*
Time5���µļ��㷽���� ��תIO�ڣ�������㲽��

Time4 ����ʱ�����ٶ�
Time3���ɵķ���

*/

//ͨ�ö�ʱ��4�жϳ�ʼ���������ٶ�
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��


extern u8 runover; 
u16 time;; //ֵ����100ms

void TIM3_TIME_Init(u16 arr, u16 psc)                        //TIM3���ƴ����ٱȵĵ��
{
    
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��

    //��ʼ��TIM3
    TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
   
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
    TIM_Cmd(TIM3, DISABLE);  //ʹ��TIM3
}



void TIME3_START_WITHTIME(void){
	time =0;
	//��ʼ��ʱ
	TIM_Cmd(TIM3, ENABLE);
}

void TIME3_CLOSE_WITHTIME(void){
	//ֹͣ��ʱ
	TIM_Cmd(TIM3,DISABLE);
}


void TIM4_Int_Init(u16 arr, u16 psc)
{
    RCC->APB1ENR |= 1 << 2;	//TIM4ʱ��ʹ��
    TIM4->ARR = arr;  		//�趨�������Զ���װֵ
    TIM4->PSC = psc;  		//Ԥ��Ƶ������
    TIM4->DIER |= 1 << 0; //��������ж�
    TIM4->CR1 |= 0x01;  //ʹ�ܶ�ʱ��4
    MY_NVIC_Init(1, 3, TIM4_IRQn, 2); //��ռ1�������ȼ�3����2
}
void TIM5_Int_Init(u16 arr, u16 psc)
{
    RCC->APB1ENR |= 1 << 3;	//TIM4ʱ��ʹ��
    TIM5->ARR = arr;  		//�趨�������Զ���װֵ
    TIM5->PSC = psc;  		//Ԥ��Ƶ������
    TIM5->DIER |= 1 << 0; //��������ж�
    TIM5->CR1 |= 0x01;  //ʹ�ܶ�ʱ��5
    MY_NVIC_Init(1, 3, TIM5_IRQn, 2); //��ռ1�������ȼ�3����2
}

//ÿ10ms����һ���ٶ�
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
			//����ٶ��Ѿ��ﵽ����٣�������Ϊ�����
            if(srd.currentSpeed < srd.decelSpeed){
				
				 srd.currentSpeed = srd.decelSpeed;
			}
            break;
    }
}
//���ö�ʱ��ֵ����������ָ��Ƶ�ʵ������ź�
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
		case STOP://��ʼ
		break;
		case ACCEL://����״̬
			
			//��������Ѿ��ﵽ�˼��ٵĲ�������ֱ�Ӽ���
			//���û�У����ж��ٶ�
			if(srd.stepCount >= srd.decel_start){
				srd.run_state = DECEL;
			}else if(srd.currentSpeed >= srd.maxSpeed){
				srd.run_state = RUN;
			}
		break;
		case RUN://����ٶȽ׶�
			if(srd.stepCount >= srd.decel_start){
				srd.run_state = DECEL;
			}
		break;
		case DECEL://���ٽ׶�
			if(srd.stepCount >= srd.maxStep)//����������
			{
                TIM5->CR1 &= 0xFE;;//�رն�ʱ����ֹͣ�����ź����
				srd.run_state = STOP;
				//���ڷ�����ת��ɵı�־
				runover = 1;
			}else{
				
			}
			break;
	}
	if(srd.run_state != STOP){
		Motor0TimeSet(72*1000000 /150 /srd.currentSpeed);
	}
		
 }


//������������ʱ�䣬��ֹ��ѭ��һֱִ�С�  500msִ��һ���жϣ�
  void TIM3_IRQHandler(void)
{
    if(TIM3->SR & 0X0001) //����ж�
    {
		time++;
		
    }

    TIM3->SR &= ~(1 << 0); //����жϱ�־λ
}



//��ʱ��4�жϷ������
void TIM4_IRQHandler(void)
{
    if(TIM4->SR & 0X0001) //����ж�
    {
        MotorCalculateSpeed();
    }

    TIM4->SR &= ~(1 << 0); //����жϱ�־λ
}

void TIM5_IRQHandler(void)
{
    if(TIM5->SR & 0X0001) //����ж�
    {
          //��תIO��
        PA7_PWM = !PA7_PWM;
		srd.stepCount++;
		MotorRunState();
    }

    TIM5->SR &= ~(1 << 0); //����жϱ�־λ
}



