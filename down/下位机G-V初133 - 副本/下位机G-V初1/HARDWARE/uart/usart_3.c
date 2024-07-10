#include "blue.h"	  
#include "usart_3.h"	  
extern char sign_c_to_rgb;

extern Task_set Task_Process; //任务集
extern	char c_rgb;         // 识别 命令头
extern int compensate_axis[2]; // 补偿数组
extern signed char Receive_host; // 上位机命令
extern	u8 first_byte; // 区域校正标志    0 r1 g1 b1  0 r2 g2 b2  8位, 仅第一次校正
/************ 与上位机通信 *****************************************************/
void USART1_IRQHandler(void) {              
	static char ucRxCnt;	
  static char ucRxBuffer_1[20];
	if(USART_GetITStatus( USART1, USART_IT_RXNE) != RESET ) { //判断是否为接收中断
		ucRxBuffer_1[ucRxCnt++]=USART_ReceiveData(USART1);;	   
		if ( ucRxBuffer_1[0]!=0x5E ) { ucRxCnt=0; return; } // 校头
		if ( ucRxBuffer_1[ucRxCnt-1]!=0x5F  )  return;      // 未收到帧尾
		else { // 开始解码
		  if( ucRxBuffer_1[1]==0x50 )        Receive_host = -1; // 可以开始校正
			else if( ucRxBuffer_1[1]==0x4F )   Receive_host = -2; // 扫码成功
			else { 
				Receive_host = ucRxBuffer_1[1]; 
				get_host_cmd(); // 获取上位机命令
			}
				ucRxCnt = 0;
		}
	}
	USART_ClearITPendingBit( USART1, USART_IT_RXNE ); // 并清除中断标志
} 
/************ 与视觉通信 *****************************************************/
void USART3_IRQHandler(void) {
	static  char ucRxBuffer[20];
	static  char ucRxCnt;	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {		//判断是否为接收中断
		ucRxBuffer[ucRxCnt++] = USART_ReceiveData( USART3 ) ;	
		if ( ucRxBuffer[0]!='(' ) { ucRxCnt=0;  return; } // 校头
	  if ( ucRxBuffer[ucRxCnt-1]!=')' )       return;   // 未收到尾
	  else{ // 开始解码	
//			if( sign_c_to_rgb >=2  ) { //码垛时，识别颜色坐标			
//				if( ucRxBuffer[1] == Staging_calibration ) sscanf(ucRxBuffer, "(%c,%d,%d)", &c_rgb, &compensate_axis[0], &compensate_axis[1]);
//			}	
//			else{
				sscanf(ucRxBuffer, "(%c,%d,%d)", &c_rgb, &compensate_axis[0], &compensate_axis[1]); 
//		  }
			ucRxCnt = 0;		
	  }
	}
	USART_ClearITPendingBit(USART3, USART_IT_RXNE);
}
/*  获取上位机命令  */
void get_host_cmd(void){
		  if    ( Receive_host==0x41 )   { Task_Process = to_code ; }  
		 else if( Receive_host==0x42 )   { Task_Process = to_Raw ;  }
		 else if( Receive_host==0x43 )	 { Task_Process = to_Rough_r;   first_byte |= 0x40; }  // 40
		 else if( Receive_host==0x44 )   { Task_Process = to_Rough_g;   first_byte |= 0x20; }
		 else if( Receive_host==0x45 )   { Task_Process = to_Rough_b;   first_byte |= 0x10; }
		 else if( Receive_host==0x4c )	 { Task_Process = to_Staging_r; first_byte |= 0xf4; }//f4
		 else if( Receive_host==0x4d )   { Task_Process = to_Staging_g; first_byte |= 0xf2; }
		 else if( Receive_host==0x4e )   { Task_Process = to_Staging_b; first_byte |= 0xf1; }
		 else if( Receive_host==0x4a )   { Task_Process = Stag_to_Rough;                 }
		 else if( Receive_host==0x4b )   { Task_Process = Rough_to_Raw; first_byte = 0;  }
		 else if( Receive_host==0x46 )   { Task_Process = to_origin ;   first_byte = 0;  }
         else if( Receive_host==0x56){Task_Process = to_sec ;   first_byte = 0;}
		 Receive_host=0;  //校正完清
}

