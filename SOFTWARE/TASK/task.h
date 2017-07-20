/************************************************************
* ��֯���ƣ�
* �ļ�����: K:\��Ƭ�����\���Ӵ���������\SOFTWARE\TASK\TASK.H
* ����:     
* �汾:    
* ����:     2017/07/06
* ����:
* ��ʷ�޸ļ�¼:
* <����> <ʱ��> <�汾 > <����>
* 
***********************************************************/

#ifndef _TASK_H_
#define _TASK_H_
#include "../HARDWARE/BSP/config.h"
#define LEFT_STEP_MOTOR   0
#define RIGHT_STEP_MOTOR  1

#define LEFT_WIRE_LENGTH   0 //����߳�
#define RIGHT_WIRE_LENGTH  1 //�ұ��߳�
enum State
{
	STOP,RUNNING
};

//State systemState=STOP;

typedef struct //����ṹ��
{  
	
   float x;
	 float y;
	  
	
} Coordinate;

typedef struct   //���Ƚṹ��
{  //
	
   float Length;
	  
	
} Wire;
static Wire g_StepMotorWireLen[2];  // �����ǰλ�ü�¼�ṹ��
static Coordinate g_TargetCoordinate, g_CurrentCoordinate; // �����ǰλ�ü�¼�ṹ��

//�������������ṹ��һͬ�ٹ���һ��ϵͳ��Ϣ�ṹ��
typedef struct  
{
	  enum State systemState ;
	  Coordinate g_TargetCoordinate;
	  Coordinate g_CurrentCoordinate;
	  Wire g_StepMotorWireLen[2];
	  
}Info;
	


extern Coordinate getCurrentCoordinate(void);
extern bit setTargetCoordinate(float x, float y);
static bit setWireLength(u8 wire);
bit isSyetemRunning(void);
bit isSystemStop(void);
bit startSystem(void);
bit stopSystem(void);


#endif