#include "delay.h"

#include "stepmotor2.h"

#define MA2 PDout(4)
#define MB2 PDout(5)
#define MC2 PDout(6)
#define MD2 PDout(7)


#define A1_2 {MA2 = 1;}
#define A0_2 {MA2 = 0;}
#define B1_2 {MB2 = 1;}
#define B0_2 {MB2 = 0;}
#define C1_2 {MC2 = 1;}
#define C0_2 {MC2 = 0;}
#define D1_2 {MD2 = 1;}
#define D0_2 {MD2 = 0;}

#define Coil_AB2 {A1_2;B1_2;C0_2;D0_2;}//AB相通电，其他相断电
#define Coil_BC2 {A0_2;B1_2;C1_2;D0_2;}//BC相通电，其他相断电
#define Coil_CD2 {A0_2;B0_2;C1_2;D1_2;}//CD相通电，其他相断电
#define Coil_DA2 {A1_2;B0_2;C0_2;D1_2;}//D相通电，其他相断电
#define Coil_A2 {A1_2;B0_2;C0_2;D0_2;}//A相通电，其他相断电
#define Coil_B2 {A0_2;B1_2;C0_2;D0_2;}//B相通电，其他相断电
#define Coil_C2 {A0_2;B0_2;C1_2;D0_2;}//C相通电，其他相断电
#define Coil_D2 {A0_2;B0_2;C0_2;D1_2;}//D相通电，其他相断电
#define Coil_OFF2 {A0_2;B0_2;C0_2;D0_2;}//全部断电

u8 step2 = 0;
u8 tachswitch2 = 1;
unsigned int Speed2 = 1;

void StepMotor2_Init(void)                        //四个IO口初始化
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //使能PC端口时钟
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	
	//GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;	  
	 GPIO_Init(GPIOD, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	 //GPIO_SetBits(GPIOC,GPIO_Pin_5); 						 //PE.5 输出高
	 GPIO_ResetBits(GPIOD,GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
}

//顺时针
void StepMotor2_Clockwise(void){
	 Coil_A2;       
     delay_ms(Speed2);
     Coil_AB2                 //遇到Coil_AB  用{A1=1;B1=1;C1=0;D1=0;}代替
     delay_ms(Speed2);         //改变这个参数可以调整电机转速 ,
                  //数字越小，转速越大,力矩越小
     Coil_B2       
     delay_ms(Speed2);
     Coil_BC2
     delay_ms(Speed2);
     Coil_C2       
     delay_ms(Speed2);
     Coil_CD2
     delay_ms(Speed2);
     Coil_D2       
     delay_ms(Speed2);
     Coil_DA2
     delay_ms(Speed2);
 }
 
 //逆时针
void StepMotor2_AntiClockwise(void){
     Coil_D2       
     delay_ms(Speed2);
     Coil_CD2                 //遇到Coil_AB  用{A1=1;B1=1;C1=0;D1=0;}代替
     delay_ms(Speed2);         //改变这个参数可以调整电机转速 ,
     Coil_C2       
     delay_ms(Speed2);
     Coil_BC2
     delay_ms(Speed2);
     Coil_B2      
     delay_ms(Speed2);
     Coil_AB2
     delay_ms(Speed2);
     Coil_A2       
     delay_ms(Speed2);
     Coil_DA2
     delay_ms(Speed2);
}
void StepMotor2_AntiClockwise_Digree(u16 digree){

	
	u16  i = 0;
	step2 = 0;
	//64 * 8 是一圈
	while(!step2)
    {
        i++;
        StepMotor2_AntiClockwise();

        if(i > ((64 * 8) * digree / 360))
        {
            i = 0;
            break;
        }
    }
    delay_ms(100);
	step2 = 1;
		tachswitch2 = 0;
	Coil_OFF2;
	
}


void StepMotor2_Clockwise_Digree(u16 digree){

	
	u16  i = 0;
	step2 = 0;
	//64 * 8 是一圈
	while(!step2)
    {
        i++;
        StepMotor2_Clockwise();
        if(i > ((64 * 8) * digree / 360))
        {
            i = 0;
            break;
        }
    }
    delay_ms(100);
	step2 = 1;
		tachswitch2 = 1;
	Coil_OFF2;
}
void StepMotor2_Stop(){
	Coil_OFF2;
}


