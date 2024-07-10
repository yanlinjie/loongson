#include "key.h"  

/**************************************************************************
函数功能：按键IO口初始化
入口参数：无
返回  值：无
注意事项：当引脚设置为任意输入时，配置引脚速度无意义
**************************************************************************/
void KEY_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure; 										//定义结构体
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 	//初始化KEY时钟	
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	 
	
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
}

/*************************************************************************************************
*	函 数 名:	KEY_Scan
*	返 回 值:	KeyShort：短按  KeyLong：长按  KeySlow：松开
*	函数功能:	按键扫描
*************************************************************************************************/
u8 KEY_Scan(void)
{
	static unsigned int KeyTime;
	delay_ms(5);			
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2) == Bit_RESET)		//检测按键是否被按下
	{	
		delay_ms(4);																							//延时消抖
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2) == Bit_RESET)	//再次检测是否为低电平
			KeyTime++;
	}
	else if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2) == Bit_SET)	//如果按键松开
	{
		if(KeyTime>3 && KeyTime<50){	//短按												
			KeyTime = 0;
			return KeyShort;
		}
		else if(KeyTime > 120){				//长按			
			KeyTime = 0;
			return KeyLong;
		} 		
	}
	return KeySlow;									//松开
}
