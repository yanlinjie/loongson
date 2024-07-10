#include "usart.h"	 

Usart2_Control U2Set;
Usart3_Control U3Set;
Uart4_Control  U4Set;
Uart5_Control  U5Set;

/**************************************************************************
函数功能：串口1初始化配置
入口参数: 无 
返回  值：无
**************************************************************************/
void USART_Config_1(u32 bound){ 
		GPIO_InitTypeDef 	GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef 	NVIC_InitStructure;
		// 打开串口 GPIO 的时钟
		RCC_APB2PeriphClockCmd(DEBUG_USART_GPIO_CLK_1, ENABLE);
		// 打开串口外设的时钟
		RCC_APB2PeriphClockCmd(DEBUG_USART_CLK_1, ENABLE);
		// 将 USART Tx 的 GPIO 配置为推挽复用模式
		GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN_1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(DEBUG_USART_TX_PORT_1, &GPIO_InitStructure);
		// 将 USART Rx 的 GPIO 配置为浮空输入模式
		GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN_1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(DEBUG_USART_RX_PORT_1, &GPIO_InitStructure);
		// 配置串口的工作参数
		// 配置波特率
		USART_InitStructure.USART_BaudRate = bound;
		// 配置 针数据字长
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		// 配置停止位
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		// 配置校验位
		USART_InitStructure.USART_Parity = USART_Parity_No ;
		// 配置硬件流控制
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		// 配置工作模式，收发一起
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		// 完成串口的初始化配置
		USART_Init(DEBUG_USART1, &USART_InitStructure);
		//------串口中断优先级配置------//
		// 配置 USART 为中断源
		NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ_1;
		// 抢断优先级
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		// 子优先级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		// 使能中断
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		// 初始化配置 NVIC
		NVIC_Init(&NVIC_InitStructure);
		// 使能串口接收中断
		USART_ITConfig(DEBUG_USART1, USART_IT_RXNE, ENABLE);
		// 使能串口
		USART_Cmd(DEBUG_USART1, ENABLE);
}

/**************************************************************************
函数功能：串口2初始化配置
入口参数: 无 
返回  值：无
**************************************************************************/
void USART_Config_2(u32 bound){ 
		GPIO_InitTypeDef 	GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef 	NVIC_InitStructure;
		// 打开串口 GPIO 的时钟
		RCC_APB2PeriphClockCmd(DEBUG_USART_GPIO_CLK_2,ENABLE);
		// 打开串口外设的时钟
		RCC_APB1PeriphClockCmd(DEBUG_USART_CLK_2, ENABLE);
		// 将 USART Tx 的 GPIO 配置为推挽复用模式
		GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN_2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(DEBUG_USART_TX_PORT_2, &GPIO_InitStructure); 
		// 将 USART Rx 的 GPIO 配置为浮空输入模式
		GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN_2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(DEBUG_USART_RX_PORT_2, &GPIO_InitStructure);
		// 配置串口的工作参数
		// 配置波特率
		USART_InitStructure.USART_BaudRate = bound;
		// 配置 针数据字长
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		// 配置停止位
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		// 配置校验位
		USART_InitStructure.USART_Parity = USART_Parity_No ;
		// 配置硬件流控制
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		// 配置工作模式，收发一起
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		// 完成串口的初始化配置
		USART_Init(DEBUG_USART2, &USART_InitStructure);
		//------串口中断优先级配置------//		
		// 配置 USART 为中断源
		NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ_2;
		// 抢断优先级
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		// 子优先级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		// 使能中断
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		// 初始化配置 NVIC
		NVIC_Init(&NVIC_InitStructure);
		// 使能串口接收中断
		USART_ITConfig(DEBUG_USART2, USART_IT_RXNE, ENABLE);
		// 使能串口
		USART_Cmd(DEBUG_USART2, ENABLE);
}

/**************************************************************************
函数功能：串口3初始化配置
入口参数: 无 
返回  值：无
**************************************************************************/
void USART_Config_3(u32 bound){ 
		GPIO_InitTypeDef 	GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef 	NVIC_InitStructure;
		// 打开串口 GPIO 的时钟
		RCC_APB2PeriphClockCmd(DEBUG_USART_GPIO_CLK_3,ENABLE); 
		// 打开串口外设的时钟
		RCC_APB1PeriphClockCmd(DEBUG_USART_CLK_3,ENABLE);
		//复位串口3
		USART_DeInit(DEBUG_USART3);  
		// 将 USART Tx 的 GPIO 配置为推挽复用模式
		GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(DEBUG_USART_TX_PORT_3, &GPIO_InitStructure); 
		// 将 USART Rx 的 GPIO 配置为浮空输入模式
		GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(DEBUG_USART_RX_PORT_3, &GPIO_InitStructure);
		// 配置串口的工作参数
		// 配置波特率
		USART_InitStructure.USART_BaudRate = bound;
		// 配置 针数据字长
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		// 配置停止位
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		// 配置校验位
		USART_InitStructure.USART_Parity = USART_Parity_No ;
		// 配置硬件流控制
		USART_InitStructure.USART_HardwareFlowControl =	USART_HardwareFlowControl_None;
		// 配置工作模式，收发一起
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		// 完成串口的初始化配置
		USART_Init(DEBUG_USART3, &USART_InitStructure);
		//------串口中断优先级配置------//		
		// 配置 USART 为中断源
		NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ_3;
		// 抢断优先级
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		// 子优先级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		// 使能中断
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		// 初始化配置 NVIC
		NVIC_Init(&NVIC_InitStructure);
//		// 使能串口接收中断
//		USART_ITConfig(DEBUG_USART3, USART_IT_RXNE, ENABLE);
		// 使能串口
		USART_Cmd(DEBUG_USART3, ENABLE);
		//初始化接收标志位
		U3Set.U3State = 0;	
}

/**************************************************************************
函数功能：串口4初始化配置
入口参数: 无 
返回  值：无
**************************************************************************/
void UART_Config_4(u32 bound){ 
		GPIO_InitTypeDef 	GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef 	NVIC_InitStructure;
		// 打开串口 GPIO 的时钟
		RCC_APB2PeriphClockCmd(DEBUG_UART_GPIO_CLK_4, ENABLE);
		// 打开串口外设的时钟
		RCC_APB1PeriphClockCmd(DEBUG_UART_CLK_4, ENABLE); 
		// 将 USART Tx 的 GPIO 配置为推挽复用模式
		GPIO_InitStructure.GPIO_Pin = DEBUG_UART_TX_GPIO_PIN_4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(DEBUG_UART_TX_PORT_4, &GPIO_InitStructure); 
		// 将 USART Rx 的 GPIO 配置为浮空输入模式
		GPIO_InitStructure.GPIO_Pin = DEBUG_UART_RX_GPIO_PIN_4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(DEBUG_UART_RX_PORT_4, &GPIO_InitStructure);
		// 配置串口的工作参数
		// 配置波特率
		USART_InitStructure.USART_BaudRate = bound;
		// 配置 针数据字长
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		// 配置停止位
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		// 配置校验位
		USART_InitStructure.USART_Parity = USART_Parity_No;
		// 配置硬件流控制
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		// 配置工作模式，收发一起
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		// 完成串口的初始化配置
		USART_Init(DEBUG_UART4, &USART_InitStructure);
		//------串口中断优先级配置------//		
		// 配置 USART 为中断源
		NVIC_InitStructure.NVIC_IRQChannel = DEBUG_UART_IRQ_4;
		// 抢断优先级
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		// 子优先级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
		// 使能中断
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		// 初始化配置 NVIC
		NVIC_Init(&NVIC_InitStructure);
		// 不使能串口接收中断
		USART_ITConfig(DEBUG_UART4, USART_IT_RXNE, DISABLE);
		// 使能串口接收中断
		USART_ITConfig(DEBUG_UART4, USART_IT_RXNE, ENABLE);
		// 使能串口
		USART_Cmd(DEBUG_UART4, ENABLE); 
		//初始化接收标志位
		U4Set.U4State = 0;	
}
/**************************************************************************
函数功能：串口5初始化配置
入口参数: 无 
返回  值：无
**************************************************************************/
void UART_Config_5(u32 bound){ 
		GPIO_InitTypeDef 	GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef 	NVIC_InitStructure;
		// 打开串口 GPIO 的时钟
		RCC_APB2PeriphClockCmd(DEBUG_UART_GPIO_CLK_TX_5, ENABLE); 
		RCC_APB2PeriphClockCmd(DEBUG_UART_GPIO_CLK_RX_5, ENABLE); 
		// 打开串口外设的时钟
		RCC_APB1PeriphClockCmd(DEBUG_UART_CLK_5, ENABLE); 
		// 将 USART Tx 的 GPIO 配置为推挽复用模式
		GPIO_InitStructure.GPIO_Pin = DEBUG_UART_TX_GPIO_PIN_5;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(DEBUG_UART_TX_PORT_5, &GPIO_InitStructure); 
		// 将 USART Rx 的 GPIO 配置为浮空输入模式
		GPIO_InitStructure.GPIO_Pin = DEBUG_UART_RX_GPIO_PIN_5;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(DEBUG_UART_RX_PORT_5, &GPIO_InitStructure);
		// 配置串口的工作参数
		// 配置波特率
		USART_InitStructure.USART_BaudRate = bound;
		// 配置 针数据字长
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		// 配置停止位
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		// 配置校验位
		USART_InitStructure.USART_Parity = USART_Parity_No;
		// 配置硬件流控制
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		// 配置工作模式，收发一起
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		// 完成串口的初始化配置
		USART_Init(DEBUG_UART5, &USART_InitStructure); 
		//------串口中断优先级配置------//		
		// 配置 USART 为中断源
		NVIC_InitStructure.NVIC_IRQChannel = DEBUG_UART_IRQ_5;
		// 抢断优先级
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		// 子优先级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
		// 使能中断
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		// 初始化配置 NVIC
		NVIC_Init(&NVIC_InitStructure);
		// 使能串口接收中断
		USART_ITConfig(DEBUG_UART5, USART_IT_RXNE, ENABLE);
		// 使能串口
		USART_Cmd(DEBUG_UART5, ENABLE); 
		//初始化接收标志位
		U5Set.U5State = 0; 			 
}

/**************************************************************************
函数功能：串口1发送定长字符串
入口参数: 字符串数组、字符串长度
返回  值：无
注意事项：舵机控制板专用
**************************************************************************/
void Usart1SendBuf(uint8_t *buf, uint8_t len)
{
	while(len--){
		while((USART1->SR & 0x40) == 0);
		USART_SendData(USART1,*buf++);
	}
}
/**************************************************************************
函数功能：选择一个串口发送一个字符
入口参数: 串口名、字符内容 
返回  值：无
注意事项：
1.Usart_SendByte(UART4,0x0d);	//"/r"换行
2.Usart_SendByte(UART4,0x0a);	//"/n"回车
**************************************************************************/
void Usart_SendByte(USART_TypeDef * pUSARTx,char ch){  
		// 发送一个字节数据到 USART 
		USART_SendData(pUSARTx,ch);
		// 等待发送数据寄存器为空
		while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}
/**************************************************************************
函数功能：选择一个串口发送一个字符串
入口参数: 串口名、字符串内容 
返回  值：无
注意事项：
1.可以直接指定发送的字符串内容		Usart_SendString(USART2,"\r\nMessage：");
2.可以发送指定的字符串数组变量		Usart_SendString(USART2,Usart2_RecBuf);
3.可以利用sprintf函数拼接数组			sprintf(Usart2_RecBuf,"Noise:%4.1fdB\r\n",(float)ADCvalue/10);
																		Usart_SendString(USART2,Usart2_RecBuf);
**************************************************************************/
void Usart_SendString(USART_TypeDef * pUSARTx,char *str){   
		unsigned int count = 0;
		do{
			Usart_SendByte(pUSARTx, *(str + count));
			count++;
		}while (*(str + count)!='\0');
		// 等待发送完成
		while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC) == RESET);
}

/**************************************************************************
函数功能：使用microLib重定向printf
入口参数: 无
返回  值：无
注意事项：
1.当定义了其他串口时可使用串口指针定义printf输出的串口，可以设置为打印输入与输出为不同的串口
2.需要勾选use MicroLIB	  
**************************************************************************/
int fputc(int ch, FILE *f){  //发送 
		USART_TypeDef * Print_USARTx = USART_Printf;	  //定义串口指针 
		USART_SendData(Print_USARTx,(unsigned char)ch);
		while(USART_GetFlagStatus(Print_USARTx,USART_FLAG_TC) == RESET){}	
		return ch;
}
int GetKey(void){           //接收
	  USART_TypeDef * Print_USARTx = USART_Printf;   //定义串口指针
    while(!(Print_USARTx->SR & USART_FLAG_RXNE));
    return((int)(Print_USARTx->DR & 0x1FF));
}

/**************************************************************************
函数功能：串口1中断配置（机械臂通信）
入口参数: 无
返回  值：无
**************************************************************************/
u8 UART_RX_BUF[16];												//接收缓存数组
void DEBUG_USART_IRQHandler_1(void){ 
//	uint8_t Res;
//	static bool isGotFrameHeader = false;
//	static uint8_t frameHeaderCount = 0;
//	static uint8_t dataLength = 2;
//	static uint8_t dataCount = 0;
//	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) { //判断接收中断
//		Res = USART_ReceiveData(USART1);			//(USART1->DR)取出接收寄存器数据
//		if (!isGotFrameHeader) {  						//判断帧头
//			if (Res == FRAME_HEADER) {
//				frameHeaderCount++;
//				if (frameHeaderCount == 2) {
//					frameHeaderCount = 0;
//					isGotFrameHeader = true;
//					dataCount = 1;
//				}
//			} else {
//				isGotFrameHeader = false;
//				dataCount = 0;
//				frameHeaderCount = 0;
//			}
//		}
//		if (isGotFrameHeader) { 					//接收接收数据部分
//			UART_RX_BUF[dataCount] = Res;
//			if (dataCount == 2) {
//				dataLength = UART_RX_BUF[dataCount];
//				if (dataLength < 2 || dataLength > 8){
//					dataLength = 2;
//					isGotFrameHeader = false;
//				}
//			}
//			dataCount++;
//			if (dataCount == dataLength + 2){
//				if (isUartRxCompleted == false){
//					isUartRxCompleted = true;  
//					memcpy(LobotRxBuf, UART_RX_BUF, dataCount);
//				}
//				isGotFrameHeader = false;
//			}
//		}
//	}
}

/**************************************************************************
函数功能：串口2中断配置（下位机通信）
入口参数: 无
返回  值：无
注意事项：
1.波特率：38400 
2.帧头：0x5E     
3.帧尾：0x5F
调试方案：
//		//收到下位机的数据
//		LCD_ClearRect(135,85,240,32);
//		LCD_SetAsciiFont(&ASCII_Font32);   	
//		LCD_DisplayString(5,85,"ReceCar:");
//		LCD_DisplayChar(140,85,ReceCarSta);
**************************************************************************/
void DEBUG_USART_IRQHandler_2(void)             
{
	u8 Receive;																							//存储从串口接收到的单个字节数据
	static u8 count;																				//计数值
	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET){  	//接收中断
		Receive =	USART_ReceiveData(USART2);									//读取接收到的数据
		U2Set.Usart2_RecBuf[count] = Receive;					
		if(Receive==0x5E || count>0)	count++;								//检查帧头
		else													count = 0;					
		if(count == 3){																				//验证数据包长度
			count = 0;																					//为数据重写入数组做准备
			if(U2Set.Usart2_RecBuf[2] == 0x5F){									//检查帧尾
				SysValue.ReceCarSta = U2Set.Usart2_RecBuf[1];			//获取下位机的状态
//				if(Task.DebugFlag){ 			
//					LCD_SetAsciiFont(&ASCII_Font32);   	
//					LCD_DisplayString(5,85,"ReceCar:");
//					LCD_ClearRect(135,85,240,32);
//					LCD_DisplayChar(140,85,ReceCarSta);
//				}
				memset(U2Set.Usart2_RecBuf,0,U2_RecLen*sizeof(char));		//重置接收数组
			}
		}
	}
} 

/**************************************************************************
函数功能：串口3中断配置（扫码模块通信）
入口参数: 无
返回  值：无
调试方案：
//		//收到扫码模块的数据
//		LCD_ClearRect(150,50,240,32);
//		LCD_SetAsciiFont(&ASCII_Font32);   	
//		LCD_DisplayString(5,50,"ReceK210:");
//		LCD_DisplayString(155,50,ReceK210Data);
**************************************************************************/
void DEBUG_USART_IRQHandler_3(void){  
	u8 Receive;																							//存储从串口接收到的单个字节数据
	if(USART_GetITStatus(USART3,USART_IT_RXNE) != RESET){ 	//接收中断(接收到的数据必须是0x0d结尾)
		Receive =	USART_ReceiveData(USART3);									//读取接收到的数据
		if(SysValue.QR_Mode == ScanModule){	
			if((U3Set.U3State & 0x8000) == 0){									//接收未完成
				if(U3Set.U3State & 0x4000)		
					U3Set.U3State |= 0x8000;												//接收到了0x0d就认为是接收完成了 
				else{ 																						//还没收到0x0d
					if(Receive == 0x0d)	U3Set.U3State|=0x4000;			//若收到回车符（0x0d），则将接收状态位置为回车符
					else{
						U3Set.Usart3_RecBuf[U3Set.U3State & 0X3FFF] = Receive;
						U3Set.U3State++;
						if(U3Set.U3State > (U3_RecLen - 1))						//如果超过了数组的最大长度，则将接收计数器清零
							U3Set.U3State = 0;													//接收数据错误,重新开始接收
					}		 
				}
			}
		}
		else if(SysValue.QR_Mode == ESP8266||SysValue.QR_Mode == DT_06){
			if((U3Set.U3State&(1<<15)) == 0)										//接收完的一批数据,还没有被处理,则不再接收其他数据
			{ 	
				if(U3Set.U3State < U3_RecLen)											//还可以接收数据
				{
					TIM_SetCounter(TIM7,0);													//计数器清空 
					if(U3Set.U3State == 0)	TIM_Cmd(TIM7,ENABLE);		//使能定时器7的中断 
					U3Set.Usart3_RecBuf[U3Set.U3State++] = Receive;	//记录接收到的值	 
				}
				else U3Set.U3State |= 1<<15;											//强制标记接收完成
			}
		}	
  } 
}

/**************************************************************************
函数功能：串口4中断配置（蓝牙通信）
入口参数: 无
返回  值：无
注意事项：每次进入串口中断时，可以读取一位从外部设备发送过来的数据
**************************************************************************/
void DEBUG_UART_IRQHandler_4(void){
	u8 Receive;																						//存储从串口接收到的单个字节数据
	if(USART_GetITStatus(UART4,USART_IT_RXNE) != RESET){ 	//接收中断(接收到的数据必须是0x0d 0x0a结尾)
		Receive =	USART_ReceiveData(UART4);									//读取接收到的数据
		if((U4Set.U4State & 0x8000) == 0){									//接收未完成
			if(U4Set.U4State & 0x4000){												//接收到了0x0d
				if(Receive != 0x0a)	U4Set.U4State = 0;					//接收错误,重新开始
				else 								U4Set.U4State |= 0x8000;		//接收完成了 
			}	
			else{ 																						//还没收到0x0d
				if(Receive == 0x0d)	U4Set.U4State|=0x4000;			//若收到回车符（0x0d），则将接收状态位置为回车符
				else{	
					U4Set.Uart4_RecBuf[U4Set.U4State & 0X3FFF] =	Receive;
					U4Set.U4State++;	
					if(U4Set.U4State > (U4_RecLen - 1))						//如果超过了数组的最大长度，则将接收计数器清零
						U4Set.U4State = 0;													//接收数据错误, 重新开始接收	  
				}		 
			}
		}
	}
}

/**************************************************************************
函数功能：串口5中断配置
入口参数: 无
返回  值：无
注意事项：每次进入串口中断时，可以读取一位从外部设备发送过来的数据
**************************************************************************/
char Uart5_RecBuf[U5_RecLen]; 									//接收缓冲,最大U5_RecLen个字节
char Uart5_SenBuf[U5_SenLen]; 									//发送缓冲,最大U5_SenLen个字节
void DEBUG_UART_IRQHandler_5(void){ 
	static u16 U5State = 0; 											//接收状态标记
	u8 Receive;																		//存储从串口接收到的单个字节数据
	if(USART_GetITStatus(UART5,USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Receive =	USART_ReceiveData(UART5);					//读取接收到的数据
		if((U5State & 0x8000) == 0){								//接收未完成
			if(U5State & 0x4000){											//接收到了0x0d
				if(Receive != 0x0a)	U5State = 0;				//接收错误,重新开始
				else 								U5State |= 0x8000;	//接收完成了 
			}
			else{ 																		//还没收到0x0d
				if(Receive == 0x0d)	U5State|=0x4000;		//若收到回车符（0x0d），则将接收状态位置为回车符
				else{
					Uart5_RecBuf[U5State & 0X3FFF] = Receive;
					U5State++;
					if(U5State > (U5_RecLen - 1))					//如果超过了数组的最大长度，则将接收计数器清零
						U5State = 0;												//接收数据错误,重新开始接收	  
				}		 
			}
		}
		if(U5State & 0x8000){	 	
//			memcpy(ReceBlueData,Uart5_RecBuf,U5_RecLen*sizeof(char));		//获取接收到的消息
//			if(Task.DebugFlag){
//				LCD_SetAsciiFont(&ASCII_Font32);   	
//				LCD_ClearRect(87,225,240,32);
//				//LCD_DisplayString(5,225,"Blue:");			
//				LCD_DisplayString(90,225,ReceBlueData);
//			}
//			memset(Uart5_RecBuf,0,U5_RecLen*sizeof(char));							//重置接收数组
			U5State = 0;
		}		
  }
}
