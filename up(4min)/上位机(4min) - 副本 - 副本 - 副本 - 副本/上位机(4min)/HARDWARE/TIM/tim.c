#include "tim.h"

/**************************************************************************
函数功能：TIM1舵机PWM初始化
入口参数：arr，psc
返回  值：无
注意事项：CH1--PA8、CH2--PA9、CH3--PA10、CH4--PA11

	//TIM_OC2Init(TIM1,&TIM_OCInitStruct);                       		//初始化输出比较参数
	//TIM_OC3Init(TIM1,&TIM_OCInitStruct);                       		//初始化输出比较参数
	//TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);   						//CH2使能预装载寄存器
	//TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);   						//CH3使能预装载寄存器
	//TIM1->CCR2 = Stuff_INIT;																			//物料盘舵机初始位置 
	//TIM1->CCR3 = Stage_INIT;																			//舵机初始位置 
**************************************************************************/
void Tim1_Init(u16 arr,u16 psc){		 	
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
  GPIO_InitTypeDef GPIO_InitStruct;                             
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);   				//使能定时器1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);  				//使能GPIOA的时钟
					
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11; //PA8、PA9、PA10、PA11
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;          				//复用推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	TIM_TimeBaseInitStruct.TIM_Period = arr-1;              			 //设定计数器自动重装值 
	TIM_TimeBaseInitStruct.TIM_Prescaler  = psc-1;          			 //设定预分频器
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;	 //TIM向上计数模式
	TIM_TimeBaseInitStruct.TIM_ClockDivision = Encoder_TIM_Psc;    //设置时钟分割  
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = CNT_Init; 	  	 //重复计数(高级定时器特有)    
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);       				 //初始化定时器
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;            	   //选择PWM1模式
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; 		 //指定TIM输出比较状态\极性(正向通道)
	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;  //指定TIM输出比较状态\极性(互补通道)
	TIM_OCInitStruct.TIM_Pulse = CNT_Init;                     		 //指定要加载到捕获比较寄存器中的脉冲值
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;     		 //指定输出极性
	TIM_OCInitStruct.TIM_OCNPolarity  = TIM_OCNPolarity_High;   	 //指定互补通道输出极性
	TIM_OCInitStruct.TIM_OCIdleState  = TIM_OCIdleState_Set;       //指定空闲状态下正向通道TIM输出比较引脚的状态
  TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCIdleState_Reset;  	 //指定空闲状态下互补通道TIM输出比较引脚的状态
	TIM_OC1Init(TIM1,&TIM_OCInitStruct);                       		 //初始化输出比较参数
	TIM_OC4Init(TIM1,&TIM_OCInitStruct);                       		 //初始化输出比较参数
	
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);   							//CH1使能预装载寄存器
	TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);  							//CH4使能预装载寄存器
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);  								 							//高级定时器输出需要设置这句
	TIM_ARRPreloadConfig(TIM1,ENABLE);                 							//使能TIM1在ARR上的预装载寄存器（默认允许）
	TIM1->CCR1 = Stage_INIT;																				//抓夹舵机初始位置 
	TIM1->CCR4 = Stuff_INIT;													 							//物料盘舵机初始位置
	
	TIM_Cmd(TIM1,ENABLE);                              							//使能定时器1
} 

/**************************************************************************
函数功能：TIM2灯环PWM初始化
入口参数：arr：自动重装值
返回  值：无
注意事项：定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
					Ft=定时器工作频率,单位:Mhz
TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);  						//使能TIM2在CCR1上的预装载寄存器
**************************************************************************/
void Tim2_Init(u16 arr,u16 psc){
	GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);					//使能PORTA时钟	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);						//使能TIM2时钟        
					
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;											//GPIO
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;							//速度50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;								//复用推挽输出                               
	GPIO_Init(GPIOA,&GPIO_InitStructure);													//根据指定的参数初始化GPIO口
	
	TIM_TimeBaseStructure.TIM_Prescaler = psc;										//定时器分频：(0+1)=1,不分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;			//向上计数模式
	TIM_TimeBaseStructure.TIM_Period = arr;												//自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;					//时钟分割
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);								//根据指定的参数初始化TIM2
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 						//选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	//TIM_OCInitStructure.TIM_Pulse = 0;													//待装入捕获比较寄存器的脉冲值(此程序不用加初值)
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 		//输出极性:TIM输出比较极性高
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  										//根据指定的参数初始化外设TIM2 Channel2
	
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);  						//使能TIM2在CCR2上的预装载寄存器
	
	TIM_Cmd(TIM2, DISABLE);  																			//失能TIM2，防止第一个脉冲异常
	TIM_DMACmd(TIM2, TIM_DMA_CC2, ENABLE);												//使能TIM2_CH2的DMA功能(CC2对应通道2)
}

/**************************************************************************
函数功能：通用定时器3初始化
入口参数：无
返回  值：无
注意事项：溢出时间(ms)=((arr+1)*(psc+1))/1000/72
**************************************************************************/
void TIM3_Init(u16 arr,u16 psc){
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
 
	TIM_TimeBaseStructure.TIM_Period = arr;								 				//自动重装载周期值	
	TIM_TimeBaseStructure.TIM_Prescaler = psc; 										//预分频值  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 									//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//TIM向上计数模式
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure); 								//根据指定的参数初始化TIMx
 
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_Trigger,ENABLE);				//使能更新触发TIM中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  							//TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  		//先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  					//从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 							//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  															//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
 
	TIM_Cmd(TIM3, ENABLE); 																			  //使能TIMx外设							
}

/**************************************************************************
函数功能：基本定时器6初始化
入口参数：无
返回  值：无
注意事项：溢出时间(ms)=((arr+1)*(psc+1))/1000/72
**************************************************************************/
void Tim6_Init(u16 arr,u16 psc){
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); 					//时钟使能
	
	TIM_TimeBaseStructure.TIM_Period = arr; 											//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 										//设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 			//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//TIM向上计数模式
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); 							//根据指定的参数初始化TIMx的时间基数单位
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE ); 										//使能指定的TIM6中断,允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  							//TIM6中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  		//先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  					//从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure); 															//初始化NVIC寄存器
	
	TIM_Cmd(TIM6, ENABLE);  																			//使能TIMx		 
}

/**************************************************************************
函数功能：通用定时器7中断初始化
入口参数：arr：自动重装值、psc：时钟预分频数
返回  值：无
注意事项：溢出时间(ms)=((arr+1)*(psc+1))/1000/72
**************************************************************************/
void TIM7_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);					//TIM7时钟使能    
	
	TIM_TimeBaseStructure.TIM_Period = arr; 											//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler = psc; 										//设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;			 	//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//TIM向上计数模式
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseStructure); 								//根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE); 											//使能指定的TIM7中断,允许更新中断
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ;		//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;						//子优先级4
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);																//根据指定的参数初始化VIC寄存器
	
	TIM_Cmd(TIM7,DISABLE);																				//关闭定时器7
}

/**************************************************************************
函数功能：高级定时器8初始化
入口参数：无
返回  值：无
注意事项：溢出时间(ms)=((arr+1)*(psc+1))/1000/72
**************************************************************************/
void Tim8_Init(u32 arr,u32 psc){  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	NVIC_InitTypeDef  NVIC_Initstruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);	 				//使能 TIM 的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE); 				//使能 GPIO 外设时钟使能
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; //配置 使能ENA 输出引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 							//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);
	
	TIM_TimeBaseStructure.TIM_Period = arr - 1; 									//设置自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc - 1; 								//设置用来作为 TIMx 时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 									//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//TIM 向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = motor.single_pause_NUM-1; //输出的脉冲周期数 cnt+1
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); 							//初始化 TIMx 的时间基数单位

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 						//选择定时器模式:PWM1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 2-1; 													//设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 		//输出极性:TIM 输出比较极性高
	
	TIM_OC1Init(TIM8, &TIM_OCInitStructure); 											//根据 TIM_OCInitStruct 的参数初始化外设 TIMx
	TIM_OC2Init(TIM8, &TIM_OCInitStructure); 
	TIM_OC3Init(TIM8, &TIM_OCInitStructure); 
	TIM_OC4Init(TIM8, &TIM_OCInitStructure); 
	
	TIM_CtrlPWMOutputs(TIM8,ENABLE); 															//MOE 主输出使能
	 
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable); 						//CH1 预装载使能
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable); 						//CH2 预装载使能
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable); 						//CH3 预装载使能
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable); 						//CH4 预装载使能
	TIM_ARRPreloadConfig(TIM8, ENABLE); 													//使能 TIMx 在 ARR 上的预装载寄存器
							
	TIM_ClearITPendingBit(TIM8,TIM_IT_Update);										//清除溢出中断标志位
  TIM_ITConfig(TIM8,TIM_IT_Update,ENABLE );											//使能定时器更新中断

  NVIC_Initstruct.NVIC_IRQChannel = TIM8_UP_IRQn;
  NVIC_Initstruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_Initstruct.NVIC_IRQChannelSubPriority = 2;
  NVIC_Initstruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_Initstruct);
	
	TIM_Cmd(TIM8, DISABLE); 																			 //失能 TIM
}

