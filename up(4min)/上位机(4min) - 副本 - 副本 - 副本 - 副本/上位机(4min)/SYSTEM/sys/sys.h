#ifndef __SYS_H
#define __SYS_H	

#include "stm32f10x.h"

#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>	
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#include "delay.h"
#include "led.h"
#include "key.h" 
#include "tim.h"
#include "fifo.h"
#include "usart.h"		
#include "lcd_spi_169.h"
#include "motor.h" 
#include "ArmCamera.h"
#include "WS2812.h"
#include "EspCommand.h"

//-----------------任务运行流程定义------------------------//
typedef enum{ 
	TaskStart,						//任务开始
	ScanQR,								//扫描二维码
	TakeRaw1,							//到达原料区后第一次拿取原料
	GoRough,TPRough1,			//前往粗加工区、第一次放置与拿取原料
	GoStaging,PutStag1,		//前往暂存区、第一次放置原料
	PassRough,TakeRaw2,		//经过粗加工区、返回原料区后第二次拿取原料 
	GoRough2,TPRough2,		//第二次前往粗加工区、第二次放置与拿取原料
	GoStag2,PutStag2,			//第二次前往暂存区、第二次放置原料
	Restart,Calibrat			//返回起点、底盘校准
}TaskSet; 
static char *TaskString[] = {
	"InitWait",
	"ScanQR",
	"TakeRaw1",
	"GoRough","TPRough1",
	"GoStaging","PutStag1",
	"PassRough","TakeRaw2",
	"GoRough2","TPRough2",
	"GoStag2","PutStag2",
	"Restart","Calibrat"	
};

//-----------------二维码接收模式------------------------//
typedef enum{
	ScanModule,ESP8266,DT_06 
}QR_Rece; 


//-----------------任务码相关定义------------------------//
typedef struct{
	TaskSet NowTask;			//当前任务
	TaskSet OldTask;			//上一次任务
	unsigned int CodeS;		//完整任务码
	unsigned int Code1;		//第一次任务码
	unsigned int Code2;		//第二次任务码
}TaskState;
extern TaskState SysTask; 

//-----------------任务控制定义------------------------//
typedef struct{
	bool TaskStart;				//任务是否开始
	bool DebugStart;			//是否进入调试
	bool RaceStart;				//是否进入竞赛
	bool DirectGrab;			//是否直接抓取(不看颜色)
	QR_Rece QR_Mode;			//二维码接收方式
	char ReceCarSta;			//收到下位机的状态
}Operat_Control;
extern Operat_Control SysValue; 

//-----------------下位机通信宏定义----------------------//
//-------发送-------------发送-------------发送---//
#define GoRawAreaS		"^B_"			//去原料区-------------------1
#define GoRoughRED		"^C_"			//去粗加工区红圆环-----------2
#define GoRoughGREEN	"^D_"			//去粗加工区绿圆环-----------3
#define GoRoughBLUE		"^E_"			//去粗加工区蓝圆环-----------4

#define GoStoreRED		"^L_"			//去暂存区红圆环-------------5
#define GoStoreGREEN	"^M_"			//去暂存区绿圆环-------------6
#define GoStoreBLUE		"^N_"			//去暂存区蓝圆环-------------7

#define StoreGoRough	"^J_"			//暂存区到粗加工区-----------8
#define RoughGoRaw		"^K_"			//粗加工到原料区-------------9

#define RestartS			"^F_"			//返回起点-------------------10
#define Calibration		"^P_"			//允许底盘进行校准-----------11
#define ReceCarSent		"^O_"		  //收到底盘发送的消息

//--------接收------------接收-------------接收--------//
#define ArriveLocation	0x51	//底盘已到达指定位置（Q）------1
//#define ReceiveMessage	0x47	//下位机确认收到指令（G）
//#define RaiseArmQRCode	0x48	//举臂扫描二维码（H）----------2
#define CarCalibration	0x49	//运行底盘校准（I）------------3
#define MaterialRED			0x72	//检测到红色色块（r）----------4
#define MaterialGREEN		0x67	//检测到绿色色块（g）----------5
#define MaterialBLUE		0x62	//检测到蓝色色块（b）----------6

//0,不支持ucos  1,支持ucos
#define SYSTEM_SUPPORT_OS		0		//定义系统文件夹是否支持UCOS
																	    
//位带操作,实现51类似的GPIO控制功能，IO口操作宏定义
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

#define GPIOA_IDR_Addr    (GPIOA_BASE+8)  //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8)  //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8)  //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8)  //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8)  //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8)  //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8)  //0x40011E08 
 
//IO口操作,只对单一的IO口!确保n的值小于16!
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

//以下为汇编函数
void WFI_SET(void);						//执行WFI指令
void INTX_DISABLE(void);			//关闭所有中断
void INTX_ENABLE(void);				//开启所有中断
void MSR_MSP(u32 addr);				//设置堆栈地址

void SystemInitialize(bool Debug,bool Race,bool Grab,QR_Rece Sele);	
//配置系统(是否调试、是否竞赛、是否直接抓)

void KeyControlTask(void);		//按键控制
void OperatingTask(void);			//任务逻辑
void SysDetectTask(void);			//程序调试

#endif
