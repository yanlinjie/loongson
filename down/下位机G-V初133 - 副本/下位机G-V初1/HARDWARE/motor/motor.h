#ifndef __MOTOR_H
#define __MOTOR_H 			   
#include "sys.h"
#include "TIM.H"

typedef struct {  // 定义电机结构体变量
	u16  Subdivision__coefficient;     // 电机细分系数
	u16  step_max;      					      //   单圈脉冲数
	u8   single_pause_NUM;  				    //  定时器单次发送的脉冲数
	char  mode_step;                   // 控制方式
  long  pulse_num;                  //   总脉冲数
  char sign_1_ok_step;              // 设定步数完成标志	
	float y_axis;  // 
	float x_axis;   // 坐标  单位: mm
}motor_set; 
/*   用户宏    */
#define  pcr_matlab 1                 // 仿真时设置的分频值
#define  sub_matlab 8                 // 仿真时设置的细分数
/*  系统宏     */  
#define  PI  3.14159265359
#define  D   80          // mm
#define  C_wheel (PI*D)  // mm

void  gui_0_yaw( float target_yaw );  
void pos_cal(char num ,char area_sign );

void pos_motion_control(float mm,u8 k);

void get_yaw_0(void);
  void pos_compensate(char num );
/*运动控制*/
  void to_axis(float target_X_axis, float target_Y_axis, char *str);
  void S_go( void  );
  void  spin( float target_yaw ,u8 sign );  
  void motion_control(float mm,u8 k);
  void dir_control(u8 k); 				 // 方向控制


#endif 

