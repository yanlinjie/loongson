#ifndef __SYS_H
#define __SYS_H	  
#include <stm32f10x.h> 
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "stdbool.h"
//---------------------------------���꼯-----------------------------------------------------------------------------------------------/
#define  rough_order         "RGB" // �ּӹ��� ɫ��˳��
#define  Staging_order       "RGB" // �ݴ���  ɫ��˳��
#define   back_calib         'r'   // ��ԭ����ʱ���ڴּӹ�ʲôɫ����У�� r g b
//-------------------- ����λ���������ݼ� ------------------------------------------------------------------------------------------------/
#define  car_Receive               "^G_"   //0x47   car_Receive  :  ������λ��,��λ���Ѿ�������� 
#define  car_Arrive                "^Q_"   //0x40   car_Arrive  �� ������λ���Ѿ���λ����
#define  code_action               "^H_"   //0x48   code_action  �� ���ƽ�Ƴ���, ������λ������ɨ��ά��Ķ���(˵��:��ǰ���ö���, ����ɨ��ά�봦�Ϳ�ɨ��) 
#define  host_calibration_action   "^I_"   //0x49   host_calibration_action  �� ������λ������С��У���Ķ���
//-------------------- IP( �ж����ȼ� ) --------------------------------------------------------------------------------------------------/
              //  tim ���ȼ�
#define TIM6_PP        1   //   ��ռ���ȼ�
#define TIM8_PP        0   //   ��ռ���ȼ�
#define TIM6_SubP        3   //   �����ȼ�
#define TIM8_SubP        0   //   �����ȼ�
              //  uart ���ȼ�
#define host_PP       0   //   ��ռ���ȼ�
#define TLY_PP        0   //   ��ռ���ȼ�
#define eye_PP        0   //   ��ռ���ȼ�
#define BLUE_PP       0   //   ��ռ���ȼ�
#define host_SubP      0   //   �����ȼ�
#define TLY_SubP       2   //   �����ȼ�
#define eye_SubP       3   //   �����ȼ�
#define BLUE_SubP      2   //   �����ȼ�

//----------------------- ���� -------------------------------------------------------------------------------------/
#define compensate_back  1  // ������ԭ���������꣬���ı�õ�λ����
#define TLY  0         // ����������ȷ����������1-on , 0-off;
#define debug_key 1    // ����Ļ��ʾˢ��
#define move_En 1   // ���� 1-on 0-off
#define EN_BLUE 1
#define first_Area_cali_cnt 2  // ���ε�������� ����У������
#define cali_cnt 2          // ���ε���ɫ�� �ĵ���У������
#define back_cali_cnt 2     // ����ԭ��������;У������
//-------------------- ���� -------------------------------------------------------------------------------------------/
typedef  enum { 
  yaw_0, yaw_90, yaw_180, yaw_270, move_blue,x_move,y_move, // �˶�ָ��
	no_task,             // �ȴ�����
	arrive,              // ���� 
  to_code,             // ȥ��ά����
	to_Raw,         // ȥԭ����
	Raw_color,
	to_Rough_r,      // ȥ�ּӹ�����ɫԲ����
	to_Rough_g,      // ȥ�ּӹ�����ɫԲ����
  to_Rough_b,      // ȥ�ּӹ�����ɫԲ����	
	to_Staging_r,         // ȥ�ݴ�����ɫԲ����
	to_Staging_g,         // ȥ�ݴ�����ɫԲ����
	to_Staging_b,         // ȥ�ݴ�����ɫԲ����		
	Stag_to_Rough,  //  �ݴ������ּӹ���Բ���� (��ҪУ��)
	Rough_to_Raw, //  �ּӹ���ԭ����
	to_origin,           // ȥ���
	pos_cal_Rough,
	pos_cal_Stag,
    to_sec,
	gui_0,
}Task_set; 
typedef  struct {    
  u16  x_code; // ��ά�봦x����
	u16  y_code;
	u16  x_Raw ;   // ԭ������ x ����ֵ   
  u16  y_Raw ;    // ԭ������ y ����ֵ        

	// �ּӹ�x��һ��
	u16		  x_C;       // �ּӹ����� x ����ֵ   
	u16    y_C_r ;    // �ּӹ����� y ����ֵ   r1 
	u16	   y_C_b ;    //  �ּӹ����� y ����ֵ   b1
	u16		 y_C_g ;   // �ּӹ����� y ����ֵ  g1
  // �ݴ���y��һ��
	u16		  x_D_r ;   // �ݴ����� x ����ֵ     r2
	u16			x_D_b ;   // �ݴ����� x ����ֵ    b2
	u16			x_D_g ;  // �ݴ����� x ����ֵ  g2
	u16			y_D ;    // �ݴ����� y ����ֵ   
}axis_Set;  


/******************************************************************************************************************************************/
//0,��֧��OS
//1,֧��OS
#define SYSTEM_SUPPORT_OS		0		//����ϵͳ�ļ����Ƿ�֧��OS
																	    
	 
//λ������,ʵ��51���Ƶ�GPIO���ƹ���
//����ʵ��˼��,�ο�<<CM3Ȩ��ָ��>>������(87ҳ~92ҳ).
//IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO�ڵ�ַӳ��
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
 
//IO�ڲ���,ֻ�Ե�һ��IO��!
//ȷ��n��ֵС��16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����
/////////////////////////////////////////////////////////////////
//Ex_NVIC_Configר�ö���
#define GPIO_A 0
#define GPIO_B 1
#define GPIO_C 2
#define GPIO_D 3
#define GPIO_E 4
#define GPIO_F 5
#define GPIO_G 6 
#define FTIR   1  //�½��ش���
#define RTIR   2  //�����ش���
								   

//JTAGģʽ���ö���
#define JTAG_SWD_DISABLE   0X02
#define SWD_ENABLE         0X01
#define JTAG_SWD_ENABLE    0X00	

/////////////////////////////////////////////////////////////////  
void Stm32_Clock_Init(u8 PLL);  //ʱ�ӳ�ʼ��  
void Sys_Soft_Reset(void);      //ϵͳ��λ
void Sys_Standby(void);         //����ģʽ 	
void MY_NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset);//����ƫ�Ƶ�ַ
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group);//����NVIC����
void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group);//�����ж�
void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM);//�ⲿ�ж����ú���(ֻ��GPIOA~G)
void JTAG_Set(u8 mode);
//////////////////////////////////////////////////////////////////////////////
//����Ϊ��ຯ��
void WFI_SET(void);		//ִ��WFIָ��
void INTX_DISABLE(void);//�ر������ж�
void INTX_ENABLE(void);	//���������ж�
void MSR_MSP(u32 addr);	//���ö�ջ��ַ

#endif

/*********************************************************************************************************/
 //                                 S �Ӽ����㷨ʵ��
/*
 f��ɢƵ�� ��1/f Ϊ��������ļ����
 m Ϊ�յ㴦�ĵ���ֵ ,���ݶԳ��ԣ�����֪���ܹ���2m����ɢ�㣨 Ҳ����������Ϊ2m ��
 ����ת�Ƶ�f��ȡ���ϣ�����������֪ʱ�����Է���ȷ�����ŵ�f��
 
 ������������Ҳ������δ֪ʱ��f��ȷ�����轨��һ��ϵͳ���Զ�Ѱ�ҽ��ŵ�f��
 
*/













