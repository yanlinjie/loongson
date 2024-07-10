#ifndef __SYS_H
#define __SYS_H	  
#include <stm32f10x.h> 
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "stdbool.h"
//---------------------------------坐标集-----------------------------------------------------------------------------------------------/
#define  rough_order         "RGB" // 粗加工区 色环顺序
#define  Staging_order       "RGB" // 暂存区  色环顺序
#define   back_calib         'r'   // 回原料区时，在粗加工什么色环处校正 r g b
//-------------------- 向上位机发送数据集 ------------------------------------------------------------------------------------------------/
#define  car_Receive               "^G_"   //0x47   car_Receive  :  告诉上位机,下位机已经接受完毕 
#define  car_Arrive                "^Q_"   //0x40   car_Arrive  ： 告诉上位机已经到位置了
#define  code_action               "^H_"   //0x48   code_action  ： 起点平移出来, 告诉上位机调用扫二维码的动作(说明:提前做好动作, 经过扫二维码处就可扫描) 
#define  host_calibration_action   "^I_"   //0x49   host_calibration_action  ： 告诉上位机做好小车校正的动作
//-------------------- IP( 中断优先级 ) --------------------------------------------------------------------------------------------------/
              //  tim 优先级
#define TIM6_PP        1   //   抢占优先级
#define TIM8_PP        0   //   抢占优先级
#define TIM6_SubP        3   //   子优先级
#define TIM8_SubP        0   //   子优先级
              //  uart 优先级
#define host_PP       0   //   抢占优先级
#define TLY_PP        0   //   抢占优先级
#define eye_PP        0   //   抢占优先级
#define BLUE_PP       0   //   抢占优先级
#define host_SubP      0   //   子优先级
#define TLY_SubP       2   //   子优先级
#define eye_SubP       3   //   子优先级
#define BLUE_SubP      2   //   子优先级

//----------------------- 开关 -------------------------------------------------------------------------------------/
#define compensate_back  1  // 补偿回原料区的坐标，不改变该点位坐标
#define TLY  0         // 利用陀螺仪确定旋步数，1-on , 0-off;
#define debug_key 1    // 开屏幕显示刷新
#define move_En 1   // 车动 1-on 0-off
#define EN_BLUE 1
#define first_Area_cali_cnt 2  // 初次到达区域的 单次校正次数
#define cali_cnt 2          // 初次到达色环 的单次校正次数
#define back_cali_cnt 2     // 返回原料区的沿途校正次数
//-------------------- 任务集 -------------------------------------------------------------------------------------------/
typedef  enum { 
  yaw_0, yaw_90, yaw_180, yaw_270, move_blue,x_move,y_move, // 运动指令
	no_task,             // 等待命令
	arrive,              // 到达 
  to_code,             // 去二维码区
	to_Raw,         // 去原料区
	Raw_color,
	to_Rough_r,      // 去粗加工区红色圆环处
	to_Rough_g,      // 去粗加工区绿色圆环处
  to_Rough_b,      // 去粗加工区蓝色圆环处	
	to_Staging_r,         // 去暂存区红色圆环处
	to_Staging_g,         // 去暂存区绿色圆环处
	to_Staging_b,         // 去暂存区蓝色圆环处		
	Stag_to_Rough,  //  暂存区到粗加工红圆环处 (需要校正)
	Rough_to_Raw, //  粗加工到原料区
	to_origin,           // 去起点
	pos_cal_Rough,
	pos_cal_Stag,
    to_sec,
	gui_0,
}Task_set; 
typedef  struct {    
  u16  x_code; // 二维码处x坐标
	u16  y_code;
	u16  x_Raw ;   // 原料区的 x 坐标值   
  u16  y_Raw ;    // 原料区的 y 坐标值        

	// 粗加工x都一样
	u16		  x_C;       // 粗加工区的 x 坐标值   
	u16    y_C_r ;    // 粗加工区的 y 坐标值   r1 
	u16	   y_C_b ;    //  粗加工区的 y 坐标值   b1
	u16		 y_C_g ;   // 粗加工区的 y 坐标值  g1
  // 暂存区y都一样
	u16		  x_D_r ;   // 暂存区的 x 坐标值     r2
	u16			x_D_b ;   // 暂存区的 x 坐标值    b2
	u16			x_D_g ;  // 暂存区的 x 坐标值  g2
	u16			y_D ;    // 暂存区的 y 坐标值   
}axis_Set;  


/******************************************************************************************************************************************/
//0,不支持OS
//1,支持OS
#define SYSTEM_SUPPORT_OS		0		//定义系统文件夹是否支持OS
																	    
	 
//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入
/////////////////////////////////////////////////////////////////
//Ex_NVIC_Config专用定义
#define GPIO_A 0
#define GPIO_B 1
#define GPIO_C 2
#define GPIO_D 3
#define GPIO_E 4
#define GPIO_F 5
#define GPIO_G 6 
#define FTIR   1  //下降沿触发
#define RTIR   2  //上升沿触发
								   

//JTAG模式设置定义
#define JTAG_SWD_DISABLE   0X02
#define SWD_ENABLE         0X01
#define JTAG_SWD_ENABLE    0X00	

/////////////////////////////////////////////////////////////////  
void Stm32_Clock_Init(u8 PLL);  //时钟初始化  
void Sys_Soft_Reset(void);      //系统软复位
void Sys_Standby(void);         //待机模式 	
void MY_NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset);//设置偏移地址
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group);//设置NVIC分组
void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group);//设置中断
void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM);//外部中断配置函数(只对GPIOA~G)
void JTAG_Set(u8 mode);
//////////////////////////////////////////////////////////////////////////////
//以下为汇编函数
void WFI_SET(void);		//执行WFI指令
void INTX_DISABLE(void);//关闭所有中断
void INTX_ENABLE(void);	//开启所有中断
void MSR_MSP(u32 addr);	//设置堆栈地址

#endif

/*********************************************************************************************************/
 //                                 S 加减速算法实现
/*
 f离散频率 ，1/f 为相邻样点的间隔。
 m 为拐点处的点数值 ,根据对称性，不难知道总共有2m个离散点（ 也即脉冲总数为2m ）
 问题转移到f的取定上，当脉冲数已知时，可以仿真确定较优的f，
 
 但当脉冲数，也即距离未知时，f的确定就需建立一个系统，自动寻找较优的f。
 
*/













