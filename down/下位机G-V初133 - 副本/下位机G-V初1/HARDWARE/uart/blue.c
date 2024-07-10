#include "blue.h"	  
#include "delay.h"

extern Task_set Task_Process;
int distance_blue, dir_blue;
extern char my_TX[50]; // �ҵķ���

void UART4_IRQHandler(void){
	static  char ucRxBuffer[30];
	static  char ucRxCnt = 0;		
	 if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET){
		 	ucRxBuffer[ucRxCnt++]= USART_ReceiveData(UART4);	 
			if ( ucRxBuffer[0]!='z' ) 	{	ucRxCnt=0;	return;}
			if ( ucRxBuffer[ucRxCnt-1]!='p' )     return;
			else{  // ����У��ɹ�
					 if( ucRxBuffer[1]=='x' )	{  // to X_axis
						sscanf(ucRxBuffer, "zx,%dp",  &distance_blue);
						Task_Process = x_move;
					}
					 else if( ucRxBuffer[1]=='e' ) // С���ƶ�ָ��
		      {
			        sscanf(ucRxBuffer, "ze%d,%dp", &dir_blue, &distance_blue);
			        Task_Process = move_blue;
		      }
					else if( ucRxBuffer[1]=='y' )	{	 // to Y_axis
						sscanf(ucRxBuffer, "zy,%dp",  &distance_blue);
						Task_Process = y_move;
					}
					else if( ucRxBuffer[1]=='g' )  Task_Process = gui_0;
					else if( ucRxBuffer[1]=='r' )  Task_Process = pos_cal_Rough;
					else if( ucRxBuffer[1]=='s' )  Task_Process = pos_cal_Stag;
					else if( ucRxBuffer[1]=='1' )  Task_Process = to_code;
					else if( ucRxBuffer[1]=='2' )  Task_Process = to_Raw;
					else if( ucRxBuffer[1]=='3' )  Task_Process = to_Rough_r;
					else if( ucRxBuffer[1]=='4' )	 Task_Process = to_Rough_g;
					else if( ucRxBuffer[1]=='5' )  Task_Process = to_Rough_b;
					else if( ucRxBuffer[1]=='6' )  Task_Process = to_Staging_r;
					else if( ucRxBuffer[1]=='7' )  Task_Process = to_Staging_g;
					else if( ucRxBuffer[1]=='8' )	 Task_Process = to_Staging_b;
					else if( ucRxBuffer[1]=='9' )  Task_Process = Stag_to_Rough;
					else if( ucRxBuffer[1]=='>' )	 Task_Process = Rough_to_Raw;
					else if( ucRxBuffer[1]=='<' )  Task_Process = to_origin; 
					else if( ucRxBuffer[1]=='a' )  Task_Process = yaw_0;
					else if( ucRxBuffer[1]=='b' )  Task_Process = yaw_90;
					else if( ucRxBuffer[1]=='c' )  Task_Process = yaw_180;
					else if( ucRxBuffer[1]=='d' )  Task_Process = yaw_270;
					ucRxCnt=0;
				}
		}
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
}
/* �����ַ��� */
void USART_SendString(USART_TypeDef* USARTx, char *DataString) {
	int i = 0;
			USART_ClearFlag(USARTx,USART_FLAG_TXE);										//�����ַ�ǰ��ձ�־λ������ȱʧ�ַ����ĵ�һ���ַ���
		while(DataString[i] != '\0') {											//�ַ���������
			while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == 0);					//���ݷ��ͼĴ�����
        USART_SendData(USARTx, DataString[i]);                       //ÿ�η����ַ�����һ���ַ�
			while(USART_GetFlagStatus(USARTx,USART_FLAG_TC) == 0);					//���ݷ������
		     USART_ClearFlag(USARTx,USART_FLAG_TXE | USART_FLAG_TC );									//�����ַ�����ձ�־λ
			i++;
		}
	strcpy( my_TX, DataString );
}


