
/************************************************************
* 组织名称：
* 文件名称: K:\单片机相关\电子大赛程序框架\SOFTWARE\TASK\TASK.C
* 作者:
* 版本:
* 日期:     2017/07/06
* 描述:  悬挂运动控制系统基本控制函数
* 历史修改记录:
* <作者> <时间> <版本 > <描述>
*
***********************************************************/

#include "task.h"
#include "../COMMON_SOFTWARE/DATA_SCOPE/DataScope_DP.h"
#include "setup.h"
#include "../HARDWARE/BSP/USART1.h"
#include "../HARDWARE/DEVICES/SENSOR/ANGLE/ANGLE.h"
#include <math.h>

#define  BOARD_HEIGHT  100.0f	//板子的高度  ，单位厘米
#define  BOARD_WIDTH   50.0f	//板子的宽度  ，单位厘米
#define  BEARING_DIA   10.0f	//旋转轴承的直径，单位毫米


//头文件中的结构体定义
/*

#define LEFT_STEP_MOTOR   0
#define RIGHT_STEP_MOTOR  1

#define LEFT_WIRE_LENGTH   0 //左边线长
#define RIGHT_WIRE_LENGTH  1 //右边线长
enum State//状态枚举
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
static Coordinate g_TargetCoordinate,g_CurrentCoordinate; // 电机当前位置记录结构体

//利用上面两个结构体一同再构建一个系统信息结构体
typedef struct
{
	  Coordinate g_TargetCoordinate;
	  Coordinate g_CurrentCoordinate;
	  Wire g_StepMotorWireLen[2];
	  u8 systemState;
}Info;


 */

//************************************
// Method:    isSyetemRunning
// FullName:  isSyetemRunning
// Access:    public 
// Returns:   bit
// Qualifier:
// Parameter: void
//************************************
bit isSyetemRunning(void)
{
  return g_MotionSystemInfo.systemState == RUNNING;
}
//************************************
// Method:    isSystemStop
// FullName:  isSystemStop
// Access:    public 
// Returns:   bit
// Qualifier:
// Parameter: void
//************************************
bit isSystemStop(void)
{
	return g_MotionSystemInfo.systemState == STOP;
}
//************************************
// Method:    startSystem
// FullName:  startSystem
// Access:    public 
// Returns:   bit
// Qualifier:
// Parameter: void
//************************************
bit startSystem(void)
{

}
//************************************
// Method:    stopSystem
// FullName:  stopSystem
// Access:    public 
// Returns:   bit
// Qualifier:
// Parameter: void
//************************************
bit stopSystem(void)
{
	g_MotionSystemInfo.systemState = STOP;
}

//************************************
// Method:    setTargetCoordinate
// FullName:  setTargetCoordinate
// Access:    public 
// Returns:   bit
// Qualifier: //设置需要到达的目标坐标
// Parameter: float x
// Parameter: float y
//************************************
bit setTargetCoordinate(float x, float y)
{
	g_MotionSystemInfo.g_TargetCoordinate.x = x;
	g_MotionSystemInfo.g_TargetCoordinate.y = y;
	setWireLength(LEFT_WIRE_LENGTH);
	setWireLength(RIGHT_WIRE_LENGTH);

	return 1;

}
//************************************
// Method:    getCurrentCoordinate
// FullName:  getCurrentCoordinate
// Access:    public 
// Returns:   Coordinate
// Qualifier: //读取当前坐标，返回一个xy结构体
// Parameter: u8 motor
//************************************
Coordinate getCurrentCoordinate(void)
{

	return g_MotionSystemInfo.g_CurrentCoordinate;
}


///////////////////！以下为私有函数，外部不能调用！///////////////////////////
//************************************
// Method:    setWireLength
// FullName:  setWireLength
// Access:    public 
// Returns:   bit
// Qualifier: //设置a,b的长度
// Parameter: u8 wire
//************************************
static bit setWireLength(u8 wire)
{
	if (wire)
	{
		//a的线长计算
		g_MotionSystemInfo.g_StepMotorWireLen[wire].Length=sqrt(	pow(g_MotionSystemInfo.g_TargetCoordinate.x,2)	            
																   +pow(BOARD_HEIGHT- g_MotionSystemInfo.g_TargetCoordinate.y,2)
															    ) ;
	}
	else
	{
		//b的线长计算
		g_MotionSystemInfo.g_StepMotorWireLen[wire].Length = sqrt(	  pow(BOARD_WIDTH-g_MotionSystemInfo.g_TargetCoordinate.x, 2)	
																	+ pow(BOARD_HEIGHT - g_MotionSystemInfo.g_TargetCoordinate.y, 2)
															     );
	}
}
static float getWireLength(u8 wire)
{
	return g_MotionSystemInfo.g_StepMotorWireLen[wire].Length;
}

static bit setStepMotorAngle(void)
{
	
}





