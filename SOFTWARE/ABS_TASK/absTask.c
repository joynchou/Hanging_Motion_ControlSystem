#include "../TASK/TASK.h"
#include <rtx51tny.h>                 /* RTX-51 tiny functions & defines      */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../TASK/setup.h"
#include "../../HARDWARE/DEVICES/BUTTON/BUTTON.H"
#include "../../HARDWARE/DEVICES/LED/LED.H"
#include "../../HARDWARE/BSP/USART1.H"
#include "../HARDWARE/DEVICES/SENSOR/ANGLE/ANGLE.h"
#include "../COMMON_SOFTWARE/DATA_SCOPE/DataScope_DP.h"
#include "../COMMON_SOFTWARE/STORAGE/STORAGE.h"
#include "../TASK/task.h"
#define DX 0.1f

#define   LEFT_MOTOR_STATE_UPDATE 1
#define   RIGHT_MOTOR_STATE_UPDATE 2
#define   SYSTEM_STATE_UPDATE  3
float initialX = 7.5f;
float initialY = 20;
u8 str[26];
Coordinate currentCodnat;


void taskStart() _task_ 0
{
	setup();
	blink(2);
		setCurrentCoordinate(initialX, initialY);
	os_create_task(LEFT_MOTOR_STATE_UPDATE);
	os_create_task(RIGHT_MOTOR_STATE_UPDATE);
	os_create_task(SYSTEM_STATE_UPDATE);
	os_create_task(8);
	os_delete_task(0);

}
void waitingSystem(void)//等待系统上一个坐标到达，此任务会堵塞当前进程
{
wait:
	if (getSystemState() == (enum State)WORKING)
	{
		os_wait(K_TMO, 5, 0);
		goto wait;
	}
}

void resetSystem()//复位系统
{
	setTargetCoordinate(initialX, initialY);
	waitingSystem();
	startSystem();


}

void systemUp(float dis)
{
	currentCodnat = getCurrentCoordinate();
	setTargetCoordinate(currentCodnat.x, currentCodnat.y + dis);
	waitingSystem();
	startSystem();

}
void systemDown(float dis)
{
	currentCodnat = getCurrentCoordinate();
	setTargetCoordinate(currentCodnat.x, currentCodnat.y - dis);
	waitingSystem();
	startSystem();

}
void systemLeft(float dis)
{
	currentCodnat = getCurrentCoordinate();
	setTargetCoordinate(currentCodnat.x - dis, currentCodnat.y);
	waitingSystem();
	startSystem();

}
void systemRight(float dis)
{
	currentCodnat = getCurrentCoordinate();
	setTargetCoordinate(currentCodnat.x + dis, currentCodnat.y);
	waitingSystem();
	startSystem();

}

void drawLoopSrtLine(void)
{
	u16 i = 0;
	for (;;)
	{
		for (i = 0; i < 100; i++)
		{
			systemUp(DX);
		}
		for (i = 0; i < 100; i++)
		{
			systemDown(DX);
		}

	}
}
void drawLoopRectangle(void) 
{
	u8 length = 6;
	for (;;)
	{
		systemUp(length);
		systemRight(length);
		systemDown(length);
		systemLeft(length);
	}

}
void drawLine() 
{
	static float x = 0, y = 0;
	int tmp = 0;
		for (tmp = 0; tmp < 14; tmp++)
		{
			sprintf(str, "(%f,%f) is arrival\n", x, y);
			PrintString1(str);
			x += 1;
			y = 2 * x;

			setTargetCoordinate(x, y);
			waitingSystem();
			startSystem();//开启系统

			
		}

		resetSystem();


}

void drawCircle(float x, float y)
{
	int tmp = 0;
	float X = initialX;
	float Y = initialY;  //初始坐标
	float subdivide = 1024.0f;//画圆的细分值
	float angle = 360;


		for (tmp = 0; tmp <= subdivide; tmp++)
		{
			sprintf(str, "(%f,%f) is arrival,tmp = %d \n", X, Y, tmp);
			PrintString1(str);

			angle -= 360.0f / subdivide;
			X = x + 5 * cos(angle*(0.01745f));
			Y = y + 5 * sin(angle*(0.01745f));
			setTargetCoordinate(X, Y);
			waitingSystem();
			startSystem();


		}
		resetSystem();

	

}
void systemTest() _task_ 8
{
	for (;;)
	{
		drawCircle(14, 30);
		
	}

}


 void leftStepMotorStateUpdate() _task_ LEFT_MOTOR_STATE_UPDATE//左电机状态更新进程
{
	for (;;)
	{
		if (getPulserState(PULSER_1) == ON)//如果左电机脉冲打开，则认为左电机正在工作
		{
			setStepMotorState(LEFT_STEP_MOTOR, (enum StepMotorState) RUNNING);
			//	PrintString1("left stepMotor is running \r\n");

		}
		else
		{
			setStepMotorState(LEFT_STEP_MOTOR, (enum StepMotorState) LOCKED);
			//	PrintString1("left stepMotor is locked \r\n");

		}
		os_wait(K_TMO, 1, 0);
	}
}
void rightStepMotorStateUpdate()  _task_  RIGHT_MOTOR_STATE_UPDATE//右电机状态更新进程
{
	for (;;)
	{
		if (getPulserState(RIGHT_STEP_MOTOR) == ON)//如果右电机脉冲打开，则认为右电机正在工作
		{
			setStepMotorState(RIGHT_STEP_MOTOR, (enum StepMotorState) RUNNING);
			//		PrintString1("right stepMotor is running \r\n");
		}
		else
		{
			setStepMotorState(RIGHT_STEP_MOTOR, (enum StepMotorState)LOCKED);
			//PrintString1("right stepMotor is locked \r\n");
		}
		os_wait(K_TMO, 1, 0);
	}
}
void systemStateUpdate() _task_  SYSTEM_STATE_UPDATE//系统状态更新进程
{
	for (;;)
	{
		if ((getStepMotorState(LEFT_STEP_MOTOR) == (enum StepMotorState)RUNNING || getStepMotorState(RIGHT_STEP_MOTOR) == (enum StepMotorState)RUNNING))//如果系统还没有到达目标坐标
		{
			setSystemState((enum State)WORKING);
			//	PrintString1("system is working \r\n");
		}
		else
		{
			//	PrintString1("system is stopped \r\n");
			setSystemState((enum State)STOP);

		}
		os_wait(K_TMO, 3, 0);

	}
}

