#include "sys.h"
/*----------------------------------------------
所用资源一览表：
舵机驱动--Tim1、灯环驱动--Tim2、控制中断---Tim6、电机驱动---Tim8
下位机通信-----Usart2（19200）、二维码通信----Uart3（9600/115200）、蓝牙通信----Uart4（9600）
STM32占用GPIO一览表：
LCD屏幕： PB3、PB4、PB5、PB6、PB7
补光灯IO：PA1、PB8、PB9
电机控制：PA5、PC7、PB13 / PA4、PC6、PB12
舵机脉冲：抓夹--PA8、物料转盘--PA11
Usart2：	PA2--TX    PA3--RX 	（下位机）
Usart3：	PB10--TX   PB11--RX	（二维码）
Uart4： 	PC10-TX    PC11-RX	（蓝牙）
注意事项：发送到串口4的数据需以\r\n（回车）为结尾
-----------------------------------------------
下位机通信表：帧头：0x5E（^）			帧尾：0x5F（_）
-------发送-------------发送-------------发送---
//0x42：去原料区（B）--------------------------- GoRawArea
//0x43：去粗加工区红圆环（C）------------------- GoRoughRED
//0x44：去粗加工区绿圆环（D）------------------- GoRoughGREEN
//0x45：去粗加工区蓝圆环（E）------------------- GoRoughBLUE
//0x4c：去暂存区红圆环（L）--------------------- GoStoreRED
//0x4d：去暂存区绿圆环（M）--------------------- GoStoreGREEN
//0x4e：去暂存区蓝圆环（N）--------------------- GoStoreBLUE
//0x4a：暂存区到粗加工区红圆环（J）------------- StoreGoRough
//0x4b：粗加工到原料区（K）--------------------- RoughGoRaw
//0x46：返回起点（F）--------------------------- RestartS
//0x50：允许底盘进行校准（P）------------------- Calibration
//0x4F：收到底盘发送的消息(O)------------------- ReceCarSent（弃用）
--上位机从一个区到下一个区，直接去任务码位值1对应的圆环到达后，
--双方通信进行色环末端校正,随后上位机根据二维码任务顺序，发送去各圆环处的指令，
--每次到达后，下位机都发  "^@_"  ，上位机接受到后，再发下一个圆环处。
--------接收------------接收-------------接收---
//0x40：底盘已到达指定位置（@）------------------- ArriveLocation
//0x47：下位机确认收到指令（G）------------------- ReceiveMessage（弃用）
//0x48：举臂扫描二维码（H）----------------------- RaiseArmQRCode
//0x49：运行底盘校准（I）------------------------- CarCalibration
//0x72：检测到红色色块（r）----------------------- MaterialRED
//0x67：检测到绿色色块（g）----------------------- MaterialGREEN
//0x62：检测到绿色色块（b）----------------------- MaterialBLUE
注意：取物料完毕，接着还是用原来的指令依次去粗加工区，暂存区
-----------------------------------------------*/

TaskState SysTask; 					//系统任务码控制
Operat_Control SysValue; 		//系统运行参数值

/**************************************************************************
函数功能：系统main函数
入口参数：无
返回  值：无
**************************************************************************/
int main(void)
{	
	//配置系统(是否调试、是否竞赛、是否直接抓、扫描模式)
	SystemInitialize(false,false,false,ScanModule);		//任务码接收模式 ScanModule/ESP8266/DT_06
	while(true){		
		if(SysValue.TaskStart)	OperatingTask();	//任务逻辑  	
		else{
			KeyControlTask();												//按键控制
			SysDetectTask();												//程序调试
			MotorMotion_Blue();											//蓝牙控制
		}
	}	
}

/**************************************************************************
函数功能：系统初始化配置
入口参数：无
返回  值：无
注意事项：屏幕动画高度位置：15、35、56、77、98、119、140、161、182、203、224、245、266
//	Tim6_Init(7199,9999);					//控制中断初始化 72,000,000/(7199+1)/(9999+1)=1 HZ
//	LCD_DisplayString(5,140,"Control Timer Unenble...");
//	Tim2_Init(90,0);							//灯环PWM控制
//	WS2812_DMA_Init();						//灯环DMA控制
//	LCD_DisplayString(5,182,"DMA Unenble...");
**************************************************************************/
void SystemInitialize(bool Debug,bool Race,bool Grab,QR_Rece QRSele){
	SysValue.RaceStart = false;			//开机重置竞赛模式
	SysValue.DebugStart = false;		//开机重置程序检测情况
	SysValue.TaskStart = false;			//开机重置任务运行情况
	SysValue.DirectGrab = false;		//开机重置物料抓取情况
	SysTask.NowTask = TaskStart;		//开机重置任务逻辑
	
	SysValue.DebugStart = Debug;		//设置开机是否进入程序检测
	SysValue.RaceStart = Race;			//开机重置竞赛模式
	SysValue.DirectGrab = Grab;			//开机设置抓取模式 
	SysValue.QR_Mode = QRSele;			//设置获取二维码方式为扫描模块或者ESP8266
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();	    			
	
	SPI_LCD_Init();				
	delay_ms(100);			
	LCD_Clear();
	LCD_SetColor(LCD_YELLOW);
	LCD_SetAsciiFont(&ASCII_Font16); 
	LCD_DisplayString(5,15,"Copyright (C)  @2023");
	LCD_SetColor(LCD_BLUE);
	LCD_DisplayString(5,35,"STM32F103RCT6  72Mhz");
	LCD_DisplayString(5,56,"FLASH:256 KB  SRAM:48 KB");
	LCD_SetColor(LCD_YELLOW);
	LCD_DisplayString(5,77,"Initialize System:"); 
	delay_ms(100);	
	
	Laser_Init();			
	KEY_Init();						
	LCD_SetColor(LCD_WHITE);
	LCD_DisplayString(5,98,"KEY/LED GPIO Success..."); 
	delay_ms(100);				

	Motor_Gpio_Enable();		
	Tim8_Init(65535,72);		
	LCD_DisplayString(5,119,"Motor GPIO/Timer Success..."); 
	delay_ms(100);					
	
	Tim1_Init(2499,71); 								//舵机PWM 36,000,000 /(2499+1)/(71+1)= 200 HZ
	if(Race)
		TIM3_Init(7199,499);							//72,000,000/(7199+1)/(499+1) = 2 HZ
	LCD_DisplayString(5,140,"Servo/Timer Success...");
	delay_ms(100);			
	
	USART_Config_2(Bound_19200);				//下位机通信
	UART_Config_4(Bound_9600); 					//蓝牙通信
	if(SysValue.QR_Mode == ScanModule){
		USART_Config_3(Bound_9600); 			//扫描模块通信
		LCD_DisplayString(5,161,"Usart 2/3/4 Success...");
		delay_ms(100);
		if(SysValue.DebugStart == false)
			ScreenDisplayPicture(Image_Hello_240x280);
	}
	else if(SysValue.QR_Mode == ESP8266){
		TIM7_Init(1000 - 1,7200 - 1);			//10ms中断串口接收
		USART_Config_3(Bound_115200); 		//WiFi模块通信
		LCD_DisplayString(5,161,"Usart 2/3/4 Success...");
		delay_ms(100);
		if(ConfigurationEsp8266()){
			if(SysValue.DebugStart == false)
				ScreenDisplayPicture(Image_Hello_240x280);
		}
		else{
			if(SysValue.DebugStart == false)
				ScreenDisplayPicture(Image_Error_240x280);
		}
	}
	else if(SysValue.QR_Mode == DT_06){
		TIM7_Init(1000 - 1,7200 - 1);			//10ms中断串口接收
		USART_Config_3(Bound_115200); 		//WiFi模块通信
		LCD_DisplayString(5,161,"Usart 2/3/4 Success...");
		delay_ms(100);
		if(SysValue.DebugStart == false) 
			ScreenDisplayPicture(Image_Hello_240x280);
	}
	MaterialPosition(Init,false);
}	

/**************************************************************************
函数功能：按键控制
入口参数：无
返回  值：无
**************************************************************************/
void KeyControlTask(void){
	char KeyValue = KEY_Scan();													//获取按键值 
	switch(KeyValue){
		case KeyShort:SysValue.TaskStart = true;break;		//任务开始运行
		case KeyLong:	SysValue.DebugStart = true;break;		//程序检测开始运行
		case KeySlow:break;
		default:ScreenDisplayPicture(Image_Error_240x280);break; 
	}
}

/**************************************************************************
函数功能：任务逻辑
入口参数：无
返回  值：无
**************************************************************************/
void OperatingTask(void){      
	switch(SysTask.NowTask){
		case TaskStart:{	
				Laser = false;																//关闭激光
				TIM_Cmd(TIM3,DISABLE);  											//关闭TIM3 
				if(SysValue.DebugStart == false)									
					ScreenDisplayPicture(Image_Start_240x280);
				SendOrderToCar(false,GoRawAreaS,2);
				SysTask.NowTask = ScanQR;	
			}break;														
		case ScanQR:{			
	//			VisualIdentifyTaskCode();											//获取任务码
				SysTask.Code1 = 1;			
				SysTask.Code2 = 3;			
				ArmClipTakeState(Avoid,false);								//抓夹松开	
				MotorMotion_Rotate(Vision,false);							//旋转电机转向
				MotorMotion_Lift(Raw,false);									//升降电机降原料区识别			
				SysTask.NowTask = TakeRaw1;
			}break;
		case TakeRaw1:{
				if(SysValue.ReceCarSta == ArriveLocation){	
					SysValue.ReceCarSta = NULL;
					IdentifyColorTake(true,SysTask.Code1);							//识别原料区物料
//					IdentifyColorTake(false,SysTask.Code1/10%10);					
//					IdentifyColorTake(false,SysTask.Code1%10); 		
					
					delay_ms(1000);																					//延时后机械臂旋出
					delay_ms(1000);		
					delay_ms(1000);		
					MotorMotion_Rotate(Vision,false);												//旋转电机旋转（准备校准）
					ArmClipTakeState(Avoid,false);													//抓夹稍微夹紧避免碰撞
				//	MotorMotion_Lift(Aim,false);														//降校准
					MaterialPosition((ColorSele)(SysTask.Code1),false);	//车上盘提前旋转至所需
					SysTask.OldTask = SysTask.NowTask;
					SysTask.NowTask = Calibrat;	
				}					
			}break;
		case GoStaging:{
			if(SysValue.ReceCarSta == ArriveLocation){				
					SendRingToCar(Staging,Wait,SysTask.Code1,true);
					SysTask.NowTask = PutStag1;
			}
			}break;	
		case PutStag1:{										
				Ground_PutAction(Place,(ColorSele)(SysTask.Code1),false);		//暂存区放置物料
//				Ground_PutAction(Place,(ColorSele)(SysTask.Code1%10),true);	
//				Ground_PutAction(Place,(ColorSele)(SysTask.Code2/100),false);			//结束后去拿物料
				MotorMotion_Lift(Aim,false);																			//降校准 
				SysTask.OldTask = SysTask.NowTask;
				SysTask.NowTask = Calibrat;												
			}break;	
		case PassRough:{														
					SendOrderToCar(false,RoughGoRaw,5);													//暂存区返回原料区	
					MotorMotion_Lift(Raw,false);																//升降电机降原料区
					SysTask.NowTask = TakeRaw2;							
			}break;	
		case TakeRaw2:{
				if(SysValue.ReceCarSta == ArriveLocation){	
					SysValue.ReceCarSta = NULL;	
					IdentifyColorTake(true,SysTask.Code2);									//识别原料区物料
//					IdentifyColorTake(false,SysTask.Code2/10%10);
//					IdentifyColorTake(false,SysTask.Code2%10);			
	
					delay_ms(1000);																							//延时后机械臂旋出
					delay_ms(1000);
					delay_ms(1000);
					MotorMotion_Rotate(Vision,false);														//旋转电机旋转（准备校准）
					ArmClipTakeState(Avoid,false);															//抓夹稍微夹紧避免碰撞
				//	MotorMotion_Lift(Aim,false);																//降校准
					MaterialPosition((ColorSele)(SysTask.Code2),false);			//车上盘提前旋转至所需
					SysTask.OldTask = SysTask.NowTask;
					SysTask.NowTask = Calibrat;		
				}					
			}break;			
		case GoStag2:{				
				if(SysValue.ReceCarSta == ArriveLocation){			
					SysValue.ReceCarSta = NULL;				
					SendRingToCar(Staging,Wait,SysTask.Code2,true);
					SysTask.NowTask = PutStag2;						
				}					
			}break;				
		case PutStag2:{		 		
				Ground_PutAction(Place,(ColorSele)(SysTask.Code2),false);//暂存区码垛	
//				Ground_PutAction(StackUp,(ColorSele)(SysTask.Code2%10),true);
//				Ground_PutAction(StackUp,NoColor,true);		
				SysTask.NowTask = Restart;											
			}break;  				
		case Restart:{				
				if(SysValue.ReceCarSta == ArriveLocation){			
					SysValue.ReceCarSta = NULL;
					ScreenDisplayPicture(Image_Hello_240x280);						
					SysTask.NowTask = TaskStart;	
					SysValue.TaskStart = false;
				}					
			}break;
		case Calibrat:{	 																											//色环校准 
					switch(SysTask.OldTask){
							case TakeRaw1:SysTask.NowTask = PassRough;break;					
							case PutStag1:SysTask.NowTask = GoStag2;break;		
							case TakeRaw2:SysTask.NowTask = GoStaging;break;					
							default:ScreenDisplayPicture(Image_Error_240x280);break; 						
					}
			}break;
		default:ScreenDisplayPicture(Image_Error_240x280);break; 
	}
}

/**************************************************************************
函数功能：系统调试程序
入口参数：无
返回  值：无
注意事项：若需要退出调试则请复位单片机
**************************************************************************/
void SysDetectTask(){
	if(SysValue.DebugStart){
		SysValue.DebugStart = false;					
		Board_LED_Init();											//指示灯
		Tim6_Init(7199,999);									//100ms
		LCD_Clear();													//清屏进入程序检测
		LCD_SetAsciiFont(&ASCII_Font32);
		LCD_DisplayString(5,15,"SendCar:");		//给下位机发送的指令
		LCD_DisplayString(5,50,"QRcode:");		//收到二维码的数据
		LCD_DisplayString(5,85,"ReceCar:");	  //收到下位机的数据
		LCD_DisplayString(5,155,"Now:");			//当前状态
		LCD_DisplayString(5,190,"Task:");			//本次任务码
		LCD_DisplayString(5,225,"Blue:");			//蓝牙指令
		LCD_DisplayString(120,240,"*Debug*");	//显示当前为Debug模式
	}
}		

/**************************************************************************
函数功能：定时器3中断服务程序（备用控制）
入口参数：无
返回  值：无
**************************************************************************/
void TIM3_IRQHandler(void)
{ 	
	static unsigned int count = 0;
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)	//更新中断
	{	 		
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
		if(SysValue.TaskStart)
			TIM_Cmd(TIM3,DISABLE);  											//关闭TIM3 
		else{
			count++;
			if(count >= 600){
				count = 0;
				SysValue.TaskStart = true;									//超时强制开始任务
				TIM_Cmd(TIM3,DISABLE);  										//关闭TIM3
			}
		}
	}	    
}

/**************************************************************************
函数功能：定时器6中断服务函数（总控制）
入口参数：无
返回  值：无
注意事项：
**************************************************************************/
void TIM6_IRQHandler(void)   
{
	if(TIM_GetITStatus(TIM6,TIM_IT_Update) != RESET)  
	{
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update);			
		LED0 =~ LED0;	
		LCD_SetAsciiFont(&ASCII_Font32);
		//当前状态
		LCD_ClearRect(70,155,240,32);  	
		LCD_DisplayString(75,155,TaskString[SysTask.NowTask]); 
		//本次任务码
		LCD_ClearRect(87,190,240,32);  	
		LCD_DisplayNumber(90,190,SysTask.CodeS,6);
		//收到下位机的数据
		LCD_ClearRect(135,85,240,32);
		LCD_DisplayChar(140,85,SysValue.ReceCarSta);
		//收到二维码的数据
		LCD_ClearRect(87,50,240,32);
		LCD_DisplayString(90,50,U3Set.Usart3_RecBuf); 
	}
}

/**************************************************************************
函数功能：定时器7中断服务程序（串口接收控制）
入口参数：无
返回  值：无
**************************************************************************/
void TIM7_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)	//更新中断
	{	 			   
		U3Set.U3State |= 1<<15;														//标记接收完成
		TIM_ClearITPendingBit(TIM7,TIM_IT_Update);  			//清除TIM7更新中断标志    
		TIM_Cmd(TIM7,DISABLE);  													//关闭TIM7 
	}	    
}

/**************************************************************************
函数功能：定时器8中断服务函数（电机脉冲控制）
入口参数：无
返回  值：无
**************************************************************************/
void TIM8_UP_IRQHandler(void)  
{ 
	static u32 step = 0;
  if(TIM_GetITStatus(TIM8,TIM_IT_Update) != RESET)	
  { 
		if(id_motor == 1){ 
			step++;  
			if(step >= motor.step_max){ //   200*motor.Subdivision__coefficient ( step ) == 1 ( circle )
				step = 0;
				motor.sign_1_ok_step = 1; //一步完成
				TIM_Cmd(TIM8, DISABLE); 	//失能 TIM8
			}
		}
		else{
			motor.sign_1_ok_step = 1; 	//一步完成
		  TIM_Cmd(TIM8, DISABLE); 		// 失能 TIM8
		}
		TIM_ClearITPendingBit(TIM8, TIM_IT_Update); // 清除溢出中断标志位
	}
}
