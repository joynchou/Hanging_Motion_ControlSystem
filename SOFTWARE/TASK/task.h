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
#include "../HARDWARE/BSP/config.h"
enum State
{
	STOP,RUNNING
};
typedef struct //坐标结构体
{  
	
   float x;
	 float y;
	  
	
} Coordinate;

typedef struct   //长度结构体
{  //
	
   float Length;
	  
	
} Wire;
static Wire g_StepMotorWireLen[2];  // 电机当前位置记录结构体
static Coordinate g_TargetCoordinate, g_CurrentCoordinate; // 电机当前位置记录结构体

//利用上面两个结构体一同再构建一个系统信息结构体
typedef struct  
{
	  Coordinate g_TargetCoordinate;
	  Coordinate g_CurrentCoordinate;
	  Wire g_StepMotorWireLen[2];
	  enum State systemState;
}Info;
	


extern Coordinate getCurrentCoordinate(void);
extern bit setTargetCoordinate(float x, float y);
static bit setWireLength(u8 wire);
bit isSyetemRunning(void);
bit isSystemStop(void);
bit startSystem(void);
bit stopSystem(void);


#endif
