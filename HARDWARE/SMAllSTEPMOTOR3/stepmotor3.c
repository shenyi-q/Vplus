#include "delay.h"

#include "stepmotor3.h"
#include "key.h"

u8 step3 = 0;
u8 tachswitch3 = 1;
unsigned int Speed3 = 1;


void A1_3(void){
	GPIO_SetBits(GPIOF,GPIO_Pin_0);
}
void A0_3(void){
	GPIO_ResetBits(GPIOF,GPIO_Pin_0);
}
void B1_3(void){
	GPIO_SetBits(GPIOF,GPIO_Pin_1);
}
void B0_3(void){
	GPIO_ResetBits(GPIOF,GPIO_Pin_1);
}
void C1_3(void){
	GPIO_SetBits(GPIOF,GPIO_Pin_2);
}
void C0_3(void){
	GPIO_ResetBits(GPIOF,GPIO_Pin_2);
}
void D1_3(void){
	GPIO_SetBits(GPIOF,GPIO_Pin_3);
}
void D0_3(void){
	GPIO_ResetBits(GPIOF,GPIO_Pin_3);
}
//
void Coil_AB3(void) {A1_3();B1_3();C0_3();D0_3();}
void Coil_BC3(void) {A0_3();B1_3();C1_3();D0_3();}
void Coil_CD3 (void){A0_3();B0_3();C1_3();D1_3();}//CD相通电，其他相断电
void Coil_DA3 (void){A1_3();B0_3();C1_3();D1_3();}//D相通电，其他相断电
void Coil_A3 (void){A1_3();B0_3();C0_3();D0_3();}//A相通电，其他相断电
void Coil_B3 (void){A0_3();B1_3();C0_3();D0_3();}//B相通电，其他相断电
void Coil_C3 (void){A0_3();B0_3();C1_3();D0_3();}//C相通电，其他相断电
void Coil_D3 (void){A0_3();B0_3();C0_3();D1_3();}//D相通电，其他相断电
void Coil_OFF3(void) {A0_3();B0_3();C0_3();D0_3();}//全部断电


void StepMotor3_Init(void)                        //四个IO口初始化
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);	 //使能PF端口时钟
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	
	//GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;	  
	 GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz					 //PE.5 输出高
	 GPIO_ResetBits(GPIOF,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
	
}
void StepMotor3_Clockwise(void){
	 Coil_A3();       
     delay_ms(Speed3);
     Coil_AB3();                //遇到Coil_AB  用{A1=1;B1=1;C1=0;D1=0;}代替
     delay_ms(Speed3);         //改变这个参数可以调整电机转速 ,
                  //数字越小，转速越大,力矩越小
     Coil_B3();      
     delay_ms(Speed3);
     Coil_BC3();
     delay_ms(Speed3);
     Coil_C3();   
     delay_ms(Speed3);
     Coil_CD3();
     delay_ms(Speed3);
     Coil_D3();   
     delay_ms(Speed3);
     Coil_DA3();
     delay_ms(Speed3);
	//Coil_A;      
 }
 
//逆时针
void StepMotor3_AntiClockwise(void){
     Coil_D3();       
     delay_ms(Speed3);
     Coil_CD3();               //遇到Coil_AB  用{A1=1;B1=1;C1=0;D1=0;}代替
     delay_ms(Speed3);         //改变这个参数可以调整电机转速 ,
     Coil_C3();   
     delay_ms(Speed3);
     Coil_BC3();
     delay_ms(Speed3);
     Coil_B3();  
     delay_ms(Speed3);
     Coil_AB3();
     delay_ms(Speed3);
     Coil_A3();   
     delay_ms(Speed3);
     Coil_DA3();
     delay_ms(Speed3);
	  //Coil_D     
}

//以正面看步进电机（看不见钻头），改函数为逆时针旋转
void StepMotor3_AntiClockwise_Digree(u16 digree){

	
	u16  i = 0;
	step3 = 0;
	//64 * 8 是一圈
	while(!step3)
    {
        i++;
        StepMotor3_AntiClockwise();

        if(i > ((64 * 8 ) * digree / 360))
        {
            i = 0;
            break;
        }
    }
    delay_ms(100);
	step3 = 1;
		tachswitch3 = 0;
	Coil_OFF3();
	
}

//顺时针旋转
void StepMotor3_Clockwise_Digree(u16 digree){

	
	u16  i = 0;
	step3 = 0;
	//64 * 8 是一圈
	while(!step3)
    {
        i++;
        StepMotor3_Clockwise();
        if(i > ((64 * 8 ) * digree / 360))
        {
            i = 0;
            break;
        }
    }
    delay_ms(100);
			tachswitch3 = 1;
	step3 = 1;
	Coil_OFF3();
}

void StepMotor3_Stop(){
	Coil_OFF3();
}
