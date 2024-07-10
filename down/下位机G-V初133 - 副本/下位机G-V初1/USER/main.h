#ifndef __MAIN_H
#define __MAIN_H

#include "sys.h"
#include "delay.h"
#include "motor.h"
#include "TIM.H"
#include "usart_3.h"	  
#include "usart_init.h"	
#include "blue.h"	  
#include "uart_2.h"
#include "JY901.h"
#include "lcd_spi_169.h"

#include "24cxx.h" 
//-----------------����----------------------//
extern int distance_blue, dir_blue; // �˶�ָ�� ���򼰾��� mm
//-------------------------------------------//
extern motor_set motor; // ����ṹ��
extern SAngle 	stcAngle;       // ŷ����

Task_set Task_Process = no_task; //����
axis_Set axis; //���꼯

char sign_c_to_rgb=0;
char combined_string[3];       // RGBƴ������
int  compensate_axis[2]; // ��������
char c_rgb;
signed char Receive_host; // ��λ������
char my_TX[50];    // �ҵķ���
	float yaw, target_yaw,yaw_0_com; 		//ŷ����,   ����� yaw , yaw>0, ��ƫ; yaw<0,��ƫ 
	u8 first_byte, first_r=1,first_g=1,first_b=1; // ����У����־    0 r1 g1 b1  0 r2 g2 b2  8λ, ����һ��У��
	
char* Task_set_strings[] = {
    "yaw_0",
    "yaw_90",
    "yaw_180",
    "yaw_270",
    "move_blue",
	  "x_move",
    "y_move",
    "no_task",
    "arrive",
    "to_code",
    "to_Raw",
	  "Raw_color",
    "to_Rough_r",
    "to_Rough_g",
    "to_Rough_b",
    "to_Staging_r",
    "to_Staging_g",
    "to_Staging_b",
    "Stag_to_Rough",
    "Rough_to_Raw",
    "to_sec"
    "to_origin",
		"pos_cal_Stag",
		"pos_cal_Rough",
		"gui_0",
};
#endif
