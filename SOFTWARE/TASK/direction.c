#include "direction.h"
#include "TASK.h"
#include "../COMMON_SOFTWARE/DATA_SCOPE/DataScope_DP.h"
#include "setup.h"
#include "../HARDWARE/BSP/USART1.h"
#include "../../HARDWARE/COMMON_HARDWARE/fixedPulser.h"
#include "../../HARDWARE/BSP/delay.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../HARDWARE/BSP/GPIO.h"
#include "../../HARDWARE/BSP/ADC.h"
#include "../ALGORITHM/FILTER/FILTER.H"
static gateInfo  gateState[4];

//寻线系统初始化
//************************************
// Method:    TrackingSystem_Init
// FullName:  TrackingSystem_Init
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void TrackingSystem_Init(void)
{
	//GPIO_InitTypeDef photogateGPIO;
	ADC_InitTypeDef photogateADC;
	//photogateGPIO.Mode = GPIO_PullUp;
	//photogateGPIO.Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	//GPIO_Inilize(GPIO_P5, &photogateGPIO);
	//photogateGPIO.Pin = GPIO_Pin_0 ;
	//GPIO_Inilize(GPIO_P4, &photogateGPIO);
	photogateADC.ADC_Px =ADC_P10| ADC_P11| ADC_P12| ADC_P13;
	photogateADC.ADC_Speed = ADC_90T;
	photogateADC.ADC_Power = ENABLE;
	photogateADC.ADC_AdjResult = ADC_RES_H8L2;
	photogateADC.ADC_Polity = PolityLow;
	photogateADC.ADC_Interrupt = DISABLE;
	ADC_Inilize(&photogateADC);
  //ADC_PowerControl(ENABLE);

}
u16 readGate_ADC_Value(u8 gate)
{
	return gateState[gate].ADC_Value;
}
//************************************
// Method:    readGate
// FullName:  readGate
// Access:    public 
// Returns:   u16
// Qualifier:
// Parameter: u8 gate
//************************************
u8  readGate(u8 gate)
{
	  return gateState[gate].state; 
}
//************************************
// Method:    gateADC_ValueUpdate
// FullName:  gateADC_ValueUpdate
// Access:    public 
// Returns:   void
// Qualifier: 更新adc的值并滤波
// Parameter: void
//************************************
void gateADC_ValueUpdate(void)
{
	u8 tmp = 10;
	u8 loopTmp = 0;
	u16 valueTmp[10];
	for (loopTmp = 0; loopTmp <tmp; loopTmp++)
	{
		valueTmp[loopTmp] = Get_ADC10bitResult(LEFT_GATE);
	}
	gateState[LEFT_GATE].ADC_Value =filter(valueTmp, tmp,1024,0);


	for (loopTmp = 0; loopTmp < tmp; loopTmp++)
	{
		valueTmp[loopTmp] = Get_ADC10bitResult(RIGHT_GATE);
	}
	gateState[RIGHT_GATE].ADC_Value = filter(valueTmp, tmp, 1024, 0);


	for (loopTmp = 0; loopTmp < tmp; loopTmp++)
	{
		valueTmp[loopTmp] = Get_ADC10bitResult(UP_GATE);
	}
	gateState[UP_GATE].ADC_Value = filter(valueTmp, tmp, 1024, 0);


	for (loopTmp = 0; loopTmp < tmp; loopTmp++)
	{
		valueTmp[loopTmp] = Get_ADC10bitResult(DOWN_GATE);
	}
	gateState[DOWN_GATE].ADC_Value = filter(valueTmp, tmp, 1024, 0);
}
//************************************
// Method:    gateStateUpdate
// FullName:  gateStateUpdate
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void gateStateUpdate(void)
{
	if (readGate_ADC_Value(LEFT_GATE)>409)
	{
		gateState[LEFT_GATE].state = ON;
	}
	else
	{
		gateState[LEFT_GATE].state = OFF;
	}
	if (readGate_ADC_Value(RIGHT_GATE)>241)
	{
		gateState[RIGHT_GATE].state = ON;
	}
	else
	{
		gateState[RIGHT_GATE].state = OFF;
	}

	if (readGate_ADC_Value(UP_GATE)>322)
	{
		gateState[UP_GATE].state = ON;
	}
	else
	{
		gateState[UP_GATE].state = OFF;
	}
	if (readGate_ADC_Value(DOWN_GATE)>337)
	{
		gateState[DOWN_GATE].state = ON;
	}
	else
	{
		gateState[DOWN_GATE].state = OFF;
	}

	
}
