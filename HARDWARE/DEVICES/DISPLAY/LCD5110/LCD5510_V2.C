#include "LCD5510_V2.H"
#include "../../../BSP/GPIO.h"
#include "../../../BSP/USART1.h"
#include <stdlib.h>
#include <stdio.h>


 extern unsigned char code Font_code[][6] = {
{0x00,0x00,0x00,0x00,0x00,0x00},// (0)
{0x00,0x00,0x00,0x4F,0x00,0x00},//!(1)
{0x00,0x00,0x07,0x00,0x07,0x00},//"(2)
{0x00,0x14,0x7F,0x14,0x7F,0x14},//#(3)
{0x00,0x24,0x2A,0x7F,0x2A,0x12},//$(4)
{0x00,0x23,0x13,0x08,0x64,0x62},//%(5)
{0x00,0x36,0x49,0x55,0x22,0x50},//&(6)
{0x00,0x00,0x05,0x03,0x00,0x00},//'(7)
{0x00,0x00,0x1C,0x22,0x41,0x00},//((8)
{0x00,0x00,0x41,0x22,0x1C,0x00},//)(9)
{0x00,0x14,0x08,0x3E,0x08,0x14},//*(10)
{0x00,0x08,0x08,0x3E,0x08,0x08},//+(11)
{0x00,0x00,0x50,0x30,0x00,0x00},//,(12)
{0x00,0x08,0x08,0x08,0x08,0x08},//-(13)
{0x00,0x00,0x60,0x60,0x00,0x00},//.(14)
{0x00,0x20,0x10,0x08,0x04,0x02},///(15)
{0x00,0x3E,0x51,0x49,0x45,0x3E},//0(16)
{0x00,0x00,0x42,0x7F,0x40,0x00},//1(17)
{0x00,0x42,0x61,0x51,0x49,0x46},//2(18)
{0x00,0x21,0x41,0x45,0x4B,0x31},//3(19)
{0x00,0x18,0x14,0x12,0x7F,0x10},//4(20)
{0x00,0x27,0x45,0x45,0x45,0x39},//5(21)
{0x00,0x3C,0x4A,0x49,0x49,0x30},//6(22)
{0x00,0x01,0x71,0x09,0x05,0x03},//7(23)
{0x00,0x36,0x49,0x49,0x49,0x36},//8(24)
{0x00,0x06,0x49,0x49,0x29,0x1E},//9(25)
{0x00,0x00,0x36,0x36,0x00,0x00},//:(26)
{0x00,0x00,0x56,0x36,0x00,0x00},//;(27)
{0x00,0x08,0x14,0x22,0x41,0x00},//<(28)
{0x00,0x14,0x14,0x14,0x14,0x14},//=(29)
{0x00,0x00,0x41,0x22,0x14,0x08},//>(30)
{0x00,0x02,0x01,0x51,0x09,0x06},//?(31)
{0x00,0x32,0x49,0x79,0x41,0x3E},//@(32)
{0x00,0x7E,0x11,0x11,0x11,0x7E},//A(33)
{0x00,0x7F,0x49,0x49,0x49,0x3E},//B(34)
{0x00,0x3E,0x41,0x41,0x41,0x22},//C(35)
{0x00,0x7F,0x41,0x41,0x22,0x1C},//D(36)
{0x00,0x7F,0x49,0x49,0x49,0x41},//E(37)
{0x00,0x7F,0x09,0x09,0x09,0x01},//F(38)
{0x00,0x3E,0x41,0x49,0x49,0x7A},//G(39)
{0x00,0x7F,0x08,0x08,0x08,0x7F},//H(40)
{0x00,0x00,0x41,0x7F,0x41,0x00},//I(41)
{0x00,0x20,0x40,0x41,0x3F,0x01},//J(42)
{0x00,0x7F,0x08,0x14,0x22,0x41},//K(43)
{0x00,0x7F,0x40,0x40,0x40,0x40},//L(44)
{0x00,0x7F,0x02,0x04,0x02,0x7F},//M(45)
{0x00,0x7F,0x04,0x08,0x10,0x7F},//N(46)
{0x00,0x3E,0x41,0x41,0x41,0x3E},//O(47)
{0x00,0x7F,0x09,0x09,0x09,0x06},//P(48)
{0x00,0x3E,0x41,0x51,0x21,0x5E},//Q(49)
{0x00,0x7F,0x09,0x19,0x29,0x46},//R(50)
{0x00,0x46,0x49,0x49,0x49,0x31},//S(51)
{0x00,0x01,0x01,0x7F,0x01,0x01},//T(52)
{0x00,0x3F,0x40,0x40,0x40,0x3F},//U(53)
{0x00,0x1F,0x20,0x40,0x20,0x1F},//V(54)
{0x00,0x3F,0x40,0x38,0x40,0x3F},//W(55)
{0x00,0x63,0x14,0x08,0x14,0x63},//X(56)
{0x00,0x03,0x04,0x78,0x04,0x03},//Y(57)
{0x00,0x61,0x51,0x49,0x45,0x43},//Z(58)
{0x00,0x00,0x7F,0x41,0x41,0x00},//[(59)
{0x00,0x15,0x16,0x7C,0x16,0x15},//\(60)
{0x00,0x00,0x41,0x41,0x7F,0x00},//](61)
{0x00,0x04,0x02,0x01,0x02,0x04},//^(62)
{0x00,0x40,0x40,0x40,0x40,0x40},//_(63)
{0x00,0x00,0x01,0x02,0x04,0x00},//`(64)
{0x00,0x20,0x54,0x54,0x54,0x78},//a(65)
{0x00,0x7F,0x48,0x44,0x44,0x38},//b(66)
{0x00,0x38,0x44,0x44,0x44,0x20},//c(67)
{0x00,0x38,0x44,0x44,0x48,0x7F},//d(68)
{0x00,0x38,0x54,0x54,0x54,0x18},//e(69)
{0x00,0x08,0x7E,0x09,0x01,0x02},//f(70)
{0x00,0x0C,0x52,0x52,0x52,0x3E},//g(71)
{0x00,0x7F,0x08,0x04,0x04,0x78},//h(72)
{0x00,0x00,0x44,0x7D,0x40,0x00},//i(73)
{0x00,0x20,0x40,0x44,0x3D,0x00},//j(74)
{0x00,0x7F,0x10,0x28,0x44,0x00},//k(75)
{0x00,0x00,0x41,0x7F,0x40,0x00},//l(76)
{0x00,0x7E,0x02,0x0C,0x02,0x7C},//m(77)
{0x00,0x7E,0x04,0x02,0x02,0x7C},//n(78)
{0x00,0x38,0x44,0x44,0x44,0x38},//o(79)
{0x00,0x7C,0x14,0x14,0x14,0x08},//p(80)
{0x00,0x08,0x14,0x14,0x18,0x7C},//q(81)
{0x00,0x7C,0x08,0x04,0x04,0x08},//r(82)
{0x00,0x48,0x54,0x54,0x54,0x20},//s(83)
{0x00,0x04,0x3F,0x44,0x40,0x20},//t(84)
{0x00,0x3C,0x40,0x40,0x20,0x7C},//u(85)
{0x00,0x1C,0x20,0x40,0x20,0x1C},//v(86)
{0x00,0x3C,0x40,0x30,0x40,0x3C},//w(87)
{0x00,0x44,0x28,0x10,0x28,0x44},//x(88)
{0x00,0x0C,0x50,0x50,0x50,0x3C},//y(89)
{0x00,0x44,0x64,0x54,0x4C,0x44},//z(90)
{0x00,0x00,0x08,0x36,0x41,0x00},//{(91)
{0x00,0x00,0x00,0x7F,0x00,0x00},//|(92)
{0x00,0x00,0x41,0x36,0x08,0x00},//}(93)
{0x00,0x08,0x04,0x08,0x10,0x08},//~(94)
{0x00,0x08,0x08,0x2A,0x1C,0x08},//→(127)
{0x00,0x08,0x1C,0x2A,0x08,0x08},//←(128)
{0x00,0x04,0x02,0x7F,0x02,0x04},//↑(129)
{0x00,0x10,0x20,0x7F,0x20,0x10},//↓(130)
{0x00,0x1C,0x2A,0x32,0x2A,0x1C},//笑面(131)
{0x00,0x1C,0x22,0x44,0x22,0x1C}};//爱心(132)

/*--------------------------------------------------------------*/
//写入一个字节(数据或指令)
//wbyte:	待写入的数据
//dat_cmd:	1-数据, 0-指令
void LCD_write_byte(unsigned char wbyte, unsigned char dat_cmd)
{
	unsigned char i;
	
	LCD_CS = 0;			//使能
	LCD_DC = dat_cmd;	//1-数据	//0-指令

	for(i = 8; i; i--)	//8位数据, 先高后低
	{
		if(wbyte & 0x80) {LCD_DIN = 1;}
		else			 {LCD_DIN = 0;}
		
		LCD_CLK = 0;
		wbyte <<= 1;	//移位(延时)
		LCD_CLK = 1;	//上升沿写入
	}
	
	LCD_CS = 1;			//禁止
}


/*--------------------------------------------------------------*/
//写入n个字节(数据)(ROM)
//*wbyte:	待写入的数据指针
//num:		待写入的数据个数
/*void LCD_write_nbyte(unsigned char code *wbyte, unsigned char num)
{
	unsigned char i, j;
	unsigned char temp;

	LCD_CS = 0;			//使能
	LCD_DC = 1;			//数据

	for(j = 0; j < num; j++)//num个数据
	{
		temp = wbyte[j];
		for(i = 8; i; i--)	//8位数据, 先高后低
		{
			if(temp & 0x80) {LCD_DIN = 1;}
			else			{LCD_DIN = 0;}
		
			LCD_CLK = 0;
			temp <<= 1;		//移位(延时)
			LCD_CLK = 1;	//上升沿写入
		}	
	}
	
	LCD_CS = 1;			//禁止
}


/*--------------------------------------------------------------*/
//显示清屏(清DDRAM)
void LCD_clr_scr(void)
{
	unsigned int i;
	
	LCD_write_cmd(X_Col_Addr);
	LCD_write_cmd(Y_Page_Addr);
	
	for(i = 504; i; i--) LCD_write_dat(0x00);
}


/*--------------------------------------------------------------*/
//显示清行
//num:	0 - 5
void LCD_clr_row(unsigned char num)
{
	unsigned char i;
	
	LCD_pos_byte(0, num);
	for(i = 84; i; i--) LCD_write_dat(0x00);
}


/*--------------------------------------------------------------*/
//液晶字节定位(1*1)
//液晶规划:
//x: 0 - 83
//y: 0 - 5
void LCD_pos_byte(unsigned char x, unsigned char y)
{
	x |= X_Col_Addr;  
	y |= Y_Page_Addr; 
	LCD_write_cmd(x);	//列地址
	LCD_write_cmd(y);	//页地址
}


/*--------------------------------------------------------------*/
//液晶字符输出(6*8字体)
//x: 0 - 13
//y: 0 - 5
void LCD_printc(unsigned char x, unsigned char y, unsigned char c_dat)
{
	unsigned char i;

	c_dat -= 32;		//查表
	x *= 6;				//宽6

	LCD_pos_byte(x, y);	//坐标
	for(i = 0; i < 6; i++) LCD_write_dat(Font_code[c_dat][i]);	
}


/*--------------------------------------------------------------*/
//液晶字符串输出(6*8字体)
//x: 0 - 13
//y: 0 - 5
void LCD_prints(unsigned char x, unsigned char y, unsigned char *s_dat)
{
	while(*s_dat && x < 14) {LCD_printc(x++, y, *s_dat); s_dat++;}
}


/*--------------------------------------------------------------*/
//液晶字符串输出,自动换行(6*8字体)
//x: 0 - 13
//y: 0 - 5
void LCD_printsl(unsigned char x, unsigned char y, unsigned char *s_dat)
{
	while(*s_dat) 
	{
		LCD_printc(x++, y, *s_dat); 
		s_dat++;
		if(x == 14) {x = 0; y++;}
		if(y == 6)  {y = 0;}
	}
}


/*--------------------------------------------------------------*/
//复位LCD5510
/*void LCD_reset_5510(void)
{
	LCD_clr_scr();				//清全屏
	
	LCD_write_cmd(0x25);		//省电模式, 水平寻址, 扩展指令
	LCD_write_cmd(0x04);		//VLCD温度系数0
	LCD_write_cmd(0x10);		//设置偏置系统(BSx)
	LCD_write_cmd(0xc0);		//设置电压VLCD = 3.06 + 0.06*Vop;
	
	LCD_write_cmd(0x24);		//省电模式, 水平寻址, 常规指令
	LCD_write_cmd(0x08);		//显示空白
	LCD_write_cmd(Y_Page_Addr);	//起始页地址0
	LCD_write_cmd(X_Col_Addr);	//起始列地址0
}


/*--------------------------------------------------------------*/
//初始化LCD5510
void LCD5510_Init(void)
{  
	GPIO_InitTypeDef    GPIO_InitStructure;     //设置管脚
    GPIO_InitStructure.Mode = GPIO_PullUp;
   	GPIO_InitStructure.Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;    //lcd5110
	GPIO_Inilize(GPIO_P5, &GPIO_InitStructure);  
	LCD_reset_hard;				//硬件复位
//	LCD_reset_soft;				//软件复位

	LCD_write_cmd(0x21);		//工作模式, 水平寻址, 扩展指令
	LCD_write_cmd(0x06);		//VLCD温度系数2
	LCD_write_cmd(0x13);		//设置偏置系统(BSx) 1:48
	LCD_write_cmd(0xc8);		//设置电压VLCD = 3.06 + 0.06*Vop, 对比度调整

	LCD_write_cmd(0x20);		//工作模式, 水平寻址, 常规指令
	LCD_write_cmd(0x0c);		//普通模式
	LCD_write_cmd(Y_Page_Addr);	//起始页地址0
	LCD_write_cmd(X_Col_Addr);	//起始列地址0
	LCD_clr_scr();				//清全屏
	PrintString1("LCD5110 was initialized \r\n");
}

/*--------------------------------------------------------------*/
	




 
