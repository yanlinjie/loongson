#include "usart_1.h"	  
#include "uart_4.h"	  

 extern u8  sign_host_receive ,sign_jiaozheng, first_byte; 
 extern Task_set Task_Process;

void USART1_IRQHandler(void) {               	//����1�жϷ������
	static char ucRxCnt;	
  static char ucRxBuffer_1[20];
	if(USART_GetITStatus( USART1, USART_IT_RXNE) != RESET ) { //�ж��Ƿ�Ϊ�����ж�
		ucRxBuffer_1[ucRxCnt++]=USART_ReceiveData(USART1);;	   
		if ( ucRxBuffer_1[0]!=0x5E ) { ucRxCnt=0; return; }
		if ( ucRxBuffer_1[ucRxCnt-1]!=0x5F  )  return;   // δ�յ�֡β
		else { // ����У��ɹ�,��ʼ����
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
		 else if( ucRxBuffer_1[1]==0x50 )    sign_jiaozheng = 1; // ���Կ�ʼУ��
			memset(ucRxBuffer_1,'\0',20*sizeof(char));	//�������
		}
	}
	USART_ClearITPendingBit( USART1, USART_IT_RXNE ); // ������жϱ�־
} 

