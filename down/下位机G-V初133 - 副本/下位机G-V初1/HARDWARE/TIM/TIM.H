#ifndef __TIM_H
#define __TIM_H 			   
#include "sys.h"

//  开关TIM
#define close_TIM6	     TIM_Cmd(TIM6,DISABLE);  //关TIM6外设
#define close_TIM8	     TIM_Cmd(TIM8,DISABLE);  //关TIM8外设
#define open_TIM6	       TIM_Cmd(TIM6,ENABLE);  //开TIM6外设
#define open_TIM8	       TIM_Cmd(TIM8,ENABLE);  //开TIM8外设
// 脉冲IO
#define L1_val  	   TIM8->CCR4  //PC9
#define R1_val 		   TIM8->CCR2  //PC7
#define L2_val 		   TIM8->CCR3  //PC8
#define R2_val  	   TIM8->CCR1  //PC6
// 方向IO
#define L_1_DIR    PBout(15)   
#define R_1_DIR    PBout(13)   
#define L_2_DIR    PBout(0)   
#define R_2_DIR    PBout(12)     
//// 脱机IO
//#define L_1_loss   PAout(7)  	 
//#define R_1_loss   PAout(5)  	 
//#define L_2_loss   PAout(6)  	 
//#define R_2_loss   PAout(4)  	

	void GPIO_motor_EN_and_dir(void);  // 电机脱机IO, 方向IO
	void TIME_8_PWM_Init(u16 arr,u16 psc);
	void TIMER6_Init(u16 arr , u16 psc);  //  Tout= ( (arr+1) * (psc+1) )/Tclk；
  float Mode_array(float *a); // 求一组数组的众数

#endif 
