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

char message[512];  //���ڽ��յ����ַ���

extern u16 count;
//����
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
const u16 step;
int emb_dir = 1;
int emb_pos = 1;

//****************************************************
void initFunction()		 //�������ܳ�ʼ��
{
    delay_init();	    	 //��ʱ������ʼ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    uart_init(4800);	 //���ڳ�ʼ��Ϊ4800
    LED_Init();			     //LED�˿ڳ�ʼ��
    LED_Init2();
    LowAngleLightInit();
    LightClose();
    PA4_EXTI_Init();
    Flash_Trigger_Init();
    KEY_Init();
    PWM_IO_Init();
    TIM3_TIME_Init(4499, 1599);  // 10Hz   //100ms��һ����
    StepMotor1_Init();
    StepMotor2_Init();
    StepMotor3_Init();
    TASTSWITCH_IO_Init();
    TACTSWITCH_Init();
	  
	   //(4*2^4) *625 /40  = 64*6250/40  =10000 = 10S
		//IWDG_Init(4,6250);    //���Ƶ��Ϊ64,����ֵΪ625,���ʱ��Ϊ1s	
    Dir = 1;   //Ĭ������ת

    Beep = 1;
    delay_ms(500);
    Beep = 0;
    //��������λд������������

}

/*�ȴ������ת��ϵĺ�����ÿ�δ�����ת��ϣ���Ҫ��runover��ֵ��Ϊ1��Ȼ������λ������runover��ָ��
��λ��������ת�����ָ����һֱ�ж��Ƿ���յ���runoverָ�������յ��˲��ܽ������յȲ���
*/
void runOver()
{
    while(1)
    {
        //�ȴ������ת��ϣ���������ת��ϵ�ָ��
        if(runover != 0)
        {
            runover = 0;
            printf("runover\r\n");
            break;
        }
    }
}

//pos: 0 for left, 1 for middle, 2 for right
//dir: 1 for left, 0 for right
void Calibrate(){
		char prev_pos = message[2];
	  char prev_dir = message[1];
		
		if(prev_pos == '0'){
			Dir = 0; //right turn
		}
		else if(prev_pos == '2'){
			Dir = 1; //left turn			
		}
		else if(prev_pos == '1'){
			if(bigStepMotorSwitch_State_PA0 == 1){//not on the middle switch
					if(prev_dir == '1') Dir = 0;
					else Dir = 1;
			}
			else{//on the middle or other switches, do nothing
					okCmd = TRUE;
					printf("runover\r\n");
					return;
			}
		}
		else{
			return;
		}
		
		runover = 0;
    speed_cntr_Move(step, 20, 20, 2000);


    okCmd = TRUE;
    runOver();
		emb_dir = 1;
		emb_pos = 1;
}

void Manual_cal(){
	
	printf("manual\r\n");	
	memset(message, 0, sizeof message);                          //�������
  USART_RX_STA = 0;
	
	while(1){
			if(USART_RX_STA & 0x8000)
    {
        len = USART_RX_STA & 0x3fff; //�õ��˴ν��յ������ݳ���
        //printf("\r\n�����͵���ϢΪ:\r\n\r\n");

        for(t = 0; t < len; t++)
        {
            sprintf(&message[t], "%c", USART_RX_BUF[t]);
        }
				
				if(strcmp(message, "Start") == 0) {
										
					printf("ready\r\n");
					return;
				}
				
				if(message[0] == 'm')		//rΪ��ת������r0����ת��r1����ת
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
            //����Ѿ�������λ���һ�������ת���Ͳ�Ҫ��ת�ˣ������Ѿ�������λ��
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
				
			  memset(message, 0, sizeof message);                          //�������
        USART_RX_STA = 0;
				
			}
		
		
	}
	
	
				
				
	
	
}



void runZeroWhileForWitch()
{
    //0����λ ���������λ�Ͳ�ת
    if(tachswitch1 != 1)
    {
        StepMotor1_Clockwise_Digree(68);
				
    }

    if(tachswitch2 != 1)
    {
			//˳ʱ���λ
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
        len = USART_RX_STA & 0x3fff; //�õ��˴ν��յ������ݳ���
        //printf("\r\n�����͵���ϢΪ:\r\n\r\n");

        for(t = 0; t < len; t++)
        {
            sprintf(&message[t], "%c", USART_RX_BUF[t]);
        }
        
				if(message[0] == 'c'){
					Calibrate();
				}
				
				if(strcmp(message, "Manual") == 0){
					Manual_cal();					
				}
				
				if(message[0] == 'r'){
					int pc_pos = message[2]-'0';
					int pc_dir = message[1]-'0';
					if(pc_pos != emb_pos || pc_dir != emb_dir || bigStepMotorSwitch_State_PA0 == 1){
						printf("error\r\n");
					}
					if(emb_pos == 1 && emb_dir == 1){
						Dir = 1;
						emb_pos = 0;
						emb_dir = 0;
					}
					else if(emb_pos == 0 && emb_dir == 0){
						Dir = 0;
						emb_pos = 1;
						emb_dir = 0;
					}
					else if(emb_pos == 1 && emb_dir == 0){
						Dir = 0;
						emb_pos = 2;
						emb_dir = 1;
					}
					else if(emb_pos == 2 && emb_dir == 1){
						Dir = 1;
						emb_pos = 1;
						emb_dir = 1;
					}
					runover = 0;
          speed_cntr_Move(step, 20, 20, 2000);

          okCmd = TRUE;
          runOver();
					
					
					
				}

        if(message[0] == 'S')		//S1-3��ת1-3��С�����S0Ϊ��λ
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

        if(strcmp(message, "TopOpen") == 0)			//���������ͽǶȵ�
        {
            LowAngle1Open();
            LowAngle3Open();
        }

        if(strcmp(message, "BottomOpen") == 0) 	//���������ͽǶȵ�
        {
            LowAngle2Open();
            LowAngle4Open();
        }

        if(strcmp(message, "LeftOpen") == 0)		//��������ͽǶȵ�
        {
            LowAngle1Open();
            LowAngle2Open();
        }

        if(strcmp(message, "RightOpen") == 0)		//�ұ������ͽǶȵ�
        {
            LowAngle3Open();
            LowAngle4Open();
        }

        memset(message, 0, sizeof message);                          //�������
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


