#ifndef __KEY_H
#define __KEY_H

#include "sys.h"

/*--------------- 按键状态 -----------------------*/
typedef enum{
	KeyShort,KeyLong,KeySlow
}KeyState;

/*--------------- 函数声明 -----------------------*/
void KEY_Init(void);	//按键IO口初始化函数
u8 KEY_Scan(void);   	//按键扫描

#endif
