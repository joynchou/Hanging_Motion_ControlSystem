/************************************************************
* 组织名称：
* 文件名称: K:\单片机相关\电子大赛程序框架\SOFTWARE\TASK\SETUP.C
* 作者:     周晨阳
* 版本:
* 日期:     2017/07/18
* 描述:		初始化器件和测试器件使用
* 历史修改记录:
* <作者> <时间> <版本 > <描述>
*
***********************************************************/

#include "setup.h"
#include "task.h"
#include "../../HARDWARE/DEVICES/LED/LED.h"
#include "../../HARDWARE/BSP/USART1.h"
#include "../../HARDWARE/BSP/timer.h"
#include "../../HARDWARE/BSP/GPIO.h"
#include "../../HARDWARE/BSP/delay.h"
#include "../../HARDWARE/BSP/STC15_PWM.h"
#include "../../HARDWARE/DEVICES/MOTOR/STEP_MOTOR/STEP_MOTOR.h"
#include "../../HARDWARE/DEVICES/DISPLAY/LCD5110/LCD5510_V2.H"
#include "../../HARDWARE/COMMON_HARDWARE/fixedPulser.h"
#include "direction.h"



/*************  串口1初始化函数 *****************/
/*************************************************
* 函数名称: void    UART1_init(unsigned long BandRate)
* 描述: 串口1初始化函数
* 输入: unsigned long BandRate ：串口的波特率，请在数值后加字母"L"，防止出问题
* 输出: 调用此函数会发送一段测试语句 “ STC15w4k58s4 UART1 is open ”
* 返回值: 无
* 其他说明: 无
*************************************************/
void    UART1_init(unsigned long BandRate)
{
	COMx_InitDefine     COMx_InitStructure;
	COMx_InitStructure.UART_Mode = UART_8bit_BRTx;
	COMx_InitStructure.UART_BRT_Use = BRT_Timer1;
	COMx_InitStructure.UART_BaudRate = BandRate;
	COMx_InitStructure.UART_RxEnable = ENABLE;
	COMx_InitStructure.BaudRateDouble = DISABLE;
	COMx_InitStructure.UART_Interrupt = ENABLE;
	COMx_InitStructure.UART_Polity = PolityHigh;
	COMx_InitStructure.UART_P_SW = UART1_SW_P30_P31;
	COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;
	USART_Configuration(USART1, &COMx_InitStructure);
	PrintString1("STC15w4k58s4's UART1 is open \r\n");   //串口测试
}
void test(void)//器件测试
{
	
}
void setup(void)
{

	UART1_init(115200L);
	timerInit();  //定时器初始化
	Board_LED_Init();
	MotionSystem_Init();
	TrackingSystem_Init();
	
	//test();  //测试

}

