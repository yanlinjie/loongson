#ifndef __USART_2_H
#define __USART_2_H
#include "stdio.h"
#include "sys.h" 

typedef struct //ŷ����
{
	short Angle[3];
	short T;
}SAngle;

extern u8 USART_RX_BUF[64];     //���ջ���,���63���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8 USART_RX_STA;         //����״̬���	


void Initial_UART2(unsigned long baudrate);
void UART2_Put_Char(unsigned char DataToSend);
void UART2_Put_String(unsigned char *Str);
void CopeSerial2Data( char ucData);
void sendcmd(char cmd[]);


#endif
