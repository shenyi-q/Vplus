#include "delay.h"
#include "stepmotor1.h"

#define MA PDout(0)
#define MB PDout(1)
#define MC PDout(2)
#define MD PDout(3)


#define A1 {MA = 1;}
#define A0 {MA = 0;}
#define B1 {MB = 1;}
#define B0 {MB = 0;}
#define C1 {MC = 1;}
#define C0 {MC = 0;}
#define D1 {MD = 1;}
#define D0 {MD = 0;}

#define Coil_AB {A1;B1;C0;D0;}//AB相通电，其他相断电
#define Coil_BC {A0;B1;C1;D0;}//BC相通电，其他相断电
#define Coil_CD {A0;B0;C1;D1;}//CD相通电，其他相断电
#define Coil_DA {A1;B0;C0;D1;}//D相通电，其他相断电
#define Coil_A {A1;B0;C0;D0;}//A相通电，其他相断电
#define Coil_B {A0;B1;C0;D0;}//B相通电，其他相断电
#define Coil_C {A0;B0;C1;D0;}//C相通电，其他相断电
#define Coil_D {A0;B0;C0;D1;}//D相通电，其他相断电
#define Coil_OFF {A0;B0;C0;D0;}//全部断电

u8 step1 = 0;
u8 tachswitch1 = 1;
unsigned int Speed = 1;

void StepMotor1_Init(void)                        //四个IO口初始化
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //使能PC端口时钟
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	
	//GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;	  
	 GPIO_Init(GPIOD, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	 //GPIO_SetBits(GPIOC,GPIO_Pin_5); 						 //PE.5 输出高
	 GPIO_ResetBits(GPIOD,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
	
}
void StepMotor1_Clockwise(void){
	 Coil_A;       
     delay_ms(Speed);
     Coil_AB                 //遇到Coil_AB  用{A1=1;B1=1;C1=0;D1=0;}代替
     delay_ms(Speed);         //改变这个参数可以调整电机转速 ,
                  //数字越小，转速越大,力矩越小
     Coil_B       
     delay_ms(Speed);
     Coil_BC
     delay_ms(Speed);
     Coil_C       
     delay_ms(Speed);
     Coil_CD
     delay_ms(Speed);
     Coil_D       
     delay_ms(Speed);
     Coil_DA
     delay_ms(Speed);
 }
 
void StepMotor1_AntiClockwise(void){
     Coil_D       
     delay_ms(Speed);
     Coil_CD                 
     delay_ms(Speed);         //改变这个参数可以调整电机转速 ,
     Coil_C       
     delay_ms(Speed);
     Coil_BC
     delay_ms(Speed);
     Coil_B      
     delay_ms(Speed);
     Coil_AB
     delay_ms(Speed);
     Coil_A       
     delay_ms(Speed);
     Coil_DA
     delay_ms(Speed);
}

void StepMotor1_AntiClockwise_Digree(u16 digree){
	u16 i = 0;
	step1 = 0 ; 
	//64 * 8 是一圈
	while(!step1)
    {
        i++;
        StepMotor1_AntiClockwise();

        if(i > ((64 * 8 ) * digree / 360))
        {
            i = 0;
            break;
        }
    }
    delay_ms(100);
	step1 = 1; 
		tachswitch1 = 0;
	Coil_OFF;
}


void StepMotor1_Clockwise_Digree(u16 digree){
	u16 i = 0;
	step1 = 0 ; 
	
	//64 * 8 是一圈
	while(!step1)
    {
        i++;
        StepMotor1_Clockwise();
        if(i > ((64 * 8 ) * digree / 360))
        {
            i = 0;
            break;
        }
    }
    delay_ms(100);
	step1 = 1; 
		tachswitch1 = 1;
	Coil_OFF;
}

void StepMotor1_Stop(){
	Coil_OFF;
}
