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
#include "../HARDWARE/BSP/stc15_pwm.h"
#include "../HARDWARE/BSP/GPIO.h"


//寻线系统初始化
void TrackingSystem_Init(void)
{
	GPIO_InitTypeDef photogateGPIO;
	photogateGPIO.Mode = GPIO_PullUp;
	photogateGPIO.Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_Inilize(GPIO_P5, &photogateGPIO);
	photogateGPIO.Pin = GPIO_Pin_0 ;
	GPIO_Inilize(GPIO_P4, &photogateGPIO);

}

