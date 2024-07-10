#ifndef __LED_H
#define __LED_H	 

#include "sys.h"


//-----------------板载LED控制通用定义------------------------//
#define LED_OPEN 	 0							//低电平亮
#define LED_CLOSE  1							//高电平灭

//-----------------外部LED控制通用定义------------------------//
#define LED_CLOSE_OUT	 0					//低电平灭
#define LED_OPEN_OUT   1					//高电平亮

//-----------------板载LED引脚宏定义------------------------//
#define LED0 PCout(0)							// PC0
#define LED1 PCout(1)							// PC1	

//-----------------外部LED引脚定义------------------------//
#define LED2 PBout(8)							// PB8
#define LED3 PBout(9)							// PB9	

//-----------------激光引脚定义------------------------//
#define Laser PBout(2)						// PB2

//-----------------板载LED函数定义------------------------//
void Board_LED_Init(void);				//初始化板载LED引脚

//-----------------外部LED函数定义------------------------//
void Load_LED_Init(void);					//初始化外部LED引脚

//-----------------激光函数定义------------------------//
void Laser_Init(void);						//初始化激光引脚

#endif
