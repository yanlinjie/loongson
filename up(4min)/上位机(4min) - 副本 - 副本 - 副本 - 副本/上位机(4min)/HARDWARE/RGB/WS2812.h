#ifndef __WS2812_H
#define __WS2812_H

#include "sys.h" 

//-----------------颜色码定义------------------------//
typedef enum{
	Red,				//红
	Green,			//绿
	Blue,				//蓝
	White,			//白
	None				//关闭
}Color_WS2812;

#define LED_NUM  	 	12		//灯珠亮的个数
#define DATA_SIZE  	24 	  //WS2812B一个编码的bit数，3*8

#define WS2812B_ARR 90		//TIM2的自动重装载值
#define T0H 				30		//0编码高电平时间占1/3
#define T1H 				60		//1编码高电平时间占2/3

#define WS2812B_HIGH   GPIO_SetBits(GPIOA,GPIO_Pin_1)
#define WS2812B_LOW	   GPIO_ResetBits(GPIOA,GPIO_Pin_1)

void WS2812_DMA_Init(void);										//WS2812灯带DMA初始化
void WS2812B_Reset(void);											//复位灯带
void WS2812B_Write(u8 num,uint32_t GRB_Data);	//写数据编码
void PWM_WS2812B_Show(u8 num);								//点亮灯珠
void WS2812B_SetColor(Color_WS2812 Color);		//灯环整体设置颜色
void WS2812B_SetColor_N(u8 Num,uint8_t red,uint8_t green,uint8_t blue);	//灯环第n位设置颜色

#endif
