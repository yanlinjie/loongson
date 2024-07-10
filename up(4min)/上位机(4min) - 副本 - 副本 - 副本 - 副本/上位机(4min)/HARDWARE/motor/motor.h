#ifndef __MOTOR_H
#define __MOTOR_H

#include "sys.h"

#define matlab_xz_pcr                     2 	// matlab 模拟时用的 分频值
#define matlab_sj_pcr                     2 	// matlab 模拟时用的 分频值

#define matlab_Subdivision_coefficient 		8  	// matlab 模拟时用的 电机细分系数

// 升降电机使用的宏定义
#define speed_id_2  (65435)   //升降电机速度  （0-65535 越大速度越大）
//电机id
#define id_1    		1
#define id_2    		2
#define id_3    		3
#define id_4    		4
// 脉冲IO  占空比全高或全低，无变化沿，给脉冲也不会转
#define id_4_val  	TIM8->CCR4  //PC9
#define id_3_val 		TIM8->CCR3  //PC8
#define id_2_val 		TIM8->CCR2  //PC7 升降
#define id_1_val  	TIM8->CCR1  //PC6 旋转
// 方向IO
#define id_4_DIR    PBout(15)   
#define id_3_DIR    PBout(0)   
#define id_2_DIR    PBout(13)   
#define id_1_DIR    PBout(12)       
// 脱机IO   	 
#define id_4_loss   PAout(7)
#define id_3_loss   PAout(6) 
#define id_2_loss   PAout(5)
#define id_1_loss   PAout(4)  

typedef struct  // 声明结构体类型
{
	float angle;  // 角度
	float axis;   // 坐标  单位: mm
}motor_quality;
typedef struct  // 声明结构体类型  
{
	u16 Subdivision_coefficient;  // 电机细分系数
	u8  single_pause_NUM;  				// 定时器单次发送的脉冲数
	u16 step_max;      					  // 单圈脉冲数 , 最大为 200 * Subdivision_coefficient / single_pause_NUM
  char sign_1_ok_step;          // 设定步数完成标志	
	long pulse_num;               // 每次任务装载的总脉冲数
  motor_quality id[5];          // 4个电机的属性 ，0号不用
}motor_set;
extern motor_set motor;

extern char id_motor;																				//电机ID

void Motor_Gpio_Enable(void);  															//电机IO初始化
void Tcb(void(*hook_1)(float,char),float t1,char id); 			//回调函数
void Angle_Control(float target_angle,char id);							//角度控制
void no_angle_control(u16 speed,float target_angle,char id);//不带加减速的角度控制
void circle_control(u32 circle); 														//转动圈数
void step_control(u16 step);     														//转动步数
void dir_control(u8 k, char id); 														//方向控制

#endif 
