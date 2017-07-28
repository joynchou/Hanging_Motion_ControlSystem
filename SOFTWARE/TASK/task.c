/************************************************************
* 组织名称：
* 文件名称: K:\单片机相关\电子大赛程序框架\SOFTWARE\TASK\TASK.C
* 作者:
* 版本:
* 日期:     2017/07/06
* 描述:  悬挂运动控制系统基本控制函数 ,初始化之后，设置好xy轴坐标，再调用startSystem函数，既可以开始
* 历史修改记录:
* <作者> <时间> <版本 > <描述>
*
***********************************************************/

#include "task.h"
#include "../COMMON_SOFTWARE/DATA_SCOPE/DataScope_DP.h"
#include "setup.h"
#include "../HARDWARE/BSP/USART1.h"
#include "../../HARDWARE/COMMON_HARDWARE/fixedPulser.h"
#include "../../HARDWARE/BSP/delay.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "../HARDWARE/BSP/stc15_pwm.h"

#define  BOARD_HEIGHT  52.5f	//板子的高度  ，单位厘米
#define  BOARD_WIDTH   30	//板子的宽度  ，单位厘米
#define  BEARING_DIA   4.05f	//旋转轴承的直径，单位厘米
#define	 PI  3.1415f   //pi的值
//#define  ACCURACY  0.1f //精度定义  每次步进的脉冲数
#define  MOVING_SPEED  1300   //系统移动速度定义 ,不要超过1400，也不要低于100




//************************************
// Method:    isSyetemRunning
// FullName:  isSyetemRunning
// Access:    public 
// Returns:   bit
// Qualifier: 返回当前系统是否正在移动
// Parameter: void
//************************************
enum State getSystemState(void)
{
	return g_MotionSystemInfo.systemState;
}
void setSystemState(enum State state)
{
	g_MotionSystemInfo.systemState = state;
}
//************************************
// Method:    startSystem
// FullName:  startSystem
// Access:    public 
// Returns:   bit
// Qualifier: 在设置好目标坐标后调用此函数悬挂系统开始执行
// Parameter: void
//************************************
bit startSystem(void)
{
	g_MotionSystemInfo.systemState = WORKING;
	//设置步进电机脉冲次数
	setStepMotorDis();
	//开启步进电机
	open_StepMotor(LEFT_STEP_MOTOR);
	open_StepMotor(RIGHT_STEP_MOTOR);

	//更新坐标
	g_MotionSystemInfo.g_CurrentCoordinate.x = g_MotionSystemInfo.g_TargetCoordinate.x;
	g_MotionSystemInfo.g_CurrentCoordinate.y = g_MotionSystemInfo.g_TargetCoordinate.y;

	return 1;
}
//************************************
// Method:    stopSystem
// FullName:  stopSystem
// Access:    public 
// Returns:   bit
// Qualifier: 强行关闭悬挂系统的运动
// Parameter: void
//************************************
bit stopSystem(void)
{
	close_StepMotor(LEFT_STEP_MOTOR);
	close_StepMotor(RIGHT_STEP_MOTOR);
	g_MotionSystemInfo.systemState = STOP;
	return 1;
}
bit setCurrentCoordinate(float x, float y)
{
	g_MotionSystemInfo.g_CurrentCoordinate.x = x;
	g_MotionSystemInfo.g_CurrentCoordinate.y = y;
	return 1;
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

void MotionSystem_Init(void)
{

	stepMotor_Init(LEFT_STEP_MOTOR, 1.8f, 16, 1000, 3.9f);
	stepMotor_Init(RIGHT_STEP_MOTOR, 1.8f, 16, 1000, 3.9f);

}

///////////////////！以下为私有函数，外部不能调用！///////////////////////////
//根据输入的xy轴数据计算两边相应的线长 ，单位cm
static float getWireLength(u8 wire, float x, float y)
{
	float Length = 0;
	if (wire == LEFT_WIRE_LENGTH)
	{
		//左边的线长计算
		Length = sqrt
			(
				pow(x, 2) //单位，cm
				+ pow(BOARD_HEIGHT - y, 2)
				);
	}
	else
	{
		//右边的线长计算
		Length = sqrt
			(
				pow(BOARD_WIDTH - x, 2)
				+ pow(BOARD_HEIGHT - y, 2)
				);
	}
	return Length;

}
//计算两点的距离公式
static double twoPointDistance(float x1, float y1, float x2, float y2)
{
	return sqrt((double)((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2)));
}
//旋转指定长度脉冲发生器需要产生的脉冲数,设细分值为16 ，步进电机的步进角为1.8
static u16 getPulserCount(float distance)
{
	return (u16)((distance * 360 * 16) / (PI*BEARING_DIA*1.8f));
}
//设置电机旋转距离，从而改变线长度
//此函数适合使用微分的办法来画出需要的形状，如直线或者圆之类的，但不适合将dx dy设置的太大，否则会有很大的误差
//其中一个原因是软件模拟的脉冲发生器的固有误差
//所以此方法很适合用微分来减小误差
static bit setStepMotorDis(void)
{
	u8 str[50];
	int a = 0;
	u32 rightMotorPulse, leftMotorPulse;//左右电机需要产生的总脉冲数
	float D_leftWireLength;//左边线的dL(微分)
	float D_rightWireLength;//右边线的dL
	float currentLeftWireLength;//当前坐标下的左线长
	float currentRightWireLength;//当前坐标下的右线长
	float targetLeftWireLength;//目标坐标下的左线长
	float targetRightWireLength;//目标坐标下的右线长
	float K;//两个电机的速度比，左电机比右电机 
	u16 rightStepMotorSpeed;//右电机的速度
	u16 leftStepMotorSpeed;//左电机的速度
	float distance;//当前坐标和目标坐标的距离

	/*  如果要使用定时器时分复用的方法则需要用到这些变量
	float sum;
	u16 loop;
	float leftMotorOneStepPulse;//单步脉冲数
	float rightMotorOneStepPulse;
	*/
	//计算当前线长
	currentLeftWireLength = getWireLength(LEFT_WIRE_LENGTH, g_MotionSystemInfo.g_CurrentCoordinate.x, g_MotionSystemInfo.g_CurrentCoordinate.y);
	currentRightWireLength = getWireLength(RIGHT_WIRE_LENGTH, g_MotionSystemInfo.g_CurrentCoordinate.x, g_MotionSystemInfo.g_CurrentCoordinate.y);
#ifdef DEBUG

	sprintf(str, "currentLeftWireLength is %f\r\n", currentLeftWireLength);
	//PrintString1(str);
	sprintf(str, "currentRightWireLength is %f\r\n", currentRightWireLength);
	//PrintString1(str);
#endif



   //计算目标线长
	targetLeftWireLength = getWireLength(LEFT_WIRE_LENGTH, g_MotionSystemInfo.g_TargetCoordinate.x, g_MotionSystemInfo.g_TargetCoordinate.y);
	targetRightWireLength = getWireLength(RIGHT_WIRE_LENGTH, g_MotionSystemInfo.g_TargetCoordinate.x, g_MotionSystemInfo.g_TargetCoordinate.y);
#ifdef DEBUG
	sprintf(str, "targetLeftWireLength is %f\r\n", targetLeftWireLength);
	//PrintString1(str);
	sprintf(str, "targetRightWireLength is %f\r\n", targetRightWireLength);
	//PrintString1(str);
#endif



   //计算微分值
	D_leftWireLength = fabs(targetLeftWireLength - currentLeftWireLength);
	D_rightWireLength = fabs(targetRightWireLength - currentRightWireLength);
	sprintf(str, "D_leftWireLength is %f\r\n", D_leftWireLength);
	//PrintString1(str);
	sprintf(str, "D_rightWireLength is %f\r\n", D_rightWireLength);
	//PrintString1(str);





	//判断左电机的正反转
	if (targetLeftWireLength >= currentLeftWireLength)//如果目标坐标的左线长大于当前坐标的左线长
	{
		setStepMotorDiversion(LEFT_STEP_MOTOR, CCW);//逆时针旋转

	}
	else
	{
		setStepMotorDiversion(LEFT_STEP_MOTOR, CW);

	}
	//判断右电机的正反转
	if (targetRightWireLength >= currentRightWireLength)//如果目标坐标的左线长大于当前坐标的左线长
	{
		setStepMotorDiversion(RIGHT_STEP_MOTOR, CW);//顺时针旋转

	}
	else
	{
		setStepMotorDiversion(RIGHT_STEP_MOTOR, CCW);

	}


	//左电机需要产生的总脉冲数
	leftMotorPulse = getPulserCount(D_leftWireLength);
	//右电机需要产生的总脉冲数
	rightMotorPulse = getPulserCount(D_rightWireLength);

	distance = twoPointDistance
		(
			g_MotionSystemInfo.g_TargetCoordinate.x,
			g_MotionSystemInfo.g_TargetCoordinate.y,
			g_MotionSystemInfo.g_CurrentCoordinate.x,
			g_MotionSystemInfo.g_CurrentCoordinate.y
			)
		;
	//	sprintf
	//		(
	//			str, "Cur(%f,%f)to Tar(%f,%f) distance is %f \r\n",
	//			g_MotionSystemInfo.g_CurrentCoordinate.x,
	//			g_MotionSystemInfo.g_CurrentCoordinate.y,
	//			g_MotionSystemInfo.g_TargetCoordinate.x,
	//			g_MotionSystemInfo.g_TargetCoordinate.y,
	//			distance
	//			)
	//		;
		//PrintString1(str);

	K = (float)rightMotorPulse / leftMotorPulse;//右电机和左电机路程之比
#ifdef DEBUG

	sprintf(str, "leftmotorPulse is %d\r\n", leftMotorPulse);
	//PrintString1(str);
	sprintf(str, "rightmotorPulse is %d\r\n", rightMotorPulse);
	//PrintString1(str);
	sprintf(str, "K is %f\r\n", K);
	PrintString1(str);
#endif
	//根据已经定好的左电机速度计算出右电机相应的速度
	rightStepMotorSpeed = (u16)(MOVING_SPEED * K);
	//防止右电机速度超出最大速度，如果超出，则将设定速度附给右电机，左电机速度减小，左电机和右电机的速度比还是不变
	if (rightStepMotorSpeed >= MOVING_SPEED)
	{
		rightStepMotorSpeed = MOVING_SPEED;
		leftStepMotorSpeed = rightStepMotorSpeed / K;
	}
	else//否则还是使左电机赋值
	{
		leftStepMotorSpeed = MOVING_SPEED;
	}
#ifdef DEBUG

	sprintf(str, "rightStepmotorSpeed is %d \r\n", rightStepMotorSpeed);
	PrintString1(str);
	sprintf(str, "leftStepmotorSpeed is %d \r\n", leftStepMotorSpeed);
	PrintString1(str);
#endif

	//通过直接算出需要的脉冲数来驱动步进电机
	setStepMotorWithPulse(LEFT_STEP_MOTOR, leftMotorPulse, leftStepMotorSpeed);
	setStepMotorWithPulse(RIGHT_STEP_MOTOR, rightMotorPulse, rightStepMotorSpeed);


	/*如果只有一个pwm信号发生器可以用以下的办法 ,两个步进电机分别使用一个定时器
	//计算两个坐标的距离
	distance = twoPointDistance(g_MotionSystemInfo.g_TargetCoordinate.x, g_MotionSystemInfo.g_TargetCoordinate.y, g_MotionSystemInfo.g_CurrentCoordinate.x, g_MotionSystemInfo.g_CurrentCoordinate.y);
	sum = (distance / ACCURACY);//最终需要让电机运动的次数
   //printf("sum is %f\n", sum);
	loop = (u16)(sum);
	leftMotorOneStepPulse = 0.5f + leftMotorPulse / sum;//左电机单步的脉冲次数
	rightMotorOneStepPulse = 0.5f + rightMotorPulse / sum;//右电机单步的脉冲次数
	while(loop--)
	{
		setPulse(LEFT_STEP_MOTOR, MOVING_SPEED, (u32)(leftMotorOneStepPulse));
		openPulser(LEFT_STEP_MOTOR);
		delay_ms((leftMotorOneStepPulse * 1000) / MOVING_SPEED);    //等待此步执行完成,delay的公式  (1/hz) *ACCuracy
	close_PWM_ALL();

		setPulse(RIGHT_STEP_MOTOR, rightStepMotorSpeed, (u32)(rightMotorOneStepPulse));
		openPulser(RIGHT_STEP_MOTOR);
		delay_ms((rightMotorOneStepPulse * 1000) / rightStepMotorSpeed);    //等待此步执行完成,delay的公式  (1/hz) *ACCuracy
	close_PWM_ALL();

	}

   */

	return 1;
}













