#include "led.h"

/**************************************************************************
函数功能：初始化板载LED引脚
入口参数: 无
返回  值：无
**************************************************************************/
void Board_LED_Init(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//使能PC端口时钟
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 			//LED0左黄-->PC0 端口配置
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			//推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;			//IO口速度为10MHz
	 GPIO_Init(GPIOC, &GPIO_InitStructure);					 				//根据设定参数初始化GPIOC0
	 GPIO_SetBits(GPIOC,GPIO_Pin_0);						 						//PC0输出高-灭
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	    		 		//LED1右蓝-->PC1 端口配置, 推挽输出
	 GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 			//推挽输出 ，IO口速度为10MHz
	 GPIO_SetBits(GPIOC,GPIO_Pin_1); 						 						//PC1输出高灭	
}
 
/**************************************************************************
函数功能：初始化外部LED引脚
入口参数: 无
返回  值：无
**************************************************************************/
void Load_LED_Init(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能PB端口时钟
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 			//PB8 端口配置
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			//推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;			//IO口速度为10MHz
	 GPIO_Init(GPIOB, &GPIO_InitStructure);					 				//根据设定参数初始化GPIOB 8
	 GPIO_ResetBits(GPIOB,GPIO_Pin_8);						 					//PB8输出低-灭
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	    		 		//PB9 端口配置, 推挽输出
	 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 			//推挽输出 ，IO口速度为10MHz
	 GPIO_ResetBits(GPIOB,GPIO_Pin_9); 						 					//PB9 输出低-灭	
	
	 LED2 = LED3 = LED_CLOSE_OUT; 													//关闭外部LED
}

/**************************************************************************
函数功能：初始化激光引脚
入口参数: 无
返回  值：无
**************************************************************************/
void Laser_Init(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能PB端口时钟
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 			//LED0左黄-->PB2 端口配置
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			//推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;			//IO口速度为10MHz
	 GPIO_Init(GPIOB, &GPIO_InitStructure);					 				//根据设定参数初始化GPIOB2
	 GPIO_SetBits(GPIOB,GPIO_Pin_2);						 						//PB2输出高-灭
}
