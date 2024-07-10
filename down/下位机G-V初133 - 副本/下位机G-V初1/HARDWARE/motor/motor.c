#include "motor.h"
#include "delay.h"
#include "blue.h"	  
#include "uart_2.h"

motor_set motor ={ sub_matlab,  200*sub_matlab,  1,   0,0,0,0,0 };

extern const unsigned int z_TIM_ARR[1600]; 
extern const unsigned int z_TIM_ARR_2[400]; 
extern SAngle 	stcAngle;       // 欧拉角
extern char c_rgb;
extern float yaw,target_yaw,yaw_0_com; 		// 期望航向角与航向角 ,及补偿零点
extern char sign_c_to_rgb; 
extern  int compensate_axis[2]; // 补偿距离
extern signed char Receive_host; // 上位机命令
 u8 dir_order[4]= { 0 ,1 ,2 , 3 };  // 小车自旋后，为了维持小车自动判别移动方向顺序, 而改变方向预设数组
 
/*  功能：  小车到达指定坐标处 (自动判别移动方向顺序，以防移动方向顺序不对而出界)
传入参数： target_X_axis(期望x坐标),   target_Y_axis(期望y坐标) 单位:mm  */
void to_axis(float target_X_axis, float target_Y_axis, char *str) {
	float x_difference, y_difference;
	    TIM_Cmd(TIM6, DISABLE); 
			// 获取差值
			x_difference = target_X_axis - motor.x_axis ;  
			y_difference = target_Y_axis - motor.y_axis ;	
			if(  strcmp( str,"x_first" ) == 0 )
			{
					if( x_difference > 0 )            motion_control(  x_difference  , dir_order[0]);
					else if( x_difference < 0 )       motion_control( -x_difference  , dir_order[1]); 
					if( x_difference != 0 )          				    gui_0_yaw(target_yaw);
					if( y_difference > 0 )            motion_control(  y_difference  , dir_order[3]);
					else if( y_difference < 0 )       motion_control( -y_difference  , dir_order[2]);
					if( y_difference != 0 )          				    gui_0_yaw(target_yaw);		
			}
			else if( strcmp( str,"y_first" ) == 0 )
			{
					if( y_difference > 0 )            motion_control(  y_difference  , dir_order[3]);
					else if( y_difference < 0 )       motion_control( -y_difference  , dir_order[2]);
					if( y_difference != 0 )          				    gui_0_yaw(target_yaw);	
					if( x_difference > 0 )            motion_control(  x_difference , dir_order[0]);
					else if( x_difference < 0 )       motion_control( -x_difference  , dir_order[1]); 
					if( x_difference != 0 )          				    gui_0_yaw(target_yaw);				
			}
     motor.x_axis = target_X_axis;			
		 motor.y_axis = target_Y_axis;  	// 更新当前小车坐标
			TIM_Cmd(TIM6, ENABLE);
}
/*  opencv 位置补偿   int compensate_axis[2]; // 补偿距离  负号表示向左向前  */
void pos_compensate(char num  ) {
	u8 i,k;
	int data[2];
  float	KP,  data_pos_1[10], data_pos_2[10];
		TIM_Cmd(TIM6, DISABLE);
		USART_Cmd(USART3, ENABLE);
     memset(compensate_axis,0,2*sizeof(int));	//清空数组 	 	
		USART_SendString( USART1, host_calibration_action );  // 告诉上位机做好小车校正的机械臂动作 
		while( Receive_host!=-1 );   Receive_host = 0;   // 等待上位机允许底盘进行校准
	  delay_ms(1500);
		for(  i=0; i<num; i++ ) {  //校正 num 次
				 for(  k=0;k<10;k++ ) // 取样
				 {
					 while(1) { 
					  if( c_rgb=='c' )  break; 
					 }
					  c_rgb = 0;
						data_pos_1[k] = compensate_axis[0];
						data_pos_2[k] = compensate_axis[1];
				 }
				 if(i==0)       KP = 1;
				 else if(i==1)  KP = 0.8;
				 else           KP = 0.3;
					data[0] = (int)(KP * 0.625 * Mode_array(data_pos_1) + 0.5 );   // 滤波
					data[1] = (int)(KP * 0.625 * Mode_array(data_pos_2) + 0.5 );
				// 先左右平移, 陀螺仪校正方向后, 再前后运动	
				if( data[1] >= 0 )  pos_motion_control( data[1], 3 ); 
				else     pos_motion_control( -data[1], 2 ); 
				if( data[0] >= 0 )  pos_motion_control( data[0], 1 ); 
				else     pos_motion_control( -data[0], 0 );
		}
		USART_Cmd(USART3, DISABLE); 
		TIM_Cmd(TIM6, ENABLE);
}
/*  opencv 位置补偿   int compensate_axis[2]; // 补偿距离  负号表示向左向前  */
void pos_cal(char num ,char area_sign ) {
	u8 i,k;
	int data[2];
  float	KP,  data_pos_1[10], data_pos_2[10];
		TIM_Cmd(TIM6, DISABLE);
		USART_Cmd(USART3, ENABLE);
		memset(compensate_axis,0,2*sizeof(int));	//清空数组
		for(  i=0; i<num; i++ ) {  //校正 num 次
				 for(  k=0;k<10;k++ ) // 取样
				 {
					 while(1) { if( c_rgb=='c' )  break; }
					  c_rgb = 0;
						data_pos_1[k] = compensate_axis[0];
						data_pos_2[k] = compensate_axis[1];
				 }
				 if(i==0)       KP = 1;
				 else if(i==1)  KP = 0.8;
				 else           KP = 0.3;
					data[0] = (int)(KP * 0.625 * Mode_array(data_pos_1) + 0.5 );   // 滤波
					data[1] = (int)(KP * 0.625 * Mode_array(data_pos_2) + 0.5 );
				// 先左右平移, 陀螺仪校正方向后, 再前后运动	
				if( data[1] >= 0 )  pos_motion_control( data[1], 3 ); 
				else     pos_motion_control( -data[1], 2 ); 
				if( data[0] >= 0 )  pos_motion_control( data[0], 1 ); 
				else     pos_motion_control( -data[0], 0 );
				 if( area_sign == 'R' )      { motor.x_axis -= data[1];  motor.y_axis -= data[0]; }
				 else if( area_sign == 'S' ) { motor.x_axis += data[0];  motor.y_axis -= data[1]; }
		}
		USART_Cmd(USART3, DISABLE); 
		TIM_Cmd(TIM6, ENABLE);
}
/*	 小车自转 ( 自动判别自旋方向 )    target_yaw(期望航向角)      */
void  spin( float target_yaw, u8 sign ) {
	float bias; 
  static int old_yaw;	
				bias = target_yaw - old_yaw;	// 计算角度误差（考虑角度跨越问题） 
			if (bias > 180.0) 		  	bias -= 360.0;
			else if (bias < -180.0) 	bias += 360.0;
			if(bias<0 )       motion_control( 332, 12);
			else if(bias>0 )  motion_control( 332, 13);
	    old_yaw = target_yaw; //更新上次方向角
	     gui_0_yaw( target_yaw );
		 /*  旋向后，修改方向控制字  */
		if( target_yaw == 0 )        { dir_order[0]= 0; dir_order[1]= 1;dir_order[2]= 2;dir_order[3]= 3; }
		else if( target_yaw == 90 )  { dir_order[0]= 2; dir_order[1]= 3;dir_order[2]= 1;dir_order[3]= 0; }
		else if( target_yaw == 180 ) { dir_order[0]= 1; dir_order[1]= 0;dir_order[2]= 3;dir_order[3]= 2; }
		else if( target_yaw == 270 ) { dir_order[0]= 3; dir_order[1]= 2;dir_order[2]= 0;dir_order[3]= 1; }				
}
/*	 小车自转 ( 自动判别自旋方向 )    target_yaw(期望航向角)      */
void  gui_0_yaw( float target_yaw ) {
	float bias=5;    
			USART_Cmd(USART2, ENABLE); //开接受陀螺仪
    	motor.mode_step = 0;            // 修改控制方式
			TIM8->PSC = 72; // 设置分频系数
			TIM8->ARR = 1000; // 设置计数值 	
			motor.step_max = 1;   //设置单步发			
			while(  fabs(bias) > 0.3  )  
			{  
				delay_ms(5); // 等待数据回传
				yaw = (float)stcAngle.Angle[2]/32768*180;
				  bias = target_yaw - yaw;	// 计算角度误差（考虑角度跨越问题） 
					if (bias > 180.0) 		  	bias -= 360.0;
					else if (bias < -180.0) 	bias += 360.0;
					if(bias<0 )      { dir_control(12);  }
					else if(bias>0 ) { dir_control(13);  }
				TIM_Cmd(TIM8, ENABLE);	  //使能 TIM8
				while( !motor.sign_1_ok_step ); //等待步数完成标志
				motor.sign_1_ok_step=0;	//重置标志					
			}
			#if TLY == 1
			;
			#else
			USART_Cmd(USART2, DISABLE); // 关接受陀螺仪			
			#endif
}
/**	@brief		 小车整体运动控制  
  * @t    mm: 运动距离 , 单位: mm  
					 k: 0 - 前进   1 - 后退  2 - 右移   3 - 左移  
							4 - 左前   5 - 左后  6 - 右前   7 - 右后  
							8 - 绕前轮中轴左旋   9 - 绕前轮中轴右旋
		          10- 绕后轮中轴左旋   11- 绕后轮中轴右旋
		          12- 绕中轴左旋       13- 绕中轴右旋  （左右手螺旋定则的四指旋向）
  *	@param		 一圈对应 235.619445 mm
	*/
u16 length = sizeof(z_TIM_ARR) / sizeof(z_TIM_ARR[0]); // 得到加速阶段的脉冲数
void motion_control(float mm,u8 k) {
		motor.mode_step = 1;         // 曲线运动
	  motor.step_max = 1;         // 单步发
	 TIM8->PSC = pcr_matlab;    // 设置分频系数	
	dir_control(k);           // 设置方向
	motor.pulse_num = mm*motor.Subdivision__coefficient * 200 / C_wheel; 	// 得到此距离对应的脉冲数目 
		S_go();
}
/* 校准时，慢速走 */
void pos_motion_control(float mm,u8 k) {
		motor.mode_step = 1;         // 曲线运动
	  motor.step_max = 1;         // 单步发
	if( mm >= 10 )                 TIM8->PSC = pcr_matlab * 12;    // 设置分频系数	
	else                           TIM8->PSC = pcr_matlab * 24; 
	dir_control(k);           // 设置方向
	motor.pulse_num = mm * motor.Subdivision__coefficient * 200 / C_wheel; 	// 得到此距离对应的脉冲数目 
		S_go();
}
/************************************************************
函数功能：s形加减速，  一定设置定时器分频值、电机细分系数与matlab导出的对应上 
入口参数: dir: 方向
返回  值：无
*************************************************************/
void S_go( void  ) {
	long i;
	if( motor.pulse_num > 2*length || motor.pulse_num == 2*length ) { // x >= 2n
			/* 升 */
		for( i=0 ;i<length ; i++ ) {     
			TIM8->ARR = z_TIM_ARR[i]; //装载计数值
			TIM_Cmd(TIM8, ENABLE);	  // 使能 TIM8
			while( !motor.sign_1_ok_step ); // 等待步数完成
			motor.sign_1_ok_step=0;	// 重置标志
	  }
		/* 匀速 */
		for( i=0; i < (motor.pulse_num-2*length) ; i++ ) { 
			TIM_Cmd(TIM8, ENABLE);	  // 使能 TIM8
			while( !motor.sign_1_ok_step ); // 等待步数完成
			motor.sign_1_ok_step=0;	// 重置标志	
	  }	
		/* 降 */
		for( i=length-1 ;i >= 0 ; i-- ) { 
			TIM8->ARR = z_TIM_ARR[i]; //装载计数值
			TIM_Cmd(TIM8, ENABLE);	  // 使能 TIM8
			while( !motor.sign_1_ok_step ); // 等待步数完成
			motor.sign_1_ok_step=0;	// 重置标志	
	  }	
	}
	else{ // x < 2n
				/* 升 */
		for( i=0 ;i < motor.pulse_num/2  ; i++ ) {  // 长整形除以2，取整，若x为奇数，3/2=1,    
			TIM8->ARR = z_TIM_ARR[i]; // 装载计数值
			TIM_Cmd(TIM8, ENABLE);	  // 使能 TIM8
			while( !motor.sign_1_ok_step ); // 等待步数完成
			motor.sign_1_ok_step=0;	// 重置标志
	  }
		  /* 匀速 */
		if( motor.pulse_num%2 !=0 ) { // 奇数步,则匀速一步
			TIM_Cmd(TIM8, ENABLE);	  // 使能 TIM8
			while( !motor.sign_1_ok_step ); // 等待步数完成
			motor.sign_1_ok_step=0;	// 重置标志	
	  }	
	   	/* 降 */
		for( i = motor.pulse_num/2 - 1  ; i >=0 ; i-- ) { 
			TIM8->ARR = z_TIM_ARR[i]; //装载计数值
			TIM_Cmd(TIM8, ENABLE);	  // 使能 TIM8
			while( !motor.sign_1_ok_step ); // 等待步数完成
			motor.sign_1_ok_step=0;	// 重置标志	
	  }	
	}
}
void dir_control(u8 k) {
	#if move_En == 1		  
	if(motor.mode_step == 1)
	L2_val =  L1_val = 	R2_val = R1_val = z_TIM_ARR[length-1] / 2;	
	else
	L2_val =  L1_val = 	R2_val = R1_val = TIM8->ARR/2;	
	#else
	L2_val =  L1_val = 	R2_val = R1_val = 0;	 // 不动
	#endif
	switch(k)
	{
		 case 0:  L_1_DIR = !1; L_2_DIR = !1; R_1_DIR = 1; R_2_DIR = 1; break;
		 case 1:  L_1_DIR = !0; L_2_DIR = !0; R_1_DIR = 0; R_2_DIR = 0; break;
		 case 2:  L_1_DIR = !1; L_2_DIR = !0; R_1_DIR = 0; R_2_DIR = 1; break;
		 case 3:  L_1_DIR = !0; L_2_DIR = !1; R_1_DIR = 1; R_2_DIR = 0; break; 
		
//		 case 4:  L_1_DIR = !1;  R_2_DIR = 1; 	L2_val=0; L1_val=pause_Wide/2; R2_val=pause_Wide/2; R1_val=0; break; //左前
//	   case 5:  L_2_DIR = !0; R_1_DIR = 0;   L2_val=pause_Wide/2; L1_val=0; R2_val=0; R1_val=pause_Wide/2; break;  //左后
//		 case 6:  L_2_DIR = !1; R_1_DIR = 1;   L2_val=pause_Wide/2; L1_val=0; R2_val=0; R1_val=pause_Wide/2;break;  //右前
//		 case 7:  L_1_DIR = !0;  R_2_DIR = 0;   L2_val=0; L1_val=pause_Wide/2; R2_val=pause_Wide/2; R1_val=0;break; //右后
//		 case 8:  L_2_DIR = !1;  R_2_DIR = 0;   L2_val=pause_Wide/2; L1_val=0; R2_val=pause_Wide/2; R1_val=0;break; //绕前轮中轴左旋
//		 case 9:  L_2_DIR = !0;  R_2_DIR = 1;   L2_val=pause_Wide/2; L1_val=0; R2_val=pause_Wide/2; R1_val=0;break; //绕前轮中轴右旋
//		 case 10: L_1_DIR = !1;  R_1_DIR = 0;   L2_val=0; L1_val=pause_Wide/2; R2_val=0; R1_val=pause_Wide/2;break; //绕后轮中轴左旋
//		 case 11: L_1_DIR = !0;  R_1_DIR = 1;   L2_val=0; L1_val=pause_Wide/2; R2_val=0; R1_val=pause_Wide/2;break; //绕后轮中轴右旋
		 case 12: L_1_DIR = !1; L_2_DIR = !1; R_1_DIR = 0; R_2_DIR = 0;   break;  //绕车子中心左旋
	   case 13: L_1_DIR = !0; L_2_DIR = !0; R_1_DIR = 1; R_2_DIR = 1;   break;  //绕车子中心右旋
		default:  break;
	}
}
void get_yaw_0(void){
	 float array[50];
	for(u16 i=0; i<50; i++) {
			array[i] = (float)stcAngle.Angle[2]/32768*180;   
	     delay_ms( 3 );
	}
   yaw_0_com = Mode_array(array) ;   // 众数
}

