#ifndef __STEPMOTOR_BIG_H
#define __STEPMOTOR_BIG_H
#include "sys.h"

#define Dir PAout(5)  // PA5
#define PA7_PWM PAout(7)    //pA7

typedef struct 
{
  unsigned char run_state : 3;//! What part of the speed ramp we are in.
  unsigned char dir : 1;//! Direction stepper motor should move.
  unsigned int step_delay;//! Peroid of next timer delay. At start this value set the accelration rate.  
  unsigned int decel_start;//! What step_pos to start decelaration
  signed int decel_val;//! Sets deceleration rate.  
  signed int min_delay;//! Minimum time delay (max speed)
  signed int accel_count;//! Counter used when accelerateing/decelerateing to calculate step_delay.
	
  unsigned int speedAdd;          //ÿ�μӶ���Hz ,ÿ10ms��һ��
  unsigned int stepCount;          //����
  unsigned int startSpeed;            //��ʼ����
	unsigned int decelSpeed;            //���ٵ��������
	unsigned int currentSpeed;         //��ǰ����
	unsigned int maxSpeed;  
	unsigned int maxStep;  
	
} speedRampData;
extern speedRampData srd;

// Speed ramp states
#define STOP  0
#define ACCEL 1
#define DECEL 2
#define RUN   3


void PWM_IO_Init(void);
//

void speed_cntr_Move(signed int step, unsigned int accel, unsigned int decel, unsigned int speed);



#endif
