#include "ANGLE.h"
#include "../../../../SOFTWARE/ALGORITHM/FILTER/FILTER.H"
#define  POTENTIOMETER
#ifdef POTENTIOMETER
//ADC��ʼ������ p1^0
/*************************************************
* ��������:void    ADC_config(u8 pin,u8 speed)
* ����: ��ʼ��adc
* ���룺u8 pin:��Ҫʹ�õ�adc����
*       u8 speed: adc��ת���ٶ�
* ����˵��:
* pin:
*                ADC_P10
*                ADC_P11
*                ADC_P12
*                ADC_P13
*                ADC_P14
*                ADC_P15
*                ADC_P16
*                ADC_P17
* speed:
*                ADC_90T
*                ADC_180T
*                ADC_360T
*                ADC_540T
*************************************************/
void    ADC_config(u8 pin, u8 speed)
{
    ADC_InitTypeDef     ADC_InitStructure;
    ADC_InitStructure.ADC_Px = pin;
    ADC_InitStructure.ADC_Speed = speed;
    ADC_InitStructure.ADC_Power = ENABLE;
    ADC_InitStructure.ADC_AdjResult = ADC_RES_H8L2;
    ADC_InitStructure.ADC_Polity = PolityLow;
    ADC_InitStructure.ADC_Interrupt = DISABLE;
    ADC_Inilize(&ADC_InitStructure);
    //ADC_PowerControl(ENABLE);
}


/*************************************************
* ��������: float getAngle(bit whichAngle)
* ����:  ��ȡ��ǰ��ڽǶȻ�����һ�ζ�ȡ�ĽǶ�
* ����:  bit whichAngle
* ����ֵ: float �Ƕ�
* ����˵��:
  ���߷����Ѿ��̶��������ٸı�
whichAngle:
            PRESENT_ANGLE
            LAST_ANGLE
*************************************************/

float getAngle(bit whichAngle)
{
    u16 angleData[10];
	u8  angleDataloopTemp1=0;
    static float presentAngle, lastAngle;  //�˺��������Է�����һ�εĽǶ�ֵ������Ҫ��ȡʱ�� ���Ǿ�̬�����ݣ���Ϊ��Ҫ����
    if (whichAngle)  //���ص�ǰ���
    {
    
		for(angleDataloopTemp1 = 0; angleDataloopTemp1 < 10; angleDataloopTemp1++) //��ȡ10�����ݲ�������������
			{
				angleData[angleDataloopTemp1] =  Get_ADC10bitResult(ADC_CH0);
			}
	     presentAngle= filter(angleData,10,1024,0);
        
        lastAngle = presentAngle;
        return presentAngle;
    }
    else  //������һ�ν��
    {
        return  lastAngle;
    }
}
#endif