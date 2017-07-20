/************************************************************
* 组织名称： 电子大赛小组
* 文件名称: DataScope_DP.c
* 作者:  未知
* 版本 :1.1
* 日期:
* 主要功能 :
* 向上位机发送数据的文件
* 历史修改记录: // 历史修改记录
* <作者> <时间> <版本 > <描述>
***********************************************************/
/***************串口示波器驱动代码************************/
#include "DataScope_DP.h"
#include "../../../HARDWARE/BSP/USART1.h"
#define DATA_SCOPE
#ifdef DATA_SCOPE

unsigned char DataScope_OutPut_Buffer[42] = {0};       //串口发送缓冲区
//函数说明：将单精度浮点数据转成4字节数据并存入指定地址
//附加说明：用户无需直接操作此函数
//target:目标单精度数据
//buf:待写入数组
//beg:指定从数组第几个元素开始写入
//函数无返回
//对此函数进行了修改，原本是有问题的，后来发现point数组的顺序反了，现在能正常使用了
 void Float2Byte(float *target,unsigned char *buf,unsigned char beg)
{
  unsigned char *point;
  point = (unsigned char*)target;   //得到float的地址
  buf[beg]   = point[3];
  buf[beg+1] = point[2];
  buf[beg+2] = point[1];
  buf[beg+3] = point[0];
}
 /*************************************************
 * 函数名称:
 * 描述:
 * 输入:
 * 输出:
 * 返回值:
 * 其他说明:
 *************************************************/
 void sendScopeData(float Data, u8 channel)
 {
	 u8 a; //

	 DataScope_Get_Channel_Data(Data, channel);

	 for (a = 0; a < DataScope_Data_Generate(channel); a++)
	 {
		 TX1_write2buff(DataScope_OutPut_Buffer[a]); //发送一通道数据到串口示波器
	 }

 }

//函数说明：将待发送通道的单精度浮点数据写入发送缓冲区
//Data：通道数据
//Channel：选择通道（1-10）
//函数无返回
void DataScope_Get_Channel_Data(float Data,unsigned char Channel)
{
  if ( (Channel > 10) || (Channel == 0) )
    {
      return;    //通道个数大于10或等于0，直接跳出，不执行函数
    }
  else
    {
      switch (Channel)
        {
          case 1:
            Float2Byte(&Data,DataScope_OutPut_Buffer,1);
            break;
          case 2:
            Float2Byte(&Data,DataScope_OutPut_Buffer,5);
            break;
          case 3:
            Float2Byte(&Data,DataScope_OutPut_Buffer,9);
            break;
          case 4:
            Float2Byte(&Data,DataScope_OutPut_Buffer,13);
            break;
          case 5:
            Float2Byte(&Data,DataScope_OutPut_Buffer,17);
            break;
          case 6:
            Float2Byte(&Data,DataScope_OutPut_Buffer,21);
            break;
          case 7:
            Float2Byte(&Data,DataScope_OutPut_Buffer,25);
            break;
          case 8:
            Float2Byte(&Data,DataScope_OutPut_Buffer,29);
            break;
          case 9:
            Float2Byte(&Data,DataScope_OutPut_Buffer,33);
            break;
          case 10:
            Float2Byte(&Data,DataScope_OutPut_Buffer,37);
            break;
        }
    }
}
//函数说明：生成 DataScopeV1.0 能正确识别的帧格式
//Channel_Number，需要发送的通道个数
//返回发送缓冲区数据个数
//返回0表示帧格式生成失败
unsigned char DataScope_Data_Generate(unsigned char Channel_Number)
{
  if ( (Channel_Number > 10) || (Channel_Number == 0) )
    {
      return 0;    //通道个数大于10或等于0，直接跳出，不执行函数
    }
  else
    {
      DataScope_OutPut_Buffer[0] = '$';  //帧头
      switch(Channel_Number)
        {
          case 1:
            DataScope_OutPut_Buffer[5]  =  5;
            return  6;
            break;
          case 2:
            DataScope_OutPut_Buffer[9]  =  9;
            return 10;
            break;
          case 3:
            DataScope_OutPut_Buffer[13] = 13;
            return 14;
            break;
          case 4:
            DataScope_OutPut_Buffer[17] = 17;
            return 18;
            break;
          case 5:
            DataScope_OutPut_Buffer[21] = 21;
            return 22;
            break;
          case 6:
            DataScope_OutPut_Buffer[25] = 25;
            return 26;
            break;
          case 7:
            DataScope_OutPut_Buffer[29] = 29;
            return 30;
            break;
          case 8:
            DataScope_OutPut_Buffer[33] = 33;
            return 34;
            break;
          case 9:
            DataScope_OutPut_Buffer[37] = 37;
            return 38;
            break;
          case 10:
            DataScope_OutPut_Buffer[41] = 41;
            return 42;
            break;
        }
    }
  return 0;
}
#endif
