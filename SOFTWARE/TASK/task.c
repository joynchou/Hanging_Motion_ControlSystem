/************************************************************
* ��֯���ƣ�
* �ļ�����: K:\��Ƭ�����\���Ӵ���������\SOFTWARE\TASK\TASK.C
* ����:
* �汾:
* ����:     2017/07/06
* ����:  �����˶�����ϵͳ�������ƺ���
* ��ʷ�޸ļ�¼:
* <����> <ʱ��> <�汾 > <����>
*
***********************************************************/

#include "task.h"
#include "../COMMON_SOFTWARE/DATA_SCOPE/DataScope_DP.h"
#include "setup.h"
#include "../HARDWARE/BSP/USART1.h"
#include "../HARDWARE/DEVICES/SENSOR/ANGLE/ANGLE.h"
#include <math.h>

#define  BOARD_HEIGHT  100.0f	//���ӵĸ߶�  ����λ����
#define  BOARD_WIDTH   50.0f	//���ӵĿ���  ����λ����
#define  BEARING_DIA   10.0f	//��ת��е�ֱ������λ����


//ͷ�ļ��еĽṹ�嶨��
/*
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
static Coordinate g_TargetCoordinate,g_CurrentCoordinate; // �����ǰλ�ü�¼�ṹ��

//�������������ṹ��һͬ�ٹ���һ��ϵͳ��Ϣ�ṹ��
typedef struct
{
	  Coordinate g_TargetCoordinate;
	  Coordinate g_CurrentCoordinate;
	  Wire g_StepMotorWireLen[2];
	  u8 systemState;
}Info;


 */

static Info g_MotionSystemInfo; //�����˶�����ϵͳ��Ϣ�ṹ��
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
// Qualifier: //������Ҫ�����Ŀ������
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
// Qualifier: //��ȡ��ǰ���꣬����һ��xy�ṹ��
// Parameter: u8 motor
//************************************
Coordinate getCurrentCoordinate(void)
{

	return g_MotionSystemInfo.g_CurrentCoordinate;
}


///////////////////������Ϊ˽�к������ⲿ���ܵ��ã�///////////////////////////
//************************************
// Method:    setWireLength
// FullName:  setWireLength
// Access:    public 
// Returns:   bit
// Qualifier: //����a,b�ĳ���
// Parameter: u8 wire
//************************************
static bit setWireLength(u8 wire)
{
	if (wire)
	{
		//a���߳�����
		g_MotionSystemInfo.g_StepMotorWireLen[wire].Length=sqrt(	pow(g_MotionSystemInfo.g_TargetCoordinate.x,2)	            
																   +pow(BOARD_HEIGHT- g_MotionSystemInfo.g_TargetCoordinate.y,2)
															    ) ;
	}
	else
	{
		//b���߳�����
		g_MotionSystemInfo.g_StepMotorWireLen[wire].Length = sqrt(	  pow(BOARD_WIDTH-g_MotionSystemInfo.g_TargetCoordinate.x, 2)	
																	+ pow(BOARD_HEIGHT - g_MotionSystemInfo.g_TargetCoordinate.y, 2)
															     );
	}
}
static float getWireLength(u8 wire)
{
	return g_MotionSystemInfo.g_StepMotorWireLen[wire].Length;
}






