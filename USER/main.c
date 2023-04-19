#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "string.h"
#include "stepmotor_big.h"
#include "stepmotor1.h"
#include "stepmotor2.h"
#include "stepmotor3.h"
#include "tactswitch.h"
#include "wdg.h"
#include <stdlib.h>
#include "main.h"

char message[512];  //串口接收到的字符串

extern u16 count;
//串口
u16 t;
u16 len;
u8 runover = 0;

extern u8 step1;
extern u8 step2;
extern u8 step3;
extern u8 tachswitch1;
extern u8 tachswitch2;
extern u8 tachswitch3;
extern u16 time;


//**************************
//! Global status flags

GLOBAL_FLAGS status = {FALSE, FALSE, 0};
int steps = 1000;// Number of steps to move.
int acceleration = 100; // Accelration to use.
int deceleration = 100;  // Deceleration to use.
int speed = 800; // Speed to use.
char okCmd = FALSE; // Tells if the received string was a valid command.
//****************************************************
void initFunction()		 //各个功能初始化
{
    delay_init();	    	 //延时函数初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    uart_init(4800);	 //串口初始化为4800
    LED_Init();			     //LED端口初始化
    LED_Init2();
    LowAngleLightInit();
    LightClose();
    PA4_EXTI_Init();
    Flash_Trigger_Init();
    KEY_Init();
    PWM_IO_Init();
    TIM3_TIME_Init(4499, 1599);  // 10Hz   //100ms记一次数
    StepMotor1_Init();
    StepMotor2_Init();
    StepMotor3_Init();
    TASTSWITCH_IO_Init();
    TACTSWITCH_Init();
	  
	   //(4*2^4) *625 /40  = 64*6250/40  =10000 = 10S
		//IWDG_Init(4,6250);    //与分频数为64,重载值为625,溢出时间为1s	
    Dir = 1;   //默认向左转

    Beep = 1;
    delay_ms(500);
    Beep = 0;
    //开机找零位写在软件里面进行

}

/*等待电机旋转完毕的函数，每次大电机旋转完毕，需要将runover的值置为1，然后向上位机发送runover的指令
上位机发送旋转电机的指令后就一直判断是否接收到了runover指令，如果接收到了才能进行拍照等操作
*/
void runOver()
{
    while(1)
    {
        //等待电机旋转完毕，发送您旋转完毕的指令
        if(runover != 0)
        {
            runover = 0;
            printf("runover\r\n");
            break;
        }
    }
}


void runZeroWhileForWitch()
{
    //0在零位 ，如果再零位就不转
    if(tachswitch1 != 1)
    {
        StepMotor1_Clockwise_Digree(68);
				
    }

    if(tachswitch2 != 1)
    {
			//顺时针归位
        StepMotor2_Clockwise_Digree(68);
				
    }

    if(tachswitch3 != 1)
    {
        StepMotor3_Clockwise_Digree(68);
				
    }
}

void usartReceiveMessage()
{

    if(USART_RX_STA & 0x8000)
    {
        len = USART_RX_STA & 0x3fff; //得到此次接收到的数据长度
        //printf("\r\n您发送的消息为:\r\n\r\n");

        for(t = 0; t < len; t++)
        {
            sprintf(&message[t], "%c", USART_RX_BUF[t]);
        }
        if(message[0] == 'r')		//r为旋转大电机，r0向右转，r1向左转
        {
            u16 value = 0;
            u8 message_length =  strlen(message);
            if(message_length == 3)
            {
                value = message[2] - '0';

            }
            else if(message_length == 4)
            {
                value = (message[2] - '0') * 10 + (message[3] - '0');
            }
            else if(message_length == 5)
            {
                value = (message[2] - '0') * 100 + (message[3] - '0') * 10 + (message[4] - '0');
            }
            Dir = message[1] - '0';
            //如果已经处于零位，且还是向左转，就不要旋转了，代表已经处于零位。
//            if(bigStepMotorSwitch_State_PA0 == 0)
////							|| bigStepMotorSwitch_State_PA1 == 0 || bigStepMotorSwitch_State_PA4 == 0)
////							&& (Dir == 1))
//            {
//                runover = 1;
//            }
//            else
//            {
                runover = 0;
                speed_cntr_Move(value, 20, 20, 2000);
//            }

            okCmd = TRUE;
            runOver();
        }

        if(message[0] == 'S')		//S1-3旋转1-3号小电机，S0为归位
        {
            u16 value = 0;
            if(message[1] != '0')
            {
                value = (message[2] - '0') * 10 + (message[3] - '0');
            }
            runZeroWhileForWitch();

            switch(message[1])
            {
                case '0':
                    break;

                case '1':
                    StepMotor1_AntiClockwise_Digree(value);
                    break;

                case '2':
                    StepMotor2_AntiClockwise_Digree(value);
                    break;

                case '3':
                    StepMotor3_AntiClockwise_Digree(value);
                    break;
            }
            printf("stepover\r\n");
        }

        if(strcmp(message, "WhiteOpen") == 0)
        {
            WhiteOpen();
        }

        if(strcmp(message, "UVOpen") == 0)
        {
            UVOpen();
        }

        if(strcmp(message, "LightClose") == 0 )
        {
            LightClose();
        }

        if(strcmp(message, "FillInOpen") == 0)
        {
            FillLampOpen();

        }

        if(strcmp(message, "OL") == 0)
        {
            LowAngle2Open();
        }

        if(strcmp(message, "FL") == 0)
        {
            FillLampLeft = 1;
        }

        if(strcmp(message, "FR") == 0)
        {
            FillLampRight = 1;
        }

        if(strcmp(message, "TopOpen") == 0)			//上面两个低角度灯
        {
            LowAngle1Open();
            LowAngle3Open();
        }

        if(strcmp(message, "BottomOpen") == 0) 	//下面两个低角度灯
        {
            LowAngle2Open();
            LowAngle4Open();
        }

        if(strcmp(message, "LeftOpen") == 0)		//左边两个低角度灯
        {
            LowAngle1Open();
            LowAngle2Open();
        }

        if(strcmp(message, "RightOpen") == 0)		//右边两个低角度灯
        {
            LowAngle3Open();
            LowAngle4Open();
        }

        memset(message, 0, sizeof message);                          //清空数组
        USART_RX_STA = 0;
    }
}

int main(void)
{
    initFunction();
    while(1)
    {
        usartReceiveMessage();

    }
}


