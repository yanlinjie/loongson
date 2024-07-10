#ifndef __TIM6_H
#define __TIM6_H

#include "sys.h"

//-----定时器相关宏定义-----//
#define Encoder_TIM_Period (u16)(65535)   //不可大于65535 因为F103的定时器是16位的 
#define Encoder_TIM_Psc			  0           //计数器分频
#define CNT_Init 							0           //计数器初值

//-----舵机接口定义-----//
#define ClipRegister 		TIM1->CCR1  			//抓夹舵机引脚
#define StuffRegister 	TIM1->CCR4  			//物料转盘舵机引脚

#define Stage_INIT			1100							//抓夹舵机初始位置
#define Stuff_INIT			1485							//物料盘舵机初始位置

void Tim1_Init(u16 arr,u16 psc);					//舵机控制
void Tim2_Init(u16 arr,u16 psc);					//灯环控制
void TIM3_Init(u16 arr,u16 psc);					//备用控制
void Tim6_Init(u16 arr,u16 psc);					//中断控制
void TIM7_Init(u16 arr,u16 psc);					//串口控制
void Tim8_Init(u32 arr,u32 psc);					//电机控制

#endif
