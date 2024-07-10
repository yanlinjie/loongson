#include "usart_1.h"	  
#include "uart_4.h"	  

 extern u8  sign_host_receive ,sign_jiaozheng, first_byte; 
 extern Task_set Task_Process;

void USART1_IRQHandler(void) {               	//串口1中断服务程序
	static char ucRxCnt;	
  static char ucRxBuffer_1[20];
	if(USART_GetITStatus( USART1, USART_IT_RXNE) != RESET ) { //判断是否为接收中断
		ucRxBuffer_1[ucRxCnt++]=USART_ReceiveData(USART1);;	   
		if ( ucRxBuffer_1[0]!=0x5E ) { ucRxCnt=0; return; }
		if ( ucRxBuffer_1[ucRxCnt-1]!=0x5F  )  return;   // 未收到帧尾
		else { // 接受校验成功,开始解码
			if    ( ucRxBuffer_1[1]==0x41 )    Task_Process = to_code ;   
		 else if( ucRxBuffer_1[1]==0x42 )    Task_Process = to_Raw ;
		 else if( ucRxBuffer_1[1]==0x43 )	 { Task_Process = to_Rough_r; first_byte |= 0x40; }
		 else if( ucRxBuffer_1[1]==0x44 )  { Task_Process = to_Rough_g; first_byte |= 0x20; }
		 else if( ucRxBuffer_1[1]==0x45 )  { Task_Process = to_Rough_b; first_byte |= 0x10; }
		 else if( ucRxBuffer_1[1]==0x4c )	 { Task_Process = to_Staging_r; first_byte |= 0xf4; }
		 else if( ucRxBuffer_1[1]==0x4d )  { Task_Process = to_Staging_g; first_byte |= 0xf2; }
		 else if( ucRxBuffer_1[1]==0x4e )  { Task_Process = to_Staging_b; first_byte |= 0xf1; }
		 else if( ucRxBuffer_1[1]==0x4a )  { Task_Process = Stag_to_Rough;   }
		 else if( ucRxBuffer_1[1]==0x4b )  { Task_Process = Rough_to_Raw; first_byte = 0;}
		 else if( ucRxBuffer_1[1]==0x46 )  { Task_Process = to_origin ;  first_byte = 0;}
		 else if( ucRxBuffer_1[1]==0x50 )    sign_jiaozheng = 1; // 可以开始校正
			memset(ucRxBuffer_1,'\0',20*sizeof(char));	//清空数组
		}
	}
	USART_ClearITPendingBit( USART1, USART_IT_RXNE ); // 并清除中断标志
} 

