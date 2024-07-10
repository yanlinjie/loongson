#include "TIM.H"

/*  TIM8 -- 可控数脉冲输出
	入口参数： arr 计数值 ; psc 分频值
*/
void TIME_8_PWM_Init(u16 arr,u16 psc) 
{  //Tout=(ARR+1)(PSC+1)/Tclk（ARR为自动重装载值，PSC为预分频系数）
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_OCInitTypeDef TIM_OCInitStructure;
		NVIC_InitTypeDef    NVIC_Initstruct;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); // 使能 TIM 的时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE); //使能 GPIO 外设时钟使能
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; //配置  输出引脚
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);
	
		TIM_TimeBaseStructure.TIM_Period = arr-1 ; //设置自动重装载寄存器周期的值
		TIM_TimeBaseStructure.TIM_Prescaler = psc-1 ; //设置用来作为 TIMx 时钟频率除数的预分频值
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM 向上计数模式
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 1-1; //输出的脉冲数 single_pause_NUM
		TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //初始化 TIMx 的时间基数单位
   
	 
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:PWM1
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
		TIM_OCInitStructure.TIM_Pulse = 2-1;                            //设置待装入捕获比较寄存器的脉冲值
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM 输出比较极性高
		
		TIM_OC1Init(TIM8, &TIM_OCInitStructure); //根据 TIM_OCInitStruct 的参数初始化外设 TIMx
		TIM_OC2Init(TIM8, &TIM_OCInitStructure); 
		TIM_OC3Init(TIM8, &TIM_OCInitStructure); 
		TIM_OC4Init(TIM8, &TIM_OCInitStructure); 
		
		 TIM_CtrlPWMOutputs(TIM8,ENABLE); //MOE 主输出使能
		 
		TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable); //CH1 预装载使能
		TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable); //CH2 预装载使能
		TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable); //CH3 预装载使能
		TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable); //CH4 预装载使能
		TIM_ARRPreloadConfig(TIM8, ENABLE); //使能 TIMx 在 ARR 上的预装载寄存器
		
		TIM_ClearITPendingBit(TIM8,TIM_IT_Update);//清除溢出中断标志位
    TIM_ITConfig(TIM8,TIM_IT_Update,ENABLE );//使能定时器更新中断

    NVIC_Initstruct.NVIC_IRQChannel = TIM8_UP_IRQn;
    NVIC_Initstruct.NVIC_IRQChannelPreemptionPriority = TIM8_PP;
    NVIC_Initstruct.NVIC_IRQChannelSubPriority = TIM8_SubP;
    NVIC_Initstruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_Initstruct);
		close_TIM8
}
	void GPIO_motor_EN_and_dir(void) 
{  
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); //使能 GPIOB 外设时钟使能
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE); //使能 GPIO 外设时钟使能

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 );  
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  |GPIO_Pin_13 | GPIO_Pin_15 | GPIO_Pin_12; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	 GPIO_SetBits(GPIOB, GPIO_Pin_0  |GPIO_Pin_13 | GPIO_Pin_15 | GPIO_Pin_12 ); 
}
/*  TIM6 -- 定时器计时
   入口参数： arr 计数值 ; psc 分频值
*/
void TIMER6_Init(u16 arr , u16 psc)  //  Tout= ( (arr+1) * (psc+1) )/Tclk；	
{                                    
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;   //TIM结构体类型
	NVIC_InitTypeDef         NVIC_Initstruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //时钟使能
	
	TIM_TimeBaseStructure.TIM_Period = arr-1 ;                     //  计数值
	TIM_TimeBaseStructure.TIM_Prescaler  = psc-1 ;                 // 设置分频系数
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;      // 时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseStructure);              //初始化TIM6寄存器
	
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);                 //允许更新中断
	
	NVIC_Initstruct.NVIC_IRQChannel =	TIM6_IRQn ;            // TIM6 中断             
	NVIC_Initstruct.NVIC_IRQChannelPreemptionPriority = TIM6_PP;   //抢占优先级
	NVIC_Initstruct.NVIC_IRQChannelSubPriority = TIM6_SubP;       //子优先级
	NVIC_Initstruct.NVIC_IRQChannelCmd = ENABLE;           //使能中断通道
	NVIC_Init ( &NVIC_Initstruct);                       //初始化NVIC寄存器

  TIM_Cmd(TIM6,ENABLE);
}
float Mode_array(float *a)
{
	int   b[100];
	float	c[100];
	int i,j,N;
	int NUM,FLAG;
	N = sizeof(a);
	for (i = 0; i < N; i++)
	{
		for (j = i; j < N; j++)
			if (a[i] == a[j])
				b[i]++;				//嵌套循环，数组b记录数组a中每个元素与其及其后面的元素的重复次数
	}
	NUM = b[0];
	for (i = 0; i < N; i++)
	{	
		if (NUM <= b[i])
		{
			NUM = b[i];
			FLAG = i;
		}
	}										//寻找数组b的最大值，即众数的次数，位置记录在FLAG
	for (i = 0; i < N; i++)
		c[i] = 0;
	c[FLAG] = a[FLAG];
	for (i = 0; i < N; i++)
	{
		if (NUM == b[i])
			c[i] = a[i];				//如果有多个次数与众数字数相同的元素，记录在数组c中
	}
	for (i = 0; i < N; i++)
		if (c[i]!=0) return c[i]; //输出众数	
	return NULL;
}

