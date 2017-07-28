#ifndef _DIRECTION_H_
#define _DIRECTION_H_
#include "../../HARDWARE/BSP/config.h"
#define LEFT_GATE 0
#define RIGHT_GATE 1
#define UP_GATE 2
#define DOWN_GATE 3

typedef struct  
{
	u16 ADC_Value;
	u8 state;
}gateInfo;

void TrackingSystem_Init(void);
u16 readGate_ADC_Value(u8 gate);
u8  readGate(u8 gate);
void gateADC_ValueUpdate(void);
void gateStateUpdate(void);
#endif
