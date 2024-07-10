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
//-----------------蓝牙----------------------//
extern int distance_blue, dir_blue; // 运动指令 方向及距离 mm
//-------------------------------------------//
extern motor_set motor; // 电机结构体
extern SAngle 	stcAngle;       // 欧拉角

Task_set Task_Process = no_task; //任务集
axis_Set axis; //坐标集

char sign_c_to_rgb=0;
char combined_string[3];       // RGB拼接命令
int  compensate_axis[2]; // 补偿距离
char c_rgb;
signed char Receive_host; // 上位机命令
char my_TX[50];    // 我的发送
	float yaw, target_yaw,yaw_0_com; 		//欧拉角,   航向角 yaw , yaw>0, 左偏; yaw<0,右偏 
	u8 first_byte, first_r=1,first_g=1,first_b=1; // 区域校正标志    0 r1 g1 b1  0 r2 g2 b2  8位, 仅第一次校正
	
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
