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
#include "../TASK/direction.h"
#define DX 0.08f

#define   LEFT_MOTOR_STATE_UPDATE 1
#define   RIGHT_MOTOR_STATE_UPDATE 2
#define   SYSTEM_STATE_UPDATE  3
#define	  SYSTEM_GATE_STATE_UPDATE 4
#define   SYSTEM_GATE_ADC_UPDATE  5

#define FORWARD_LEFT 0
#define FORWARD_RIGHT 1
#define FORWARD_UP 2
#define FORWARD_DOWN 3

#define DEFUALT_HEAD  FORWARD_UP
#define DEBUG

u8 currentHead= DEFUALT_HEAD;
u8 lastTimeHead= DEFUALT_HEAD;
float initialX =5;
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
	os_create_task(SYSTEM_GATE_ADC_UPDATE);
	os_create_task(SYSTEM_GATE_STATE_UPDATE);

	os_create_task(6);
	os_delete_task(0);

}

void waitingSystem(void)//等待系统上一个坐标到达，此任务会堵塞在使用它的进程进程
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

bool readLeft()//读逆时针旋转90°的方向上的传感器
{
	switch (currentHead)
	{
	case FORWARD_UP:
	{
		if (readGate(LEFT_GATE))
		{
			return 1;
		}
	};
	break;
	case FORWARD_DOWN:
	{
		if (readGate(RIGHT_GATE))
		{
			return 1;
		}
	};
	break;
	case FORWARD_RIGHT:
	{
		if (readGate(UP_GATE))
		{
			return 1;
		}
	};
	break;
	case FORWARD_LEFT:
	{
		if (readGate(DOWN_GATE))
		{
			return 1;
		}
	};
	break;
	default:
		break;

	}
}

void turnLeft()//逆时针旋转90°
{
	switch (currentHead)
	{
	case FORWARD_UP:
	{
		systemLeft(DX);
	};
	break;
	case FORWARD_DOWN:
	{
		systemRight(DX);
	};
	break;
	case FORWARD_RIGHT:
	{
		systemUp(DX);
	};
	break;
	case FORWARD_LEFT:
	{
	//	systemDown(DX);
	};
	break;
	default:
		break;

	}
}

bool readRight()//读顺时针旋转90°方向的值
{
	switch (currentHead)
	{
	case FORWARD_UP:
	{
		if (readGate(RIGHT_GATE))
		{
			return 1;
		}
	};
	break;
	case FORWARD_DOWN:
	{
		if (readGate(LEFT_GATE))
		{
			return 1;
		}
	};
	break;
	case FORWARD_RIGHT:
	{
		if (readGate(DOWN_GATE))
		{
			return 1;
		}
	};
	break;
	case FORWARD_LEFT:
	{
		if (readGate(UP_GATE))
		{
			return 1;
		}
	};
	break;
	default:
		break;

	}
}

void turnRight()//顺时针旋转90°
{
	switch (currentHead)
	{
	case FORWARD_UP:
	{
		systemRight(DX);
	};
	break;
	case FORWARD_DOWN:
	{
		systemLeft(DX);
	};
	break;
	case FORWARD_RIGHT:
	{
	//	systemDown(DX);
	};
	break;
	case FORWARD_LEFT:
	{
		systemUp(DX);
	};
	break;
	default:
		break;

	}
}

void goStraight()//方向为车头方向
{
	switch (currentHead)
	{
	case FORWARD_UP:
	{
		systemUp(DX);
	};
	break;
	case FORWARD_DOWN:
	{
	//	systemDown(DX);
	};
	break;
	case FORWARD_RIGHT:
	{
		systemRight(DX);
	};
	break;
	case FORWARD_LEFT:
	{
		systemLeft(DX);
	};
	break;
	default:
		break;

	}

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
void Tracking(void) _task_ 9
{
	for(;;)
	
	{
		if (readGate(LEFT_GATE))
		{
			systemRight(DX);
#ifdef DEBUG
			PrintString1("right\r\n");
#endif // DEBUG

			
		}
		else if (readGate(RIGHT_GATE))
		{
			systemLeft(DX);
#ifdef DEBUG
			PrintString1("left\r\n");
#endif // DEBUG

		}
		else if (readGate(UP_GATE))
		{
			systemDown(DX);
#ifdef DEBUG
			PrintString1("down\r\n");
#endif // DEBUG

		}
		else if (readGate(DOWN_GATE))
		{
			systemUp(DX);
#ifdef DEBUG
			PrintString1("up\r\n");
#endif // DEBUG

		}
		else if (readGate(DOWN_GATE) && readGate(UP_GATE))
		{
			systemUp(DX);
#ifdef DEBUG
			PrintString1("up\r\n");
#endif // DEBUG

		}
		else if (readGate(LEFT_GATE) && readGate(RIGHT_GATE))
		{
			systemRight(DX);
#ifdef DEBUG
			PrintString1("right\r\n");
#endif // DEBUG

		}
		else if(readGate(UP_GATE)&&readGate(RIGHT_GATE))
		{
			systemDown(DX);
			systemLeft(DX);
#ifdef DEBUG
			PrintString1("down and left\r\n");
#endif // DEBUG

		}
		else if(readGate(UP_GATE)&&readGate(LEFT_GATE))
		{
			systemDown(DX);
			systemRight(DX);
#ifdef DEBUG
			PrintString1("down and right\r\n");
#endif // DEBUG

		}
		else if (readGate(DOWN_GATE) && readGate(RIGHT_GATE))
		{
			systemUp(DX);
			systemLeft(DX);
#ifdef DEBUG
			PrintString1("up and left\r\n");
#endif // DEBUG

		}
		else if (readGate(LEFT_GATE) && readGate(DOWN_GATE))
		{
			systemRight(DX);
			systemUp(DX);
#ifdef DEBUG
			PrintString1("right and up\r\n");
#endif // DEBUG

		}
		else
		{
			systemUp(DX);
		}
		os_wait(K_TMO, 70, 0);
	}
	/*
	for (;;)
	{
		if (readLeft())//如果车头左边读到线，则车头方向逆时针旋转90°
		{
			turnLeft();
			lastTimeHead = currentHead;//更新上一次的车头方向

			switch (lastTimeHead)
			{
			case FORWARD_UP:
			{
				currentHead = FORWARD_LEFT;
			};
			break;
			case FORWARD_DOWN:
			{
				currentHead = FORWARD_RIGHT;
			};
			break;
			case FORWARD_LEFT:
			{
			//	currentHead = FORWARD_DOWN;
			};
			break;
			case FORWARD_RIGHT:
			{
				currentHead = FORWARD_UP;
			};
			break;
			default:
				break;
			}
		}
		else if (readRight())//如果车头右边读到线，则车头方向顺时针旋转90°
		{
			turnRight();
			lastTimeHead = currentHead;//更新上一次的车头方向

			switch (lastTimeHead)
			{
			case FORWARD_UP:
			{
				currentHead = FORWARD_RIGHT;
			};
			break;
			case FORWARD_DOWN:
			{
				currentHead = FORWARD_LEFT;
			};
			break;
			case FORWARD_LEFT:
			{
				currentHead = FORWARD_UP;
			};
			break;
			case FORWARD_RIGHT:
			{
				currentHead = FORWARD_DOWN;
			};
			break;
			default:
				break;
			}
		}
		else //如果没有读到线，车头方向直走
		{
			goStraight();
		}
		os_wait(K_TMO, 70, 0);
	}
	*/

}
void systemTest( ) _task_ 6
{
	for (;;)
	{
		//drawCircle(14, 30);
        systemUp(5);
		systemRight(5);
		systemDown(5);
		systemLeft(5);
		//resetSystem();

	}

}
void printGateState(void) _task_ 7
{
	for (;;)
	{
		if (readGate(LEFT_GATE))
		{
			PrintString1("left gate is on\r\n");
		}
		else
		{
			PrintString1("left gate is off\r\n");
		}
		if (readGate(RIGHT_GATE))
		{
			PrintString1("right gate is on\r\n");
		}
		else
		{
			PrintString1("right gate is off\r\n");
		}
		if (readGate(UP_GATE))
		{
			PrintString1("up gate is on\r\n");
		}
		else
		{
			PrintString1("up gate is off\r\n");
		}

		if (readGate(DOWN_GATE))
		{
			PrintString1("down gate is on\r\n");
		}
		else
		{
			PrintString1("down gate is off\r\n");
		}
		os_wait(K_TMO, 10, 0);

	}

}
void printADCValue(void) _task_ 8
{
	for (;;)
	{
		DataScope_Get_Channel_Data(readGate_ADC_Value(LEFT_GATE), 1);
		DataScope_Get_Channel_Data(readGate_ADC_Value(RIGHT_GATE), 2);
		DataScope_Get_Channel_Data(readGate_ADC_Value(UP_GATE), 3);
		DataScope_Get_Channel_Data(readGate_ADC_Value(DOWN_GATE), 4);
		sendScopeData(4);


		os_wait(K_TMO, 10, 0);
	}
}
void systemGateADC_ValueUpdate(void) _task_ SYSTEM_GATE_ADC_UPDATE
{
	for (;;)
	{
		gateADC_ValueUpdate();
		os_wait(K_TMO, 20, 0);
	}
}
void systemGateStateUpdate(void) _task_ SYSTEM_GATE_STATE_UPDATE
{
	for (;;)
	{
		gateStateUpdate();
		os_wait(K_TMO, 20, 0);
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

