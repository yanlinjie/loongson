#ifndef ARMCAMERA_H_
#define ARMCAMERA_H_

#include "sys.h"

static char *TaskCodeStr[] = {
	"1+1","1+2","1+3",
	"2+1","2+2","2+3",
	"3+1","3+2","3+3"
};

static unsigned int TaskCodeNum[] = {
	11,12,13,
	21,22,23,
	31,32,33
};
  
//-----------------颜色任务码定义------------------------//
typedef enum{
	Red_1 = 1,					//红1
	Green_2,						//绿2
	Blue_3,							//蓝3
	NoColor ,						//无
	Init
}ColorSele;
//-----------------机械臂任务定义------------------------//
typedef enum{	
	RecoverDefault,			//恢复默认
	ColorAndRing,				//颜色识别与色环定位
}ArmTaskSet; 
//-----------------指令需求定义------------------------//
typedef enum{	
	Jump,Wait,										//发送指令后等待还是跳出
	Rough,Staging,								//选择粗加工区还是暂存区
	Grasp,Release,Avoid,MAX,MIN,	//抓夹状态
	Place,StackUp									//放置物料时平放还是码垛
}Order;
//-----------------旋转电机位置定义------------------------//
typedef enum{	
	Reset_R,Vision			//复位、识别
}Rotate;

//-----------------升降电机位置定义------------------------//
typedef enum{	
	Reset_L,Grab,  Stack,  RawArea,    Stuff,    Aim,  Rise, Fall,Lowest,    Raw,     Places,   Pile
//  复位  降抓取 降码垛 降原料转盘 降车上托盘 降校准 上升  下降  最低		原料区识别  降放置	降码垛校准
}Lifting;

extern u32  HeightNow;					//机械臂实时高度

//-----------------机械臂拿取放置动作函数---------------------//
void Ground_PutAction(Order state,ColorSele color,bool Rotate);	//放置物料动作组
void Ground_TakeAction(ColorSele color);												//从地上拿取物料并放置在物料盘
void TakeRawMaterial(void);																			//原料区拿取物料动作组
void ArmClipTakeState(Order state,bool Wait);										//物料夹是否抓取
void MaterialPosition(ColorSele color,bool Wait);								//物料盘选择颜色位置
void DirectionState(Rotate Pos);																//方向舵机位置选择（旋转电机代替）
void MotorMotion_Lift(Lifting Pos,bool Wait);										//升降电机位置选择
void MotorMotion_Rotate(Rotate Pos,bool Wait);									//旋转电机运动
void MotorMotion_Blue(void);																		//蓝牙控制电机运动

//-----------------视觉检测与动作函数------------------------//
void IdentifyColorTake(bool Update,char AloneCode);							//识别并拿取对应颜色的物料以及发送信息给车
bool VisualIdentifyTaskCode(void);															//视觉获取任务顺序

//-----------------屏幕显示函数------------------------------//
void ScreenDisplayTaskCode(uint32_t TaskCode);									//屏幕显示任务码
void ScreenDisplayPicture(const uint8_t *pImage);								//屏幕显示图片

//-----------------指令发送函数------------------------------//
void SendOrderToCar(char note,char *CarCommand,char SendCount);				//向下位机发送指令
void SendRingToCar(Order Loca,Order State,char AloneCode,bool Init);	//向下位机发送圆环指令

#endif

