
/************************************************************
* 组织名称：
* 文件名称: K:\单片机相关\电子大赛程序框架\SOFTWARE\TASK\TASK.H
* 作者:     
* 版本:    
* 日期:     2017/07/06
* 描述:
* 历史修改记录:
* <作者> <时间> <版本 > <描述>
* 
***********************************************************/

#ifndef _TASK_H_
#define _TASK_H_
#include "../../HARDWARE/BSP/config.h"
#include "../../HARDWARE/DEVICES/MOTOR/STEP_MOTOR/STEP_MOTOR.H"
#include "../../HARDWARE/COMMON_HARDWARE/fixedPulser.h"
#include "../../HARDWARE/BSP/delay.h"
#include <math.h>
#include "../HARDWARE/BSP/stc15_pwm.h"

#define LEFT_STEP_MOTOR   STEP_MOTOR_1
#define RIGHT_STEP_MOTOR  STEP_MOTOR_2

#define LEFT_WIRE_LENGTH   0 //左边线长
#define RIGHT_WIRE_LENGTH  1 //右边线长
enum State//状态枚举
{
	STOP,WORKING
};

//State systemState=STOP;

typedef struct //坐标结构体
{  
	
   float x;
	 float y;
	  
	
} Coordinate;

typedef struct   //长度结构体
{  //
	
   float Length;
	  
	
} Wire;
static Wire g_StepMotorTargetWireLen[2],g_StepMotorCurrentWireLen[2];  // 电机当前位置记录结构体
static Coordinate g_TargetCoordinate, g_CurrentCoordinate; // 电机当前位置记录结构体

//利用上面两个结构体一同再构建一个系统信息结构体
typedef struct  
{
	  enum State systemState ;
	  Coordinate g_TargetCoordinate;
	  Coordinate g_CurrentCoordinate;
	  Wire g_StepMotorWireLen[2];
	  
}Info;
	
static Info g_MotionSystemInfo; //悬挂运动控制系统信息结构体 	
extern enum State getSystemState(void);
extern void setSystemState(enum State state);

static bit setWireLength(u8 wire);
static float getWireLength(u8 wire,float x,float y);
static bit setStepMotorDis(void);
extern void MotionSystem_Init(void);                                                          
extern Coordinate getCurrentCoordinate(void);
extern bit setTargetCoordinate(float x, float y);
extern bit setCurrentCoordinate(float x,float y);
extern bit startSystem(void);
extern bit stopSystem(void);


#endif
