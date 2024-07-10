#include "motor.h"
#include "delay.h"
#include "blue.h"	  
#include "uart_2.h"

motor_set motor ={ sub_matlab,  200*sub_matlab,  1,   0,0,0,0,0 };

extern const unsigned int z_TIM_ARR[1600]; 
extern const unsigned int z_TIM_ARR_2[400]; 
extern SAngle 	stcAngle;       // ŷ����
extern char c_rgb;
extern float yaw,target_yaw,yaw_0_com; 		// ����������뺽��� ,���������
extern char sign_c_to_rgb; 
extern  int compensate_axis[2]; // ��������
extern signed char Receive_host; // ��λ������
 u8 dir_order[4]= { 0 ,1 ,2 , 3 };  // С��������Ϊ��ά��С���Զ��б��ƶ�����˳��, ���ı䷽��Ԥ������
 
/*  ���ܣ�  С������ָ�����괦 (�Զ��б��ƶ�����˳���Է��ƶ�����˳�򲻶Զ�����)
��������� target_X_axis(����x����),   target_Y_axis(����y����) ��λ:mm  */
void to_axis(float target_X_axis, float target_Y_axis, char *str) {
	float x_difference, y_difference;
	    TIM_Cmd(TIM6, DISABLE); 
			// ��ȡ��ֵ
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
		 motor.y_axis = target_Y_axis;  	// ���µ�ǰС������
			TIM_Cmd(TIM6, ENABLE);
}
/*  opencv λ�ò���   int compensate_axis[2]; // ��������  ���ű�ʾ������ǰ  */
void pos_compensate(char num  ) {
	u8 i,k;
	int data[2];
  float	KP,  data_pos_1[10], data_pos_2[10];
		TIM_Cmd(TIM6, DISABLE);
		USART_Cmd(USART3, ENABLE);
     memset(compensate_axis,0,2*sizeof(int));	//������� 	 	
		USART_SendString( USART1, host_calibration_action );  // ������λ������С��У���Ļ�е�۶��� 
		while( Receive_host!=-1 );   Receive_host = 0;   // �ȴ���λ��������̽���У׼
	  delay_ms(1500);
		for(  i=0; i<num; i++ ) {  //У�� num ��
				 for(  k=0;k<10;k++ ) // ȡ��
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
					data[0] = (int)(KP * 0.625 * Mode_array(data_pos_1) + 0.5 );   // �˲�
					data[1] = (int)(KP * 0.625 * Mode_array(data_pos_2) + 0.5 );
				// ������ƽ��, ������У�������, ��ǰ���˶�	
				if( data[1] >= 0 )  pos_motion_control( data[1], 3 ); 
				else     pos_motion_control( -data[1], 2 ); 
				if( data[0] >= 0 )  pos_motion_control( data[0], 1 ); 
				else     pos_motion_control( -data[0], 0 );
		}
		USART_Cmd(USART3, DISABLE); 
		TIM_Cmd(TIM6, ENABLE);
}
/*  opencv λ�ò���   int compensate_axis[2]; // ��������  ���ű�ʾ������ǰ  */
void pos_cal(char num ,char area_sign ) {
	u8 i,k;
	int data[2];
  float	KP,  data_pos_1[10], data_pos_2[10];
		TIM_Cmd(TIM6, DISABLE);
		USART_Cmd(USART3, ENABLE);
		memset(compensate_axis,0,2*sizeof(int));	//�������
		for(  i=0; i<num; i++ ) {  //У�� num ��
				 for(  k=0;k<10;k++ ) // ȡ��
				 {
					 while(1) { if( c_rgb=='c' )  break; }
					  c_rgb = 0;
						data_pos_1[k] = compensate_axis[0];
						data_pos_2[k] = compensate_axis[1];
				 }
				 if(i==0)       KP = 1;
				 else if(i==1)  KP = 0.8;
				 else           KP = 0.3;
					data[0] = (int)(KP * 0.625 * Mode_array(data_pos_1) + 0.5 );   // �˲�
					data[1] = (int)(KP * 0.625 * Mode_array(data_pos_2) + 0.5 );
				// ������ƽ��, ������У�������, ��ǰ���˶�	
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
/*	 С����ת ( �Զ��б��������� )    target_yaw(���������)      */
void  spin( float target_yaw, u8 sign ) {
	float bias; 
  static int old_yaw;	
				bias = target_yaw - old_yaw;	// ����Ƕ������ǽǶȿ�Խ���⣩ 
			if (bias > 180.0) 		  	bias -= 360.0;
			else if (bias < -180.0) 	bias += 360.0;
			if(bias<0 )       motion_control( 332, 12);
			else if(bias>0 )  motion_control( 332, 13);
	    old_yaw = target_yaw; //�����ϴη����
	     gui_0_yaw( target_yaw );
		 /*  ������޸ķ��������  */
		if( target_yaw == 0 )        { dir_order[0]= 0; dir_order[1]= 1;dir_order[2]= 2;dir_order[3]= 3; }
		else if( target_yaw == 90 )  { dir_order[0]= 2; dir_order[1]= 3;dir_order[2]= 1;dir_order[3]= 0; }
		else if( target_yaw == 180 ) { dir_order[0]= 1; dir_order[1]= 0;dir_order[2]= 3;dir_order[3]= 2; }
		else if( target_yaw == 270 ) { dir_order[0]= 3; dir_order[1]= 2;dir_order[2]= 0;dir_order[3]= 1; }				
}
/*	 С����ת ( �Զ��б��������� )    target_yaw(���������)      */
void  gui_0_yaw( float target_yaw ) {
	float bias=5;    
			USART_Cmd(USART2, ENABLE); //������������
    	motor.mode_step = 0;            // �޸Ŀ��Ʒ�ʽ
			TIM8->PSC = 72; // ���÷�Ƶϵ��
			TIM8->ARR = 1000; // ���ü���ֵ 	
			motor.step_max = 1;   //���õ�����			
			while(  fabs(bias) > 0.3  )  
			{  
				delay_ms(5); // �ȴ����ݻش�
				yaw = (float)stcAngle.Angle[2]/32768*180;
				  bias = target_yaw - yaw;	// ����Ƕ������ǽǶȿ�Խ���⣩ 
					if (bias > 180.0) 		  	bias -= 360.0;
					else if (bias < -180.0) 	bias += 360.0;
					if(bias<0 )      { dir_control(12);  }
					else if(bias>0 ) { dir_control(13);  }
				TIM_Cmd(TIM8, ENABLE);	  //ʹ�� TIM8
				while( !motor.sign_1_ok_step ); //�ȴ�������ɱ�־
				motor.sign_1_ok_step=0;	//���ñ�־					
			}
			#if TLY == 1
			;
			#else
			USART_Cmd(USART2, DISABLE); // �ؽ���������			
			#endif
}
/**	@brief		 С�������˶�����  
  * @t    mm: �˶����� , ��λ: mm  
					 k: 0 - ǰ��   1 - ����  2 - ����   3 - ����  
							4 - ��ǰ   5 - ���  6 - ��ǰ   7 - �Һ�  
							8 - ��ǰ����������   9 - ��ǰ����������
		          10- �ƺ�����������   11- �ƺ�����������
		          12- ����������       13- ����������  �������������������ָ����
  *	@param		 һȦ��Ӧ 235.619445 mm
	*/
u16 length = sizeof(z_TIM_ARR) / sizeof(z_TIM_ARR[0]); // �õ����ٽ׶ε�������
void motion_control(float mm,u8 k) {
		motor.mode_step = 1;         // �����˶�
	  motor.step_max = 1;         // ������
	 TIM8->PSC = pcr_matlab;    // ���÷�Ƶϵ��	
	dir_control(k);           // ���÷���
	motor.pulse_num = mm*motor.Subdivision__coefficient * 200 / C_wheel; 	// �õ��˾����Ӧ��������Ŀ 
		S_go();
}
/* У׼ʱ�������� */
void pos_motion_control(float mm,u8 k) {
		motor.mode_step = 1;         // �����˶�
	  motor.step_max = 1;         // ������
	if( mm >= 10 )                 TIM8->PSC = pcr_matlab * 12;    // ���÷�Ƶϵ��	
	else                           TIM8->PSC = pcr_matlab * 24; 
	dir_control(k);           // ���÷���
	motor.pulse_num = mm * motor.Subdivision__coefficient * 200 / C_wheel; 	// �õ��˾����Ӧ��������Ŀ 
		S_go();
}
/************************************************************
�������ܣ�s�μӼ��٣�  һ�����ö�ʱ����Ƶֵ�����ϸ��ϵ����matlab�����Ķ�Ӧ�� 
��ڲ���: dir: ����
����  ֵ����
*************************************************************/
void S_go( void  ) {
	long i;
	if( motor.pulse_num > 2*length || motor.pulse_num == 2*length ) { // x >= 2n
			/* �� */
		for( i=0 ;i<length ; i++ ) {     
			TIM8->ARR = z_TIM_ARR[i]; //װ�ؼ���ֵ
			TIM_Cmd(TIM8, ENABLE);	  // ʹ�� TIM8
			while( !motor.sign_1_ok_step ); // �ȴ��������
			motor.sign_1_ok_step=0;	// ���ñ�־
	  }
		/* ���� */
		for( i=0; i < (motor.pulse_num-2*length) ; i++ ) { 
			TIM_Cmd(TIM8, ENABLE);	  // ʹ�� TIM8
			while( !motor.sign_1_ok_step ); // �ȴ��������
			motor.sign_1_ok_step=0;	// ���ñ�־	
	  }	
		/* �� */
		for( i=length-1 ;i >= 0 ; i-- ) { 
			TIM8->ARR = z_TIM_ARR[i]; //װ�ؼ���ֵ
			TIM_Cmd(TIM8, ENABLE);	  // ʹ�� TIM8
			while( !motor.sign_1_ok_step ); // �ȴ��������
			motor.sign_1_ok_step=0;	// ���ñ�־	
	  }	
	}
	else{ // x < 2n
				/* �� */
		for( i=0 ;i < motor.pulse_num/2  ; i++ ) {  // �����γ���2��ȡ������xΪ������3/2=1,    
			TIM8->ARR = z_TIM_ARR[i]; // װ�ؼ���ֵ
			TIM_Cmd(TIM8, ENABLE);	  // ʹ�� TIM8
			while( !motor.sign_1_ok_step ); // �ȴ��������
			motor.sign_1_ok_step=0;	// ���ñ�־
	  }
		  /* ���� */
		if( motor.pulse_num%2 !=0 ) { // ������,������һ��
			TIM_Cmd(TIM8, ENABLE);	  // ʹ�� TIM8
			while( !motor.sign_1_ok_step ); // �ȴ��������
			motor.sign_1_ok_step=0;	// ���ñ�־	
	  }	
	   	/* �� */
		for( i = motor.pulse_num/2 - 1  ; i >=0 ; i-- ) { 
			TIM8->ARR = z_TIM_ARR[i]; //װ�ؼ���ֵ
			TIM_Cmd(TIM8, ENABLE);	  // ʹ�� TIM8
			while( !motor.sign_1_ok_step ); // �ȴ��������
			motor.sign_1_ok_step=0;	// ���ñ�־	
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
	L2_val =  L1_val = 	R2_val = R1_val = 0;	 // ����
	#endif
	switch(k)
	{
		 case 0:  L_1_DIR = !1; L_2_DIR = !1; R_1_DIR = 1; R_2_DIR = 1; break;
		 case 1:  L_1_DIR = !0; L_2_DIR = !0; R_1_DIR = 0; R_2_DIR = 0; break;
		 case 2:  L_1_DIR = !1; L_2_DIR = !0; R_1_DIR = 0; R_2_DIR = 1; break;
		 case 3:  L_1_DIR = !0; L_2_DIR = !1; R_1_DIR = 1; R_2_DIR = 0; break; 
		
//		 case 4:  L_1_DIR = !1;  R_2_DIR = 1; 	L2_val=0; L1_val=pause_Wide/2; R2_val=pause_Wide/2; R1_val=0; break; //��ǰ
//	   case 5:  L_2_DIR = !0; R_1_DIR = 0;   L2_val=pause_Wide/2; L1_val=0; R2_val=0; R1_val=pause_Wide/2; break;  //���
//		 case 6:  L_2_DIR = !1; R_1_DIR = 1;   L2_val=pause_Wide/2; L1_val=0; R2_val=0; R1_val=pause_Wide/2;break;  //��ǰ
//		 case 7:  L_1_DIR = !0;  R_2_DIR = 0;   L2_val=0; L1_val=pause_Wide/2; R2_val=pause_Wide/2; R1_val=0;break; //�Һ�
//		 case 8:  L_2_DIR = !1;  R_2_DIR = 0;   L2_val=pause_Wide/2; L1_val=0; R2_val=pause_Wide/2; R1_val=0;break; //��ǰ����������
//		 case 9:  L_2_DIR = !0;  R_2_DIR = 1;   L2_val=pause_Wide/2; L1_val=0; R2_val=pause_Wide/2; R1_val=0;break; //��ǰ����������
//		 case 10: L_1_DIR = !1;  R_1_DIR = 0;   L2_val=0; L1_val=pause_Wide/2; R2_val=0; R1_val=pause_Wide/2;break; //�ƺ�����������
//		 case 11: L_1_DIR = !0;  R_1_DIR = 1;   L2_val=0; L1_val=pause_Wide/2; R2_val=0; R1_val=pause_Wide/2;break; //�ƺ�����������
		 case 12: L_1_DIR = !1; L_2_DIR = !1; R_1_DIR = 0; R_2_DIR = 0;   break;  //�Ƴ�����������
	   case 13: L_1_DIR = !0; L_2_DIR = !0; R_1_DIR = 1; R_2_DIR = 1;   break;  //�Ƴ�����������
		default:  break;
	}
}
void get_yaw_0(void){
	 float array[50];
	for(u16 i=0; i<50; i++) {
			array[i] = (float)stcAngle.Angle[2]/32768*180;   
	     delay_ms( 3 );
	}
   yaw_0_com = Mode_array(array) ;   // ����
}

