#include "ArmCamera.h"

/**************************************************************************
函数功能：从物料盘拿取并放置物料动作组
入口参数：AloneCode：任务码位值、color：任务码对应的颜色、Rotate：是否让旋转电机复位
返回  值：无
**************************************************************************/
void Ground_PutAction(Order state,ColorSele color,bool Rotate){ 
//	static char PutCount;
//	PutCount++;
	MaterialPosition(color,true);			//物料托盘提前旋转至下一个颜色所需位置 
	MotorMotion_Lift(Stuff,false);			//升降电机下降至车上托盘 true
	ArmClipTakeState(Grasp,true);				//抓夹夹紧
	MotorMotion_Lift(Reset_L,false);		//升降复位
	MotorMotion_Rotate(Vision,true);		//旋转电机转向
	if(state == Place)				MotorMotion_Lift(Places,false);			//升降触底
	else if(state == StackUp)	MotorMotion_Lift(Stack,false);			//升降码垛
	delay_ms(400);
	ArmClipTakeState(Avoid,true);				//抓夹松开
	
/*	if(PutCount < 3){	
		switch(SysTask.NowTask){									//第一、二次抓取物料放置
			case TPRough1:SendRingToCar(Rough,Wait,(int)color,false);	 break;	//粗加工区内移动至下一个色环
			case PutStag1:SendRingToCar(Staging,Wait,(int)color,false);break;	//暂存区内移动至下一个色环
			case TPRough2:SendRingToCar(Rough,Wait,(int)color,false);	 break;	//粗加工区内移动至下一个色环
			case PutStag2:{
						MotorMotion_Lift(Pile,false);																//升降码垛校准
						SendRingToCar(Staging,Wait,(int)color,false);								//暂存区内移动至下一个色环
				}break;	
		}
		MotorMotion_Lift(Reset_L,false);																		//升降电机复位
	}
	else{
		switch(SysTask.NowTask){									//第三次抓取物料放置
			case TPRough1:SendRingToCar(Rough,Wait,(int)color,false);break;		//粗加工区内移动至下一个色环
			case PutStag1:SendOrderToCar(false,StoreGoRough,5);			 break;		//前往粗加工区
			case TPRough2:SendRingToCar(Rough,Wait,(int)color,false);break;		//粗加工区内移动至下一个色环
			case PutStag2:{																										//码垛完成
							SendOrderToCar(false,RestartS,5);													//返回起点	
							MotorMotion_Lift(Reset_L,false);													//升降电机复位
				}break;
		}
		PutCount = 0;		
	}*/
	if(Rotate)	MotorMotion_Rotate(Reset_R,false);												//旋转电机是否复位
}

/**************************************************************************
函数功能：从地上拿取物料并放置在物料盘动作组
入口参数：AloneCode：任务码位值
返回  值：无
**************************************************************************/
void Ground_TakeAction(ColorSele color){
	static char TakeCount;
	TakeCount++;
	MaterialPosition(color,false);			// 物料托盘提前旋转至所需要
	ArmClipTakeState(Avoid,false);			// 抓夹稍微夹紧避免碰撞
	MotorMotion_Lift(Grab,false);				// 升降电机触底
	ArmClipTakeState(Grasp,true);				// 抓夹夹紧
	MotorMotion_Lift(Reset_L,false);		// 升降电机复位
	MotorMotion_Rotate(Reset_R,true);		// 旋转电机复位
	MotorMotion_Lift(Stuff,true);				// 升降电机下降至车上托盘
	ArmClipTakeState(Release,true);			// 抓夹松开(松开得足够大，升降电机不需要复位)
//	if(SysTask.NowTask == TPRough1){
//		if(TakeCount < 3)	SendRingToCar(Rough,Jump,(int)color,false);
//		else{															
//			SendRingToCar(Staging,Jump,(int)color,false);	//移动至暂存区
//			TakeCount = 0;
//		}
//	}				
//	else if(SysTask.NowTask == TPRough2){
//		if(TakeCount < 3)	SendRingToCar(Rough,Jump,(int)color,false);
//		else{														
//			SendRingToCar(Staging,Jump,(int)color,false);	//移动至暂存区
//			TakeCount = 0;
//		}
//	}	
	MotorMotion_Rotate(Vision,false);		// 旋转电机旋转
}

/**************************************************************************
函数功能：原料区拿取物料动作组
入口参数：NeedColor：所需物料的颜色 
返回  值：无
**************************************************************************/
void TakeRawMaterial(void) 
{
	static char ActionCount;
	MotorMotion_Lift(RawArea,false);		// 升降电机降至原料盘
	ArmClipTakeState(Grasp,true);				// 抓夹夹紧
	ActionCount++;
	MotorMotion_Lift(Reset_L,false);		// 升降电机复位
	MotorMotion_Rotate(Reset_R,true);		// 旋转电机复位
	MotorMotion_Lift(Stuff,false);			// 升降电机下降至托盘
	ArmClipTakeState(Release,true); 		// 抓夹松开
/* 	if(ActionCount >= 3){								
		if(SysTask.NowTask == TakeRaw1)				SendRingToCar(Rough,Jump,SysTask.Code1/100,false);	//第一次去粗加工区
		else if(SysTask.NowTask == TakeRaw2)	SendRingToCar(Rough,Jump,SysTask.Code2/100,false);	//第二次去粗加工区
	}
	if(ActionCount < 3){
		MotorMotion_Rotate(Vision,false);	// 旋转电机旋转
		MotorMotion_Lift(RawArea,true);		// 升降电机降至原料盘
	}
	else	ActionCount = 0;	 */						// 避免拿完后还举起来等待
}

/**************************************************************************
函数功能：蓝牙控制电机运动
入口参数：无
返回  值：无
注意事项：电机进行任何运动之前尽量先复位！！！
零点时抓夹距离地面2125mm
需要下降至距离地面 x mm 时 需要 (2125 - x)/1.67 度
1200mm 575 | 12.75 525 | 1475mm 410  一度下降1.67mm
**************************************************************************/
void MotorMotion_Blue(void)   
{
	char ReceBlueState;																	//收到的蓝牙的数字
	if(U4Set.U4State & 0x8000){													//如果接收完成了消息						
		if(strlen(U4Set.Uart4_RecBuf) == 1)								//数字仅有个位
			ReceBlueState = U4Set.Uart4_RecBuf[0] - 48;
		else if(strlen(U4Set.Uart4_RecBuf) == 2)					//数字有个位和十位
			ReceBlueState = (U4Set.Uart4_RecBuf[0] - 48)*10 + (U4Set.Uart4_RecBuf[1] - 48);
		switch(ReceBlueState){   
			case 0:MotorMotion_Rotate(Reset_R,false);break; // 0 旋转电机复位
			case 1:MotorMotion_Rotate(Vision,false);break;	// 1 旋转电机旋转
			case 2:MotorMotion_Lift(Reset_L,false);break; 	// 2 升降电机复位
			case 3:MotorMotion_Lift(Grab,false);break; 	 		// 3 升降电机降地面抓取
			case 4:MotorMotion_Lift(Stack,false);break; 		// 4 升降电机降码垛
			case 5:MotorMotion_Lift(RawArea,false);break; 	// 5 升降电机降转盘
			case 6:MotorMotion_Lift(Stuff,false);break; 		// 6 升降电机降托盘
			case 7:ArmClipTakeState(Release,false);break; 	// 7 抓夹松开
			case 8:ArmClipTakeState(Grasp,false);break; 		// 8 抓夹抓紧
			case 9:break; // 9 
			case 10:MotorMotion_Lift(Rise,false);break; 		// 10 上升50mm 
			case 11:MotorMotion_Lift(Fall,false);break; 		// 11 下降50mm 
			case 12:ArmClipTakeState(Avoid,false);break; 		// 12 抓夹避撞
			case 13:MotorMotion_Lift(Places,false);break; 	// 13 升降电机降地面放置
			case 14:MotorMotion_Lift(Aim,false);break; 			// 14	升降电机降校准
			case 15:MotorMotion_Lift(Lowest,false);break; 	// 15	升降电机降最低
			case 16:break;
			default:break;
		}
		sprintf(U4Set.Uart4_SenBuf,"Height:%d  %d\r\n",HeightNow,ReceBlueState);
		Usart_SendString(DEBUG_UART4,U4Set.Uart4_SenBuf);     //回送消息
		memset(U4Set.Uart4_RecBuf,0,U4_RecLen*sizeof(char));	//重置接收数组 
		U4Set.U4State = 0;																		//待全部动作执行完后才置零接收下一消息
	}
}

/**************************************************************************
函数功能：升降电机运动
入口参数：Pos：电机位置
返回  值：无
注意事项：电机进行任何运动之前尽量先复位！！！
--高度单位除以100后为cm！！！提高分辨率
--零点时升降电机为 0 度，然后测得抓夹降至地面时为 x 度
--零点时抓夹高度为 H mm，抓夹降至地面时为 h mm
--则可以算得电机旋转 1 度下降 (H - h)/ x mm
--即需要下降至距离地面 Height mm 时 需要 (H - Height)*(H - h)/x 度
--标准物料抓取高度为 550 mm
**************************************************************************/
#define ZeroHeight		1950
#define LowestHeight	300
#define MaxAngle			950
#define Division			1.6842  	//((ZeroHeight-LowestHeight)/MaxAngle)	
u32 HeightNow = ZeroHeight;			//机械臂实时高度
void MotorMotion_Lift(Lifting Pos,bool Wait)   
{
	if(Pos != Places && Pos != Stack){
		switch(Pos){ 
			case Reset_L:HeightNow = ZeroHeight;break; 	//复位（升最高）
			case Grab:   HeightNow = 475; break;				//降标准物件抓取高度（距离地面 525 mm） 
			case RawArea:HeightNow = 1375;break;				//降标准物件抓取转盘高度
			case Stuff:	 HeightNow = 1740;break;				//降标准物件放置托盘高度	
			case Aim:	 	 HeightNow = 575; break;				//降校准
			case Raw:		 HeightNow = 1375;break;				//降原料区识别
			case Pile:	 HeightNow = 875;break;					//降码垛校准 
			case Rise:{
					HeightNow += 50;
					if(HeightNow > ZeroHeight + 25)		HeightNow = ZeroHeight;		//升高50 mm
				}break;				
			case Fall:{
					HeightNow -= 50;
					if(HeightNow < LowestHeight - 25)	HeightNow = LowestHeight;	//降低50 mm
				}break;		
			case Lowest:HeightNow = LowestHeight;break;	//降最低(距离地面 325 mm)
		}
		if(HeightNow <= ZeroHeight && HeightNow >= LowestHeight)
			no_angle_control(speed_id_2,-((ZeroHeight - HeightNow)/Division),2);
	}
	else{
		switch(Pos){
			case Places:HeightNow = 775; break;				//降标准物件抓取高度（距离地面 525 mm）
			case Stack:	HeightNow = 1175;break;				//降标准物件码垛高度（距离地面 1225 mm）
		}
		Tcb(Angle_Control,-((ZeroHeight - HeightNow)/Division),2);
	}
	if(Wait)	delay_ms(150);
}

/**************************************************************************
函数功能：旋转电机运动
入口参数：Pos：电机位置
返回  值：无
注意事项：电机进行任何运动之前尽量先复位！！！
**************************************************************************/
void MotorMotion_Rotate(Rotate Pos,bool Wait)       
{
	switch(Pos){ 
		case Reset_R:Tcb(Angle_Control,0,1);break; 			//复位
		case Vision: Tcb(Angle_Control,-164,1);break;		//旋转
	}
	if(Wait)	delay_ms(250);			
}

/**************************************************************************
函数功能：识别并拿取对应颜色的物料
入口参数：AloneCode：任务码位值
返回  值：无
注意事项：为了跳过刚举臂识别颜色时的第一个物料，则让识别到的颜色需与上一次颜色值不同才能夹取，
如此以来不管第一个物料是什么颜色，都不会被夹取，此外还需要将刚举臂识别到的第一种颜色设置代表
为上一次颜色值变量的初值
调试方案：
//		//颜色抓取
//		LCD_ClearRect(167,120,240,32);
//		LCD_SetAsciiFont(&ASCII_Font32);   	
//		LCD_DisplayString(5,120,"ColorTake:");
//		LCD_DisplayChar(170,120,ColorCode);
**************************************************************************/
void IdentifyColorTake(bool Update,char AloneCode){ 
//	bool UpdateOld = false;
//	ColorSele ColorCode = NoColor;												//当前时刻看到的颜色
//	static ColorSele OldValue = NoColor;									//颜色旧值
	//if(!SysValue.DirectGrab)	
	MaterialPosition((ColorSele)AloneCode,false);				//物料托盘提前旋转		
	SysValue.ReceCarSta = NULL;														//清空避免旧值误判
//	if(Update)	UpdateOld = true;													//第一次抓取更新颜色旧值
 	while(true){
/*		switch(SysValue.ReceCarSta){
			case MaterialRED:		ColorCode = Red_1;	break;
			case MaterialGREEN:	ColorCode = Green_2;break;
			case MaterialBLUE:	ColorCode = Blue_3;	break;
			default:						ColorCode = NoColor;break;
		} */
		TakeRawMaterial();							//直到拿起物料后跳出
		delay_ms(10); 										
		break;		
//		if(Task.DebugFlag){
//			LCD_ClearRect(167,120,240,32);
//			LCD_SetAsciiFont(&ASCII_Font32);   	
//			LCD_DisplayString(5,120,"ColorTake:");
//			LCD_DisplayNumber(170,120,ColorCode,1);  
//		}


/* 		if(ColorCode != NoColor){								//检查数字码是否正确
			if(UpdateOld){												//如果是第一次抓取								
				UpdateOld = false;									//在循环中仅更新一次
				OldValue = ColorCode;								//将第一次看到的颜色设置上一次颜色变量初始值
			}					
			else{
				if(SysValue.DirectGrab){						//若不看颜色直接拿
					if(ColorCode != OldValue){				//颜色与旧值不相同
						OldValue = ColorCode;						//更新颜色旧值
						MaterialPosition((ColorSele)ColorCode,false);	
						delay_ms(800);									//等物料旋转至合适位置
						TakeRawMaterial();							//直到拿起物料后跳出												
						break;		
					}
				}
				else{																//若检查颜色后才抓取
					if(Update){												//抓取第一个物料
						if(ColorCode!=OldValue && ColorCode==AloneCode){
							delay_ms(500);								//颜色与旧值不相同且颜色码与任务码其一位值对应
							delay_ms(200);								//等物料旋转至合适位置
							TakeRawMaterial();						//直到拿起物料后跳出												
							break;		
						}
						OldValue = ColorCode;						//更新颜色，避免第一颜色是所需要颜色而卡死
					}
					else{															//抓取第二、三个物料
						if(ColorCode == AloneCode){			//如果是所需的颜色就直接抓
							delay_ms(700);								//等物料旋转至合适位置								
							TakeRawMaterial();						//直到拿起物料后跳出							
							break;		
						}	
					}	
				}												
			}
		}	 */

	}
}

/**************************************************************************
函数功能：向下位机发送普通指令
入口参数：*CarCommand：指令集
返回  值：无
调试方案：
//		//发送给下位机的数据
//		LCD_ClearRect(135,15,240,32);
//		LCD_SetAsciiFont(&ASCII_Font32);   		
//		LCD_DisplayString(5,15,"SendCar:");
//		LCD_DisplayString(140,15,CarCommand); 
**************************************************************************/
void SendOrderToCar(char note,char *CarCommand,char SendCount){   
	for(char count = 0;count < SendCount;count++){  
		Usart_SendString(USART2,CarCommand);	//发送指令
		delay_ms(40);
	}
//	if(Task.DebugFlag){
//		LCD_SetAsciiFont(&ASCII_Font32);   		
//		//LCD_DisplayString(5,15,"SendCar:");
//		LCD_ClearRect(135,15,240,32);
//		LCD_DisplayString(140,15,CarCommand); 
//		if(note != false)
//			LCD_DisplayNumber(195,15,note,1); 
//	}
//	while(true){
//		Usart_SendString(USART2,CarCommand);	//发送指令
//		delay_ms(100);												
//		if(ReceCarSta == ReceiveMessage){		//确认收到后就不再发送
//			ReceCarSta = NULL;
//			break;
//		}	
//	}
}
/**************************************************************************
函数功能：向下位机发送圆环指令
入口参数：Loca：选择粗加工区或暂存区。State：是否需要等待。AloneCode：任务码位值
返回  值：无
**************************************************************************/
void SendRingToCar(Order Loca,Order State,char AloneCode,bool Init){  
		if(Loca == Rough){						//去粗加工区
			switch(AloneCode){														//根据任务码位值选择发送命令
					case 1:SendOrderToCar(false,GoRoughRED,8);break; 
					case 2:SendOrderToCar(false,GoRoughGREEN,8);break;
					case 3:SendOrderToCar(false,GoRoughBLUE,8);break;
				}
		}
		else if(Loca == Staging){			//去暂存区
			switch(AloneCode){														//根据任务码位值选择发送命令
					case 1:SendOrderToCar(false,GoStoreRED,8);break;
					case 2:SendOrderToCar(false,GoStoreGREEN,8);break;
					case 3:SendOrderToCar(false,GoStoreBLUE,8);break;
				}
		}
		if(Init){
			MotorMotion_Lift(Reset_L,false);							//升降电机复位
			MotorMotion_Rotate(Reset_R,false);						//旋转电机旋转
		}
		if(State == Wait){															//如果需要等待
			while(true){
				delay_ms(10);																//延时等待寄存器值变化
				if(SysValue.ReceCarSta == ArriveLocation){	//底盘到达位置后才跳出
					SysValue.ReceCarSta = NULL;	
					break;
				}		
			}
		}																								//如果不需要等待就直接跳出
}

/**************************************************************************
函数功能：抓夹抓取状态
入口参数：state 物料夹是否抓取
返回  值：无
注意事项：
--舵机 PWM 为 200 HZ
--寄存器最大值 2200
--寄存器最小值 200
**************************************************************************/
void ArmClipTakeState(Order state,bool Wait) 
{
	switch(state){ 
		case Grasp:	 ClipRegister = 1050;break;
		case Release:ClipRegister = 2050;break;
		case Avoid:	 ClipRegister = 1825;break;
		case MAX:		 ClipRegister = 2200;break;
		case MIN:		 ClipRegister = 200;break;
	}
	if(Wait)	delay_ms(450);	
}

/**************************************************************************
函数功能：旋转方向舵机位置
入口参数：state 物料夹是否抓取
返回  值：无
注意事项：已被旋转电机代替
**************************************************************************/
void DirectionState(Rotate Pos) 
{
//	switch(Pos){ 
//		case Reset_R:StuffRegister = 2000;break; 		
//		case Vision: StuffRegister = 1500;break;
//	}
//	delay_ms(400);
}

/**************************************************************************
函数功能：物料盘选择颜色位置
入口参数：color：所需要的颜色
返回  值：无
**************************************************************************/
void MaterialPosition(ColorSele color,bool Wait) 
{
	switch(color){
		case Red_1:  StuffRegister = 615;break;
		case Green_2:StuffRegister = Stuff_INIT;break;
		case Blue_3: StuffRegister = 1500;break;
		case Init: StuffRegister = 400;break;
	}
	if(Wait)	delay_ms(700);
} 

/**************************************************************************
函数功能：视觉获取任务顺序
入口参数: 无
返回  值：任务码顺序
注意事项：strcmp(str1,str2);比较两个字符串是否相同，相同为0，不同为1
					strstr(str1,str2);查找str2在str1中的位置，返回其位置
					\r\n的十六进制是0xOd,0xOa,字符串比较函数strcmp();中不需要写\r\n
					若收到的数据不对则清空接收数组重新接收，否则就保留数组内容
其他方式：
//----------字符串匹配--------------//
//	if(strstr(ReceQRData,"123+123") == 0)		 		return 123123;
//	else if(strstr(ReceQRData,"123+132") == 0)	return 123132;
//	else if(strstr(ReceQRData,"123+213") == 0)	return 123213;
//  .....
//	return false;

//----------字符串比较--------------//
//	if(strcmp(ReceQRData,"123+123") == 0)		 		return 123123;
//	else if(strcmp(ReceQRData,"123+132") == 0)	return 123132;
//	else if(strcmp(ReceQRData,"123+213") == 0)	return 123213;
//  .....
//	return false;

//----------取队列前七位--------------//
//	u8 ReceBuf[8],count;    									//接收缓冲
//	u16 TaskValue;
//	while(rxFIFO.Length < 8); 								//进队数据不足7字节
//	for(char count = 0;count < 7;count++){
//		ReceBuf[count] = fifo_deQueue(); 				//取出前七
//		ReceBuf[count] = ReceBuf[count] - '0';  //字符变为数字
//		if(ReceBuf[count] > 0 && ReceBuf[count]<4)	continue;
//		else	ScreenDisplayPicture(Image_Error_240x280);
//	}
//	TaskValue = ReceBuf[0]*100000+ReceBuf[1]*10000+ReceBuf[2]*1000+ReceBuf[4]*100+ReceBuf[5]*10+ReceBuf[6];
//	return TaskValue;

调试方案：
//				if(Task.DebugFlag){
//					LCD_SetAsciiFont(&ASCII_Font32);   	
//					LCD_ClearRect(87,50,240,32);
//					LCD_DisplayString(5,50,"QR:");
//					LCD_DisplayString(90,50,QRCode.ReceQRData);
//				}
//				if(Task.DebugFlag){
//					LCD_SetAsciiFont(&ASCII_Font32);   	
//					LCD_ClearRect(87,50,240,32);
//					LCD_DisplayString(5,50,"ESP:");
//					LCD_DisplayString(90,50,QRCode.ReceESP8266);
//				}
**************************************************************************/
bool VisualIdentifyTaskCode(void){
	char CodeSpace = sizeof(TaskCodeStr)/sizeof(TaskCodeStr[0]);
	U3Set.U3State = 0;																			//初始化接收标志位
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);						//开启串口3接收中断
	while(true){
		delay_ms(10);
//		if(U3Set.U3State & 0x8000){	
//			U3Set.Usart3_RecBuf[U3Set.U3State & 0X7FFF] = 0;		//添加结束符
			for(char count = 0;count < CodeSpace;count++){ 
				if(strstr(U3Set.Usart3_RecBuf,TaskCodeStr[count]) != 0){
					SendOrderToCar(false,ReceCarSent,2);
					SysTask.CodeS = TaskCodeNum[count];
					if(SysValue.DebugStart == false)								//遍历匹配任务码
						ScreenDisplayTaskCode(SysTask.CodeS);					//屏幕显示任务码
					SysTask.Code1 = SysTask.CodeS / 10;			
					SysTask.Code2 = SysTask.CodeS % 10;
					USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);		//关闭串口接收中断
					return true;
				}	
			}
			memset(U3Set.Usart3_RecBuf,0,U3_RecLen*sizeof(char));	
			U3Set.U3State = 0;																	//若未匹配到则重新接收
		}
	}
//}

/**************************************************************************
函数功能：屏幕显示任务码
入口参数：TaskCode：任务码
返回  值：无
**************************************************************************/
void ScreenDisplayTaskCode(uint32_t TaskCode)
{
	LCD_Clear();
	switch(TaskCode){
		case 11:LCD_DrawImage(120,0,120,280,Image_123_240x280);LCD_DrawImage(0,0,120,280,Image_add123_240x280);break;
		case 12:LCD_DrawImage(120,0,120,280,Image_123_240x280);LCD_DrawImage(0,0,120,280,Image_add132_240x280);break;
		case 13:LCD_DrawImage(120,0,120,280,Image_123_240x280);LCD_DrawImage(0,0,120,280,Image_add213_240x280);break;
		case 21:LCD_DrawImage(120,0,120,280,Image_123_240x280);LCD_DrawImage(0,0,120,280,Image_add231_240x280);break;
		case 22:LCD_DrawImage(120,0,120,280,Image_123_240x280);LCD_DrawImage(0,0,120,280,Image_add312_240x280);break;
		case 23:LCD_DrawImage(120,0,120,280,Image_123_240x280);LCD_DrawImage(0,0,120,280,Image_add321_240x280);break;
		case 31:LCD_DrawImage(120,0,120,280,Image_132_240x280);LCD_DrawImage(0,0,120,280,Image_add123_240x280);break;
		case 32:LCD_DrawImage(120,0,120,280,Image_132_240x280);LCD_DrawImage(0,0,120,280,Image_add132_240x280);break;
		case 33:LCD_DrawImage(120,0,120,280,Image_132_240x280);LCD_DrawImage(0,0,120,280,Image_add213_240x280);break;
	/* 	case 132231:LCD_DrawImage(120,0,120,280,Image_132_240x280);LCD_DrawImage(0,0,120,280,Image_add231_240x280);break;
		case 132312:LCD_DrawImage(120,0,120,280,Image_132_240x280);LCD_DrawImage(0,0,120,280,Image_add312_240x280);break;
		case 132321:LCD_DrawImage(120,0,120,280,Image_132_240x280);LCD_DrawImage(0,0,120,280,Image_add321_240x280);break;
		case 213123:LCD_DrawImage(120,0,120,280,Image_213_240x280);LCD_DrawImage(0,0,120,280,Image_add123_240x280);break;
		case 213132:LCD_DrawImage(120,0,120,280,Image_213_240x280);LCD_DrawImage(0,0,120,280,Image_add132_240x280);break;
		case 213213:LCD_DrawImage(120,0,120,280,Image_213_240x280);LCD_DrawImage(0,0,120,280,Image_add213_240x280);break;
		case 213231:LCD_DrawImage(120,0,120,280,Image_213_240x280);LCD_DrawImage(0,0,120,280,Image_add231_240x280);break;
		case 213312:LCD_DrawImage(120,0,120,280,Image_213_240x280);LCD_DrawImage(0,0,120,280,Image_add312_240x280);break;
		case 213321:LCD_DrawImage(120,0,120,280,Image_213_240x280);LCD_DrawImage(0,0,120,280,Image_add321_240x280);break;
		case 231123:LCD_DrawImage(120,0,120,280,Image_231_240x280);LCD_DrawImage(0,0,120,280,Image_add123_240x280);break;
		case 231132:LCD_DrawImage(120,0,120,280,Image_231_240x280);LCD_DrawImage(0,0,120,280,Image_add132_240x280);break;
		case 231213:LCD_DrawImage(120,0,120,280,Image_231_240x280);LCD_DrawImage(0,0,120,280,Image_add213_240x280);break;
		case 231231:LCD_DrawImage(120,0,120,280,Image_231_240x280);LCD_DrawImage(0,0,120,280,Image_add231_240x280);break;
		case 231312:LCD_DrawImage(120,0,120,280,Image_231_240x280);LCD_DrawImage(0,0,120,280,Image_add312_240x280);break;
		case 231321:LCD_DrawImage(120,0,120,280,Image_231_240x280);LCD_DrawImage(0,0,120,280,Image_add321_240x280);break;
		case 312123:LCD_DrawImage(120,0,120,280,Image_312_240x280);LCD_DrawImage(0,0,120,280,Image_add123_240x280);break;
		case 312132:LCD_DrawImage(120,0,120,280,Image_312_240x280);LCD_DrawImage(0,0,120,280,Image_add132_240x280);break;
		case 312213:LCD_DrawImage(120,0,120,280,Image_312_240x280);LCD_DrawImage(0,0,120,280,Image_add213_240x280);break;
		case 312231:LCD_DrawImage(120,0,120,280,Image_312_240x280);LCD_DrawImage(0,0,120,280,Image_add231_240x280);break;
		case 312312:LCD_DrawImage(120,0,120,280,Image_312_240x280);LCD_DrawImage(0,0,120,280,Image_add312_240x280);break;
		case 312321:LCD_DrawImage(120,0,120,280,Image_312_240x280);LCD_DrawImage(0,0,120,280,Image_add321_240x280);break;
		case 321123:LCD_DrawImage(120,0,120,280,Image_321_240x280);LCD_DrawImage(0,0,120,280,Image_add123_240x280);break;
		case 321132:LCD_DrawImage(120,0,120,280,Image_321_240x280);LCD_DrawImage(0,0,120,280,Image_add132_240x280);break;
		case 321213:LCD_DrawImage(120,0,120,280,Image_321_240x280);LCD_DrawImage(0,0,120,280,Image_add213_240x280);break;
		case 321231:LCD_DrawImage(120,0,120,280,Image_321_240x280);LCD_DrawImage(0,0,120,280,Image_add231_240x280);break;
		case 321312:LCD_DrawImage(120,0,120,280,Image_321_240x280);LCD_DrawImage(0,0,120,280,Image_add312_240x280);break;
		case 321321:LCD_DrawImage(120,0,120,280,Image_321_240x280);LCD_DrawImage(0,0,120,280,Image_add321_240x280);break; */
	}
}

/**************************************************************************
函数功能：屏幕显示图片
入口参数: 任务码
返回  值：无
**************************************************************************/
void ScreenDisplayPicture(const uint8_t *pImage)
{
	LCD_Clear();
	LCD_DrawImage(0,0,240,280,pImage);
}
