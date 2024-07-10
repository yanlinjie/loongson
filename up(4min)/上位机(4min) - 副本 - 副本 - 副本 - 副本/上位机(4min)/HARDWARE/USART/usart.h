#ifndef __USART_H
#define __USART_H

// USART 通用宏定义
//-----字节数-----// 
#define U2_RecLen  		30  	
#define U2_SenLen  		30
#define U3_RecLen  		1024  	
#define U3_SenLen  		1024 
#define U4_RecLen  		30  	
#define U4_SenLen  		30  
#define U5_RecLen  		30  	
#define U5_SenLen  		30 

//头文件上放的宏定义可以在公共头文件中使用
#include "sys.h" 

//-----波特率-----//					
#define Bound_2400    	2400
#define Bound_4800    	4800
#define Bound_9600    	9600
#define Bound_19200   	19200
#define Bound_38400   	38400
#define Bound_57600   	57600
#define Bound_115200  	115200
#define Bound_128000  	128000
#define Bound_230400  	230400
#define Bound_256000  	256000
#define Bound_460800  	460800

// 串口 1-USART1
#define DEBUG_USART1                  USART1
#define DEBUG_USART_CLK_1             RCC_APB2Periph_USART1
#define DEBUG_USART_GPIO_CLK_1        RCC_APB2Periph_GPIOA
#define DEBUG_USART_TX_PORT_1      	  GPIOA
#define DEBUG_USART_RX_PORT_1      	  GPIOA
#define DEBUG_USART_TX_GPIO_PIN_1     GPIO_Pin_9
#define DEBUG_USART_RX_GPIO_PIN_1     GPIO_Pin_10
#define DEBUG_USART_IRQ_1             USART1_IRQn
#define DEBUG_USART_IRQHandler_1      USART1_IRQHandler
// 串口 2-USART2
#define DEBUG_USART2                  USART2
#define DEBUG_USART_CLK_2             RCC_APB1Periph_USART2
#define DEBUG_USART_GPIO_CLK_2        RCC_APB2Periph_GPIOA
#define DEBUG_USART_TX_PORT_2      	  GPIOA
#define DEBUG_USART_RX_PORT_2      	  GPIOA
#define DEBUG_USART_TX_GPIO_PIN_2     GPIO_Pin_2
#define DEBUG_USART_RX_GPIO_PIN_2     GPIO_Pin_3
#define DEBUG_USART_IRQ_2             USART2_IRQn
#define DEBUG_USART_IRQHandler_2      USART2_IRQHandler
// 串口 3-USART3
#define DEBUG_USART3                  USART3
#define DEBUG_USART_CLK_3             RCC_APB1Periph_USART3
#define DEBUG_USART_GPIO_CLK_3        RCC_APB2Periph_GPIOB
#define DEBUG_USART_TX_PORT_3      	  GPIOB
#define DEBUG_USART_RX_PORT_3      	  GPIOB
#define DEBUG_USART_TX_GPIO_PIN_3     GPIO_Pin_10
#define DEBUG_USART_RX_GPIO_PIN_3     GPIO_Pin_11
#define DEBUG_USART_IRQ_3             USART3_IRQn
#define DEBUG_USART_IRQHandler_3      USART3_IRQHandler
// 串口 4-UART4 
#define DEBUG_UART4                   UART4
#define DEBUG_UART_CLK_4              RCC_APB1Periph_UART4
#define DEBUG_UART_GPIO_CLK_4         RCC_APB2Periph_GPIOC
#define DEBUG_UART_TX_PORT_4      	  GPIOC
#define DEBUG_UART_RX_PORT_4      	  GPIOC
#define DEBUG_UART_TX_GPIO_PIN_4      GPIO_Pin_10
#define DEBUG_UART_RX_GPIO_PIN_4      GPIO_Pin_11
#define DEBUG_UART_IRQ_4              UART4_IRQn
#define DEBUG_UART_IRQHandler_4       UART4_IRQHandler
// 串口 5-UART5
#define DEBUG_UART5                   UART5
#define DEBUG_UART_CLK_5              RCC_APB1Periph_UART5
#define DEBUG_UART_GPIO_CLK_TX_5      RCC_APB2Periph_GPIOC
#define DEBUG_UART_GPIO_CLK_RX_5      RCC_APB2Periph_GPIOD
#define DEBUG_UART_TX_PORT_5      	  GPIOC
#define DEBUG_UART_RX_PORT_5     	  GPIOD
#define DEBUG_UART_TX_GPIO_PIN_5      GPIO_Pin_12
#define DEBUG_UART_RX_GPIO_PIN_5      GPIO_Pin_2
#define DEBUG_UART_IRQ_5              UART5_IRQn
#define DEBUG_UART_IRQHandler_5       UART5_IRQHandler

#define USART_Printf  	  USART3   	  //选择要打印输出的串口
extern USART_TypeDef *Print_USARTx;   //定义串口指针

typedef struct{
	char Usart2_RecBuf[U2_RecLen]; 			//接收缓冲,最大U2_RecLen个字节
	char Usart2_SenBuf[U2_SenLen]; 			//发送缓冲,最大U2_SenLen个字节
	vu16 U2State;												//接收数据状态
}Usart2_Control;
typedef struct{
	char Usart3_RecBuf[U3_RecLen]; 			//接收缓冲,最大U3_RecLen个字节
	char Usart3_SenBuf[U3_SenLen]; 			//发送缓冲,最大U3_SenLen个字节
	vu16 U3State;												//接收数据状态
}Usart3_Control;
typedef struct{
	char Uart4_RecBuf[U4_RecLen]; 			//接收缓冲,最大U4_RecLen个字节
	char Uart4_SenBuf[U4_SenLen]; 			//发送缓冲,最大U4_SenLen个字节
	vu16 U4State;												//接收数据状态
}Uart4_Control;
typedef struct{
	char Uart5_RecBuf[U5_RecLen]; 			//接收缓冲,最大U5_RecLen个字节
	char Uart5_SenBuf[U5_SenLen]; 			//发送缓冲,最大U5_SenLen个字节
	vu16 U5State;												//接收数据状态
}Uart5_Control;

extern Usart2_Control U2Set;
extern Usart3_Control U3Set;
extern Uart4_Control  U4Set;
extern Uart5_Control  U5Set;

void Usart1SendBuf(uint8_t *buf, uint8_t len);							// 发送定长字符串（机械臂）
void Usart_SendByte(USART_TypeDef * pUSARTx, char ch);   		// 发送一个字节
void Usart_SendString(USART_TypeDef * pUSARTx,char *str);   // 发送一串字符

//串口1
void USART_Config_1(u32 bound);
void DEBUG_USART_IRQHandler_1(void);
//串口2
void USART_Config_2(u32 bound);
void DEBUG_USART_IRQHandler_2(void);
//串口3
void USART_Config_3(u32 bound);
void DEBUG_USART_IRQHandler_3(void);
//串口4
void UART_Config_4(u32 bound);
void DEBUG_UART_IRQHandler_4(void);
//串口5
void UART_Config_5(u32 bound);
void DEBUG_UART_IRQHandler_5(void);

#endif
