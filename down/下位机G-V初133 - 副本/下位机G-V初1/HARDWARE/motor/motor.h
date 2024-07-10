#ifndef __MOTOR_H
#define __MOTOR_H 			   
#include "sys.h"
#include "TIM.H"

typedef struct {  // �������ṹ�����
	u16  Subdivision__coefficient;     // ���ϸ��ϵ��
	u16  step_max;      					      //   ��Ȧ������
	u8   single_pause_NUM;  				    //  ��ʱ�����η��͵�������
	char  mode_step;                   // ���Ʒ�ʽ
  long  pulse_num;                  //   ��������
  char sign_1_ok_step;              // �趨������ɱ�־	
	float y_axis;  // 
	float x_axis;   // ����  ��λ: mm
}motor_set; 
/*   �û���    */
#define  pcr_matlab 1                 // ����ʱ���õķ�Ƶֵ
#define  sub_matlab 8                 // ����ʱ���õ�ϸ����
/*  ϵͳ��     */  
#define  PI  3.14159265359
#define  D   80          // mm
#define  C_wheel (PI*D)  // mm

void  gui_0_yaw( float target_yaw );  
void pos_cal(char num ,char area_sign );

void pos_motion_control(float mm,u8 k);

void get_yaw_0(void);
  void pos_compensate(char num );
/*�˶�����*/
  void to_axis(float target_X_axis, float target_Y_axis, char *str);
  void S_go( void  );
  void  spin( float target_yaw ,u8 sign );  
  void motion_control(float mm,u8 k);
  void dir_control(u8 k); 				 // �������


#endif 

