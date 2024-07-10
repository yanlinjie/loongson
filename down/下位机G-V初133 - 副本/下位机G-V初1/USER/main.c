#include "main.h"
/* ��ע 
������Դһ����
�������---Tim8
��ʱ����---Tim6
����λ��ͨ��-----Usart1		19200
��������ͨ��-----Usart2 	115200
��openmvͨ��-----Usart3  	19200
������ͨ��-------Uart4  	115200   
*/
int main(void) {
	   char cnt, old_color; //ԭ��������ʶͬһ��ɫ����+1 ,����ɫ
		 delay_init(72);
	   read_axis();  // ��ȡ����
	   uart_1_init(19200);    // ����λ��ͨ��
		 Initial_UART2(115200); // ������
	   usart_3_Init(19200); // sj
	   uart_4_Init(115200);   // ����
     GPIO_motor_EN_and_dir();  // �����������
		 TIME_8_PWM_Init(65535, 72);//�����������, 72M / Pcr  �ļ���Ƶ��,������pause_WideΪpause_Wide  
	    TIMER6_Init(5000,7200);   //   500 ms  ��ʱˢ��		
    	SPI_LCD_Init();  //��ʼ��lcd 
	    LCD_Clear();
	  LCD_DisplayString(10, 10+32,   "Task:");
	  LCD_DisplayString(10, 10+32*2, "axis_X:");
	  LCD_DisplayString(10, 10+32*3, "axis_Y:");
	  LCD_DisplayString(10, 10+32*4, "CRGB:");
	  LCD_DisplayString(10, 10+32*5, "TX:");
#if TLY == 1
			USART_Cmd(USART2, ENABLE);
	    LCD_DisplayString(10, 10+32*6, "ya:");
#else	
	    LCD_DisplayString(10, 10+32*6, "Rx:");
#endif
  while(1){
#if TLY == 1
		yaw = (float)stcAngle.Angle[2]/32768*180 ;
				if ( yaw < -180.0 )    yaw = yaw + 180;
		else if( yaw > 180.0 )     yaw = yaw - 180;
		LCD_DisplayDecimals(10+16*3, 10+32*6, yaw,8,3);
#endif
	  switch( Task_Process ) {
			case to_Raw:{ // �� ԭ����
        to_axis( axis.x_code, axis.y_code, "y_first");	 // ��ά�봦x����		
				while(Receive_host != -2 ); Receive_host=0; //ɨ��ɹ� 
				to_axis( axis.x_Raw, axis.y_Raw, "x_first"); 
				USART_SendString(USART1, car_Arrive);				    
					Task_Process = Raw_color;
			}  
			case Raw_color:{ // ԭ����ʶ����ɫ
				USART_Cmd(USART3, ENABLE); 
				while( Task_Process == Raw_color ) {
					 if(  c_rgb=='r' || c_rgb=='g' || c_rgb=='b' ) {
								if( c_rgb == old_color ) {
									cnt++;
									if( cnt >= 2 ) { //���� n + 1 �� ������ͬһ��ɫ
										sprintf(combined_string, "^%c_", c_rgb);    // ��ʽ��ƴ���ַ���
										USART_SendString( USART1, combined_string); //����ʶ���������ɫ
										if( debug_key )   LCD_DisplayChar(10+16*5, 10+32*4, c_rgb);
										memset(combined_string,NULL,3*sizeof(char)); // ���ý�������
										cnt = 0; old_color=0;
									}
								}
								else  cnt = 0;     // �м���ִ�����ɫ�������ü���
								old_color = c_rgb; // ���¾���ɫ
								c_rgb = NULL;      // ����ɫ��־
					  }
			   }
				 USART_Cmd(USART3, DISABLE);
			}break;
			case to_Rough_r:{ // ԭ���� ��  �ּӹ��� r	
				if( first_byte==0x40 ) { //���ε�����
					to_axis( axis.x_C-100, axis.y_Raw+50, "y_first"); 
					spin(target_yaw=90,1);
					to_axis( axis.x_C,  axis.y_C_r, "y_first"); 
					//pos_compensate(first_Area_cali_cnt);
					first_byte = 0xf0; first_r = 0; 
				}			
				else { // �ǳ��ε�����
					if( first_r == 1 ) { // ���ε�ɫ�� ��ҪУ��
					 to_axis( axis.x_C, axis.y_C_r, "y_first"); 
                        
                        //�Ȳ�У׼
					 Receive_host=-1; pos_compensate( cali_cnt );
           first_r = 0; 						
					} 
          else	to_axis( axis.x_C, axis.y_C_r, "y_first"); 	// ����ɫ���ƶ�			
				}  
								Task_Process = no_task;
				USART_SendString(USART1, car_Arrive);
			}break;
			case to_Rough_g:{ // ԭ���� ��  �ּӹ��� g  
				if( first_byte==0x20 ) {
					to_axis( axis.x_C-100, axis.y_Raw+50, "y_first" ); 
					spin(target_yaw=90,1);
					to_axis( axis.x_C,     axis.y_C_g, "y_first"); 
					pos_compensate(first_Area_cali_cnt);
					first_byte = 0xf0; first_g = 0;
				}
				else { 
					if( first_g == 1 ) { //��ҪУ��
						to_axis( axis.x_C, axis.y_C_g, "y_first");
						Receive_host=-1; pos_compensate( cali_cnt );
						first_g = 0; 
					}
					else to_axis( axis.x_C, axis.y_C_g, "y_first");
				}					
		  					Task_Process = no_task;
				USART_SendString(USART1, car_Arrive);
			}break;
			case to_Rough_b:{ // ԭ���� ��  �ּӹ��� b
				if( first_byte==0x10 ) {
					to_axis( axis.x_C-100, axis.y_Raw+50, "y_first" ); 
					spin(target_yaw=90,1);
					to_axis( axis.x_C,     axis.y_C_b, "y_first"); 
					pos_compensate(first_Area_cali_cnt);
					first_byte = 0xf0; first_b = 0; 
				}
				else 	{
					if( first_b == 1 ) { //��ҪУ��
						to_axis( axis.x_C, axis.y_C_b, "y_first");
						Receive_host=-1; pos_compensate( cali_cnt );
						first_b = 0; 
					}
					else to_axis( axis.x_C, axis.y_C_b, "y_first");  					
				}
								Task_Process = no_task;
				USART_SendString(USART1, car_Arrive);
			}break;			 
            
            
			case to_Staging_r:{ // �ּӹ� ��  �ݴ��� r	 
				if( first_byte==0xf4 ){
					to_axis( axis.x_C-100, axis.y_D-90, "x_first");
					spin(target_yaw=180,1);
					to_axis( axis.x_D_r, axis.y_D, "x_first");
				//	pos_compensate(first_Area_cali_cnt);
					first_byte = 0x0f; first_r = 1; 
				}
				else {
					if( first_r == 0 ) { //��ҪУ��
						to_axis( axis.x_D_r,   motor.y_axis, "x_first");
						//Receive_host=-1; pos_compensate( cali_cnt );
						first_r = 1; 
					}					
				 else to_axis( axis.x_D_r,   motor.y_axis, "x_first"); 
				}				
								Task_Process = no_task;
				USART_SendString(USART1, car_Arrive);
			}break;
            
            
            
			case to_Staging_g:{ // �ּӹ� ��  �ݴ��� g
				if( first_byte==0xf2 ){
					to_axis( axis.x_C-100, axis.y_D-90, "x_first");
					spin(target_yaw=180,1);
					to_axis( axis.x_D_g, axis.y_D, "x_first" );
					pos_compensate(first_Area_cali_cnt);
					first_byte = 0x0f; first_g = 1; 
				}			
				else  {
					if( first_g == 0 ) { //��ҪУ��
						to_axis( axis.x_D_g,   motor.y_axis, "x_first");
						Receive_host=-1; pos_compensate( cali_cnt );
						first_g = 1; 
					}					
				 else to_axis( axis.x_D_g,   motor.y_axis, "x_first"); 
				}
								Task_Process = no_task;
				USART_SendString(USART1, car_Arrive);
			}break;
			case to_Staging_b:{ // �ּӹ� ��  �ݴ��� b
				if( first_byte==0xf1 ){
					to_axis( axis.x_C-100, axis.y_D-90, "x_first");
					spin(target_yaw=180,1);
					to_axis( axis.x_D_b, axis.y_D, "x_first");
					pos_compensate(first_Area_cali_cnt); 
					first_byte = 0x0f; first_b = 1;
				}		
				else  {
					if( first_b == 0 ) { //��ҪУ��
						to_axis( axis.x_D_b,   motor.y_axis, "x_first");
						Receive_host=-1; pos_compensate( cali_cnt );
						first_b = 1; 
					}					
				 else to_axis( axis.x_D_b,   motor.y_axis, "x_first");  
				}
				Task_Process = no_task;
				USART_SendString(USART1, car_Arrive);
			}break;			
			case Stag_to_Rough:{ // �ݴ��� ��  �ּӹ�
				to_axis( axis.x_C-100, axis.y_D-90, "y_first"); // Զ���ݴ���
				spin(target_yaw=90,1);
        #if  back_calib == 'r' 				
				to_axis( axis.x_C,     axis.y_C_r, "y_first");  // ȥ�ּӹ�r��У��
				#elif  back_calib == 'g'
				to_axis( axis.x_C,     axis.y_C_g, "y_first");	// ȥ�ּӹ�g��У��		 	
				#elif  back_calib == 'b' 
				to_axis( axis.x_C,     axis.y_C_b, "y_first"); // ȥ�ּӹ�b��У��
				#endif
				pos_compensate( back_cali_cnt );
				Task_Process = no_task;
				USART_SendString(USART1, car_Arrive);
			}break;	
			case Rough_to_Raw:{ // �ּӹ� ��  ԭ����
				  to_axis( axis.x_C-100, axis.y_Raw+50,"x_first" );
				  spin(target_yaw=0,1);
				if( compensate_back ) to_axis( axis.x_Raw + 70, axis.y_Raw-10,"x_first");
				else    to_axis( axis.x_Raw , axis.y_Raw,"x_first");
					motor.x_axis = axis.x_Raw;
				  motor.x_axis = axis.y_Raw;
				 Task_Process = Raw_color; // ʶ��ɫ
				 USART_SendString(USART1, car_Arrive);
			}break;
            
            
			case to_origin:{  // �����
				//to_axis( 80, axis.y_D-90, "y_first");
				//spin(target_yaw=270,1);				
				to_axis( 0, 294, "y_first"); 
				to_axis( 0, 0, "y_first");
				INTX_DISABLE();  //�������ж�
				Task_Process = no_task;
				USART_SendString(USART1, car_Arrive);
			}break;
            
            case to_sec:
            {
                to_axis( 80, axis.y_D-90, "y_first");
				spin(target_yaw=270,1);		
                to_axis( 80, 898, "y_first");                
                Task_Process = no_task;
                USART_SendString(USART1, car_Arrive);
            
            }break;
			case yaw_90:{    spin( target_yaw=90,1 );   Task_Process = no_task; }break; 
			case yaw_180:{   spin( target_yaw=180,1);   Task_Process = no_task; }break; 
			case yaw_270:{   spin( target_yaw=270,1);  Task_Process = no_task;  }break; 
			case yaw_0:{     spin( target_yaw=0,1 );   Task_Process = no_task;  }break;
			case gui_0:{     gui_0_yaw( target_yaw );    Task_Process = no_task;  }break;
			case move_blue:{ motion_control( distance_blue,  dir_blue );  Task_Process = no_task;  }break;
			case pos_cal_Rough:{
							pos_cal(3, 'R');
							LCD_DisplayNumber(10+16*7, 10+32*2, motor.x_axis, 5); 
							LCD_DisplayNumber(10+16*7, 10+32*3, motor.y_axis, 5);  // ��ʾ����				
							Task_Process = no_task; 
			}break; 
			case pos_cal_Stag:{
							pos_cal(3, 'S'); 
							LCD_DisplayNumber(10+16*7, 10+32*2, motor.x_axis, 5); 
							LCD_DisplayNumber(10+16*7, 10+32*3, motor.y_axis, 5);  // ��ʾ����				
							Task_Process = no_task; 
			}break; 
			case x_move:  {
							to_axis( motor.x_axis + distance_blue ,  motor.y_axis, "x_first");
							LCD_DisplayNumber(10+16*7, 10+32*2, motor.x_axis, 5); 
							Task_Process = no_task; 
			}break;
			case y_move:  {
							to_axis( motor.x_axis  ,   motor.y_axis + distance_blue, "y_first");
							LCD_DisplayNumber(10+16*7, 10+32*3, motor.y_axis, 5);  // ��ʾ���� 				
							Task_Process = no_task; 
			}break;
			default: break;		
		}
	}
}
 // ��ʱ�����ж�
void TIM6_IRQHandler(void) {
	static char cnt;
	if(TIM6->SR&0X0001) //����ж�
	{
		if( debug_key ){		
				cnt++;
				if( cnt==1 )	  LCD_DisplayString(10+16*3, 10+32*5, my_TX ); // ��ʾ���͵�����
		#if TLY == 0  // ��ʹ������������ȷ������
				else if(cnt==2)		LCD_DisplayChar(10+16*3, 10+32*6, Receive_host );		// ��ʾ������λ��������
		#endif
				else if(cnt==3)    LCD_DisplayChar(10+16*5,    10+32*4, c_rgb   );
				else if(cnt==4)    LCD_DisplayNumber(10+16*6,  10+32*4, compensate_axis[0] ,4);
				else if(cnt==5)    LCD_DisplayNumber(10+16*10, 10+32*4, compensate_axis[1] ,4);
				else if(cnt==6){
					LCD_DisplayString(10+16*5, 10+32, "                            "); 
					LCD_DisplayString(10+16*5, 10+32,     Task_set_strings[Task_Process]); // ��ʾ��ǰ����
					}
				if(cnt>=6) cnt=0;
		}
	} 
	TIM6->SR&=~(1<<0); //����жϱ�־λ 
}
 // ������������ж�
void TIM8_UP_IRQHandler(void){ 
	 static u32 step=0;
  if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET){ 
			if( motor.mode_step==1  ){   // ���μӼ���
				 motor.sign_1_ok_step=1; //һ�����
				 TIM_Cmd(TIM8, DISABLE); // ʧ�� TIM8
			}
			else if(motor.mode_step==0  ){	// ������	
					step++;
					if( step >= motor.step_max ) { //   200*motor.Subdivision__coefficient (step) == 1 (circle)
					step=0;
					motor.sign_1_ok_step=1; //һ�����
					TIM_Cmd(TIM8, DISABLE); //ʧ�� TIM8
				}
			}
			TIM_ClearITPendingBit(TIM8, TIM_IT_Update); // �������жϱ�־λ
	} 
}
