#ifndef __USART_2_H
#define __USART_2_H
#include "stdio.h"
#include "sys.h" 

typedef struct //欧拉角
{
	short Angle[3];
	short T;
}SAngle;

extern u8 USART_RX_BUF[64];     //接收缓冲,最大63个字节.末字节为换行符 
extern u8 USART_RX_STA;         //接收状态标记	


void Initial_UART2(unsigned long baudrate);
void UART2_Put_Char(unsigned char DataToSend);
void UART2_Put_String(unsigned char *Str);
void CopeSerial2Data( char ucData);
void sendcmd(char cmd[]);


#endif
