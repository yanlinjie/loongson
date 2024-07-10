#include "main.h"
/* 备注 
所用资源一览：
脉冲输出---Tim8
计时控制---Tim6
与上位机通信-----Usart1		19200
与陀螺仪通信-----Usart2 	115200
与openmv通信-----Usart3  	19200
与蓝牙通信-------Uart4  	115200   
*/
int main(void) {
	   char cnt, old_color; //原料区连续识同一颜色次数+1 ,旧颜色
		 delay_init(72);
	   read_axis();  // 获取坐标
	   uart_1_init(19200);    // 与上位机通信
		 Initial_UART2(115200); // 陀螺仪
	   usart_3_Init(19200); // sj
	   uart_4_Init(115200);   // 蓝牙
     GPIO_motor_EN_and_dir();  // 电机引脚配置
		 TIME_8_PWM_Init(65535, 72);//脉冲输出配置, 72M / Pcr  的计数频率,计数到pause_Wide为pause_Wide  
	    TIMER6_Init(5000,7200);   //   500 ms  定时刷新		
    	SPI_LCD_Init();  //初始化lcd 
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
			case to_Raw:{ // 到 原料区
        to_axis( axis.x_code, axis.y_code, "y_first");	 // 二维码处x坐标		
				while(Receive_host != -2 ); Receive_host=0; //扫码成功 
				to_axis( axis.x_Raw, axis.y_Raw, "x_first"); 
				USART_SendString(USART1, car_Arrive);				    
					Task_Process = Raw_color;
			}  
			case Raw_color:{ // 原料区识别颜色
				USART_Cmd(USART3, ENABLE); 
				while( Task_Process == Raw_color ) {
					 if(  c_rgb=='r' || c_rgb=='g' || c_rgb=='b' ) {
								if( c_rgb == old_color ) {
									cnt++;
									if( cnt >= 2 ) { //连续 n + 1 次 看的是同一颜色
										sprintf(combined_string, "^%c_", c_rgb);    // 格式化拼接字符串
										USART_SendString( USART1, combined_string); //发送识别的物料颜色
										if( debug_key )   LCD_DisplayChar(10+16*5, 10+32*4, c_rgb);
										memset(combined_string,NULL,3*sizeof(char)); // 重置接收数组
										cnt = 0; old_color=0;
									}
								}
								else  cnt = 0;     // 中间出现错误颜色，则重置计数
								old_color = c_rgb; // 更新旧颜色
								c_rgb = NULL;      // 清颜色标志
					  }
			   }
				 USART_Cmd(USART3, DISABLE);
			}break;
			case to_Rough_r:{ // 原料区 到  粗加工区 r	
				if( first_byte==0x40 ) { //初次到区域
					to_axis( axis.x_C-100, axis.y_Raw+50, "y_first"); 
					spin(target_yaw=90,1);
					to_axis( axis.x_C,  axis.y_C_r, "y_first"); 
					//pos_compensate(first_Area_cali_cnt);
					first_byte = 0xf0; first_r = 0; 
				}			
				else { // 非初次到区域
					if( first_r == 1 ) { // 初次到色环 需要校正
					 to_axis( axis.x_C, axis.y_C_r, "y_first"); 
                        
                        //先不校准
					 Receive_host=-1; pos_compensate( cali_cnt );
           first_r = 0; 						
					} 
          else	to_axis( axis.x_C, axis.y_C_r, "y_first"); 	// 相邻色环移动			
				}  
								Task_Process = no_task;
				USART_SendString(USART1, car_Arrive);
			}break;
			case to_Rough_g:{ // 原料区 到  粗加工区 g  
				if( first_byte==0x20 ) {
					to_axis( axis.x_C-100, axis.y_Raw+50, "y_first" ); 
					spin(target_yaw=90,1);
					to_axis( axis.x_C,     axis.y_C_g, "y_first"); 
					pos_compensate(first_Area_cali_cnt);
					first_byte = 0xf0; first_g = 0;
				}
				else { 
					if( first_g == 1 ) { //需要校正
						to_axis( axis.x_C, axis.y_C_g, "y_first");
						Receive_host=-1; pos_compensate( cali_cnt );
						first_g = 0; 
					}
					else to_axis( axis.x_C, axis.y_C_g, "y_first");
				}					
		  					Task_Process = no_task;
				USART_SendString(USART1, car_Arrive);
			}break;
			case to_Rough_b:{ // 原料区 到  粗加工区 b
				if( first_byte==0x10 ) {
					to_axis( axis.x_C-100, axis.y_Raw+50, "y_first" ); 
					spin(target_yaw=90,1);
					to_axis( axis.x_C,     axis.y_C_b, "y_first"); 
					pos_compensate(first_Area_cali_cnt);
					first_byte = 0xf0; first_b = 0; 
				}
				else 	{
					if( first_b == 1 ) { //需要校正
						to_axis( axis.x_C, axis.y_C_b, "y_first");
						Receive_host=-1; pos_compensate( cali_cnt );
						first_b = 0; 
					}
					else to_axis( axis.x_C, axis.y_C_b, "y_first");  					
				}
								Task_Process = no_task;
				USART_SendString(USART1, car_Arrive);
			}break;			 
            
            
			case to_Staging_r:{ // 粗加工 到  暂存区 r	 
				if( first_byte==0xf4 ){
					to_axis( axis.x_C-100, axis.y_D-90, "x_first");
					spin(target_yaw=180,1);
					to_axis( axis.x_D_r, axis.y_D, "x_first");
				//	pos_compensate(first_Area_cali_cnt);
					first_byte = 0x0f; first_r = 1; 
				}
				else {
					if( first_r == 0 ) { //需要校正
						to_axis( axis.x_D_r,   motor.y_axis, "x_first");
						//Receive_host=-1; pos_compensate( cali_cnt );
						first_r = 1; 
					}					
				 else to_axis( axis.x_D_r,   motor.y_axis, "x_first"); 
				}				
								Task_Process = no_task;
				USART_SendString(USART1, car_Arrive);
			}break;
            
            
            
			case to_Staging_g:{ // 粗加工 到  暂存区 g
				if( first_byte==0xf2 ){
					to_axis( axis.x_C-100, axis.y_D-90, "x_first");
					spin(target_yaw=180,1);
					to_axis( axis.x_D_g, axis.y_D, "x_first" );
					pos_compensate(first_Area_cali_cnt);
					first_byte = 0x0f; first_g = 1; 
				}			
				else  {
					if( first_g == 0 ) { //需要校正
						to_axis( axis.x_D_g,   motor.y_axis, "x_first");
						Receive_host=-1; pos_compensate( cali_cnt );
						first_g = 1; 
					}					
				 else to_axis( axis.x_D_g,   motor.y_axis, "x_first"); 
				}
								Task_Process = no_task;
				USART_SendString(USART1, car_Arrive);
			}break;
			case to_Staging_b:{ // 粗加工 到  暂存区 b
				if( first_byte==0xf1 ){
					to_axis( axis.x_C-100, axis.y_D-90, "x_first");
					spin(target_yaw=180,1);
					to_axis( axis.x_D_b, axis.y_D, "x_first");
					pos_compensate(first_Area_cali_cnt); 
					first_byte = 0x0f; first_b = 1;
				}		
				else  {
					if( first_b == 0 ) { //需要校正
						to_axis( axis.x_D_b,   motor.y_axis, "x_first");
						Receive_host=-1; pos_compensate( cali_cnt );
						first_b = 1; 
					}					
				 else to_axis( axis.x_D_b,   motor.y_axis, "x_first");  
				}
				Task_Process = no_task;
				USART_SendString(USART1, car_Arrive);
			}break;			
			case Stag_to_Rough:{ // 暂存区 到  粗加工
				to_axis( axis.x_C-100, axis.y_D-90, "y_first"); // 远离暂存区
				spin(target_yaw=90,1);
        #if  back_calib == 'r' 				
				to_axis( axis.x_C,     axis.y_C_r, "y_first");  // 去粗加工r环校正
				#elif  back_calib == 'g'
				to_axis( axis.x_C,     axis.y_C_g, "y_first");	// 去粗加工g环校正		 	
				#elif  back_calib == 'b' 
				to_axis( axis.x_C,     axis.y_C_b, "y_first"); // 去粗加工b环校正
				#endif
				pos_compensate( back_cali_cnt );
				Task_Process = no_task;
				USART_SendString(USART1, car_Arrive);
			}break;	
			case Rough_to_Raw:{ // 粗加工 到  原料区
				  to_axis( axis.x_C-100, axis.y_Raw+50,"x_first" );
				  spin(target_yaw=0,1);
				if( compensate_back ) to_axis( axis.x_Raw + 70, axis.y_Raw-10,"x_first");
				else    to_axis( axis.x_Raw , axis.y_Raw,"x_first");
					motor.x_axis = axis.x_Raw;
				  motor.x_axis = axis.y_Raw;
				 Task_Process = Raw_color; // 识颜色
				 USART_SendString(USART1, car_Arrive);
			}break;
            
            
			case to_origin:{  // 到起点
				//to_axis( 80, axis.y_D-90, "y_first");
				//spin(target_yaw=270,1);				
				to_axis( 0, 294, "y_first"); 
				to_axis( 0, 0, "y_first");
				INTX_DISABLE();  //关所有中断
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
							LCD_DisplayNumber(10+16*7, 10+32*3, motor.y_axis, 5);  // 显示坐标				
							Task_Process = no_task; 
			}break; 
			case pos_cal_Stag:{
							pos_cal(3, 'S'); 
							LCD_DisplayNumber(10+16*7, 10+32*2, motor.x_axis, 5); 
							LCD_DisplayNumber(10+16*7, 10+32*3, motor.y_axis, 5);  // 显示坐标				
							Task_Process = no_task; 
			}break; 
			case x_move:  {
							to_axis( motor.x_axis + distance_blue ,  motor.y_axis, "x_first");
							LCD_DisplayNumber(10+16*7, 10+32*2, motor.x_axis, 5); 
							Task_Process = no_task; 
			}break;
			case y_move:  {
							to_axis( motor.x_axis  ,   motor.y_axis + distance_blue, "y_first");
							LCD_DisplayNumber(10+16*7, 10+32*3, motor.y_axis, 5);  // 显示坐标 				
							Task_Process = no_task; 
			}break;
			default: break;		
		}
	}
}
 // 定时访问中断
void TIM6_IRQHandler(void) {
	static char cnt;
	if(TIM6->SR&0X0001) //溢出中断
	{
		if( debug_key ){		
				cnt++;
				if( cnt==1 )	  LCD_DisplayString(10+16*3, 10+32*5, my_TX ); // 显示发送的数据
		#if TLY == 0  // 不使能陀螺仪旋向确定步数
				else if(cnt==2)		LCD_DisplayChar(10+16*3, 10+32*6, Receive_host );		// 显示来自上位机的数据
		#endif
				else if(cnt==3)    LCD_DisplayChar(10+16*5,    10+32*4, c_rgb   );
				else if(cnt==4)    LCD_DisplayNumber(10+16*6,  10+32*4, compensate_axis[0] ,4);
				else if(cnt==5)    LCD_DisplayNumber(10+16*10, 10+32*4, compensate_axis[1] ,4);
				else if(cnt==6){
					LCD_DisplayString(10+16*5, 10+32, "                            "); 
					LCD_DisplayString(10+16*5, 10+32,     Task_set_strings[Task_Process]); // 显示当前任务
					}
				if(cnt>=6) cnt=0;
		}
	} 
	TIM6->SR&=~(1<<0); //清除中断标志位 
}
 // 脉冲输出访问中断
void TIM8_UP_IRQHandler(void){ 
	 static u32 step=0;
  if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET){ 
			if( motor.mode_step==1  ){   // 梯形加减速
				 motor.sign_1_ok_step=1; //一步完成
				 TIM_Cmd(TIM8, DISABLE); // 失能 TIM8
			}
			else if(motor.mode_step==0  ){	// 陀螺仪	
					step++;
					if( step >= motor.step_max ) { //   200*motor.Subdivision__coefficient (step) == 1 (circle)
					step=0;
					motor.sign_1_ok_step=1; //一步完成
					TIM_Cmd(TIM8, DISABLE); //失能 TIM8
				}
			}
			TIM_ClearITPendingBit(TIM8, TIM_IT_Update); // 清除溢出中断标志位
	} 
}
