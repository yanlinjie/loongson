#include "uart_2.h"

static unsigned char TxBuffer[256];
static unsigned char TxCounter=0;
static unsigned char count=0; 
 SAngle 	stcAngle;

// uart_2 ----------------------------------------------------------------------
void Initial_UART2(unsigned long baudrate)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
 	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	 
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure); 
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);    
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	USART_ClearFlag(USART2,USART_FLAG_TC);	
	   USART_Cmd(USART2, DISABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TLY_PP;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = TLY_SubP;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void USART2_IRQHandler(void)
{
  if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
  {   
    USART_SendData(USART2, TxBuffer[TxCounter++]); 
    USART_ClearITPendingBit(USART2, USART_IT_TXE);
    if(TxCounter == count) USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
  }
	else if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {
		CopeSerial2Data((unsigned char)USART2->DR);//处理数据
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
  }
	
	USART_ClearITPendingBit(USART2,USART_IT_ORE);
}
//用串口2给JY模块发送指令
void sendcmd(char cmd[])
{
	char i;
	for(i=0;i<5;i++)
		UART2_Put_Char(cmd[i]);
}

//CopeSerialData为串口2中断调用函数，串口每收到一个数据，调用一次这个函数。
void CopeSerial2Data( char ucData)
{
	static  char ucRxBuffer[30];
	static  char ucRxCnt = 0;	
	
	ucRxBuffer[ucRxCnt++]=ucData;	//将收到的数据存入缓冲区中
	if (ucRxBuffer[0]!=0x55) //数据头不对，则重新开始寻找0x55数据头
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) {return;}//数据不满11个，则返回
	else
	{
		if(ucRxBuffer[1]==0x53) memcpy(&stcAngle,&ucRxBuffer[2],8);
		ucRxCnt=0;//清空缓存区
	}
}

void UART2_Put_Char(unsigned char DataToSend)
{
	TxBuffer[count++] = DataToSend;  
  USART_ITConfig(USART2, USART_IT_TXE, ENABLE);  
}

void UART2_Put_String(unsigned char *Str)
{
	while(*Str)
	{
		if(*Str=='\r')UART2_Put_Char(0x0d);
			else if(*Str=='\n')UART2_Put_Char(0x0a);
				else UART2_Put_Char(*Str);
		Str++;
	}
}
