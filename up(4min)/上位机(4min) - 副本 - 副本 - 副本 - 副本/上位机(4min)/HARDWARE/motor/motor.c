#include "motor.h"
 
char id_motor = 0;			//电机ID

motor_set motor = {
	matlab_Subdivision_coefficient,1,200*matlab_Subdivision_coefficient,0,0,0
}; // 定义电机结构体变量

//分频值：2 ,细分系数: 8  
//最大角速度为377.627452  度/S
//最小角速度为132.878205  度/S
//总阶段运行总时长为1.250195 s
const unsigned int sj_TIM_ARR[400] = {
    60958,    60726,    60496,    60266,    60038,    59811,    59585,    59360,    59136,    58913,
    58691,    58470,    58251,    58032,    57815,    57599,    57383,    57169,    56956,    56744,
    56533,    56323,    56114,    55906,    55699,    55493,    55288,    55084,    54881,    54679,
    54478,    54279,    54080,    53882,    53685,    53489,    53294,    53100,    52907,    52715,
    52524,    52333,    52144,    51956,    51769,    51582,    51397,    51212,    51029,    50846,
    50664,    50483,    50303,    50124,    49946,    49769,    49593,    49417,    49242,    49069,
    48896,    48724,    48553,    48382,    48213,    48045,    47877,    47710,    47544,    47379,
    47214,    47051,    46888,    46726,    46565,    46405,    46246,    46087,    45929,    45772,
    45616,    45461,    45306,    45152,    44999,    44847,    44696,    44545,    44395,    44246,
    44097,    43950,    43803,    43657,    43511,    43367,    43223,    43079,    42937,    42795,
    42654,    42514,    42374,    42235,    42097,    41960,    41823,    41687,    41552,    41417,
    41283,    41150,    41017,    40886,    40754,    40624,    40494,    40365,    40236,    40108,
    39981,    39855,    39729,    39603,    39479,    39355,    39232,    39109,    38987,    38865,
    38745,    38624,    38505,    38386,    38268,    38150,    38033,    37916,    37801,    37685,
    37571,    37457,    37343,    37230,    37118,    37006,    36895,    36784,    36674,    36565,
    36456,    36348,    36240,    36133,    36026,    35920,    35815,    35710,    35606,    35502,
    35398,    35296,    35193,    35092,    34991,    34890,    34790,    34690,    34591,    34493,
    34395,    34297,    34200,    34103,    34008,    33912,    33817,    33723,    33629,    33535,
    33442,    33349,    33257,    33166,    33075,    32984,    32894,    32804,    32715,    32627,
    32538,    32451,    32363,    32276,    32190,    32104,    32018,    31933,    31849,    31765,
    31681,    31598,    31515,    31433,    31351,    31269,    31188,    31107,    31027,    30947,
    30868,    30789,    30710,    30632,    30554,    30477,    30400,    30324,    30248,    30172,
    30097,    30022,    29947,    29873,    29799,    29726,    29653,    29581,    29508,    29437,
    29365,    29294,    29224,    29153,    29083,    29014,    28945,    28876,    28808,    28740,
    28672,    28604,    28538,    28471,    28405,    28339,    28273,    28208,    28143,    28079,
    28014,    27950,    27887,    27824,    27761,    27699,    27636,    27575,    27513,    27452,
    27391,    27330,    27270,    27210,    27151,    27092,    27033,    26974,    26916,    26858,
    26800,    26743,    26686,    26629,    26572,    26516,    26460,    26405,    26350,    26295,
    26240,    26186,    26132,    26078,    26024,    25971,    25918,    25865,    25813,    25761,
    25709,    25658,    25606,    25555,    25505,    25454,    25404,    25354,    25305,    25255,
    25206,    25157,    25109,    25060,    25012,    24964,    24917,    24870,    24823,    24776,
    24729,    24683,    24637,    24591,    24546,    24500,    24455,    24410,    24366,    24322,
    24277,    24234,    24190,    24147,    24103,    24061,    24018,    23975,    23933,    23891,
    23849,    23808,    23767,    23725,    23685,    23644,    23603,    23563,    23523,    23483,
    23444,    23404,    23365,    23326,    23288,    23249,    23211,    23173,    23135,    23097,
    23059,    23022,    22985,    22948,    22911,    22875,    22839,    22803,    22767,    22731,
    22695,    22660,    22625,    22590,    22555,    22521,    22486,    22452,    22418,    22384,
    22350,    22317,    22284,    22251,    22218,    22185,    22152,    22120,    22088,    22056,
    22024,    21992,    21961,    21929,    21898,    21867,    21836,    21805,    21775,    21744,
    21714,    21684,    21654,    21625,    21595,    21566,    21536,    21507,    21478,    2000
};

 unsigned int z_TIM_ARR[88] = {
    58113,    56655,    55241,    53869,    52537,    51245,    49992,    48776,    47597,    46454,
    45345,    44270,    43228,    42218,    41238,    40289,    39369,    38478,    37614,    36777,
    35966,    35181,    34419,    33682,    32968,    32276,    31607,    30958,    30330,    29721,
    29132,    28562,    28009,    27475,    26957,    26456,    25971,    25501,    25047,    24607,
    24181,    23769,    23370,    22984,    22611,    22249,    21900,    21561,    21234,    20917,
    20610,    20313,    20026,    19749,    19480,    19220,    18969,    18725,    18490,    18262,
    18042,    17829,    17623,    17423,    17231,    17044,    16864,    16689,    16520,    16357,
    16199,    16047,    15899,    15756,    15618,    15484,    15355,    15230,    15109,    14992,
    14879,    14770,    14664,    14562,    14463,    14367,    14275,    14185
};
 unsigned int  length = sizeof(z_TIM_ARR) / sizeof(z_TIM_ARR[0]);
 unsigned int  sj_length = sizeof(sj_TIM_ARR) / sizeof(sj_TIM_ARR[0]);

/**************************************************************************
函数功能：电机脱机IO与方向IO初始化
入口参数：无
返回  值：无
**************************************************************************/
void Motor_Gpio_Enable(void){  
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); 		//使能 GPIOB 外设时钟使能
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); 		//使能 GPIOA 外设时钟使能

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			 	//推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA,&GPIO_InitStructure);
		GPIO_ResetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_5);  
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			 	//推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB,&GPIO_InitStructure);
		GPIO_SetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13); 	
}

/**************************************************************************
函数功能：以函数指针为参数的函数,
入口参数：无
返回  值：无
注意事项：可以调用 所有形参类型且顺序相同，且返回值类型相同的函数
定义一个函数指针变量hook_1，该函数指针指向一个返回类型为void、参数类型分别为float和char的函数。
Tcb函数接受这个函数指针作为参数，并在调用时将t1和id传递给该函数。
**************************************************************************/
void (*hook_1)(float,char);		 // 定义函数指针变量hook_1
void Tcb(void(*hook_1)(float,char),float t1,char id)
{
	hook_1(t1,id);
}

/**************************************************************************
函数功能：S型加减速 ,( 包括所有运动情况 )
入口参数：无
返回  值：无
**************************************************************************/
void S_go( unsigned int id , u32 pulse_num){
	long i;  
	motor.step_max = 1;       // 单步走
	if( id == 1 )  {
				if(pulse_num > 2*length||pulse_num == 2*length){ // x >= 2n
				for(i = 0;i < length;i++){  										/* 升 */
					TIM8->ARR = z_TIM_ARR[i];				// 装载计数值
					TIM_Cmd(TIM8,ENABLE);						// 使能TIM8
					while(!motor.sign_1_ok_step); 	// 等待步数完成
					motor.sign_1_ok_step = 0;				// 重置标志
				}
				for(i = 0;i < (pulse_num-2*length);i++){ 	/* 匀速 */
					TIM_Cmd(TIM8,ENABLE);	  				// 使能 TIM8
					while(!motor.sign_1_ok_step); 	// 等待步数完成
					motor.sign_1_ok_step = 0;				// 重置标志	
				}	
				for(i = length - 1;i >= 0 ;i--){ 								/* 降 */
					TIM8->ARR = z_TIM_ARR[i]; 			// 装载计数值
					TIM_Cmd(TIM8,ENABLE);	  				// 使能 TIM8
					while(!motor.sign_1_ok_step); 	// 等待步数完成
					motor.sign_1_ok_step = 0;				// 重置标志	
				}	
			}
			else{ 																												// x < 2n
				for(i = 0;i < pulse_num/2;i++){  					/* 升 */ 
					TIM8->ARR = z_TIM_ARR[i]; 			// 装载计数值（整形除以2，取整，若x为奇数，3/2=1）
					TIM_Cmd(TIM8,ENABLE);	  				// 使能 TIM8
					while(!motor.sign_1_ok_step); 	// 等待步数完成
					motor.sign_1_ok_step = 0;				// 重置标志
				}  
				if( pulse_num%2 !=0) { 										/* 匀速 */
					TIM_Cmd(TIM8,ENABLE);	  				// 使能 TIM8（x为奇数步,则匀速一步）
					while(!motor.sign_1_ok_step); 	// 等待步数完成
					motor.sign_1_ok_step = 0;				// 重置标志	
				}		
				for(i = pulse_num/2 - 1;i >= 0;i--){ 			/* 降 */
					TIM8->ARR = z_TIM_ARR[i];				// 装载计数值
					TIM_Cmd(TIM8, ENABLE);	  			// 使能 TIM8
					while(!motor.sign_1_ok_step); 	// 等待步数完成
					motor.sign_1_ok_step = 0;				// 重置标志	
				}	
			}
		}
	else	if( id == 2 )  {
				for(i = 0;i < pulse_num - sj_length;i++){  										/* 匀速 */
					TIM8->ARR = 2000 ;				// 装载计数值
					TIM_Cmd(TIM8,ENABLE);						// 使能TIM8
					while(!motor.sign_1_ok_step); 	// 等待步数完成
					motor.sign_1_ok_step = 0;				// 重置标志
				}
				for(i = sj_length - 1;i >= 0 ;i--){ 								/* 降速 */
					TIM8->ARR = sj_TIM_ARR[i]; 			// 装载计数值
					TIM_Cmd(TIM8,ENABLE);	  				// 使能 TIM8
					while(!motor.sign_1_ok_step); 	// 等待步数完成
					motor.sign_1_ok_step = 0;				// 重置标志	
				}	
	}
}
/**************************************************************************
函数功能：角度控制: (-∞ ~ +∞)  电机设置上电自动回中,角度顺增逆减 
入口参数：TargetAngle: 目标角度；  ID: 电机码
返回  值：无
**************************************************************************/
void Angle_Control(float target_angle,char id)
{
	float angle_difference;     // 角度差
	long pulse_num;
	if(SysValue.DebugStart)
		TIM_Cmd(TIM6,DISABLE);    // 关定时中断，减少对紧急任务影响
	id_motor = 0;
	switch(id){
		case id_1:{angle_difference = target_angle - motor.id[id_1].angle;}break;
		case id_2:{angle_difference = target_angle - motor.id[id_2].angle;}break;
		case id_3:{angle_difference = target_angle - motor.id[id_3].angle;}break;
		case id_4:{angle_difference = target_angle - motor.id[id_4].angle;}break; 
		default: break;
	}
	pulse_num = 200 * motor.Subdivision_coefficient * angle_difference / 360.0; 	// 得到脉冲差
  if( id == 1 ){    	// 旋转
			if(pulse_num >= 0)	id_1_DIR = 1; 					// 正转
			else{ 																			// 反转
				pulse_num = -pulse_num; 
				id_1_DIR = 0;
			}
		  TIM8->PSC = matlab_xz_pcr;   	// 装载 旋转 分频值
			id_4_val=id_3_val=id_2_val=0; id_1_val = z_TIM_ARR[length-1]/2;  		 // 选中1号电机,占空比尽量大，减少零电平时间过短检测不到变化沿的风险。
		S_go( id,  pulse_num );
	}
  else if( id == 2 ){  // 升降
		if(pulse_num >= 0)	{ id_2_DIR = 1;  target_angle = motor.id[id].angle;					}// 上升
			else{ 			//下降																
				pulse_num = -pulse_num; 
				id_2_DIR = 0;
				  TIM8->PSC = matlab_sj_pcr;     //装载 升降 分频值
			    id_4_val=id_3_val=id_1_val=0; id_2_val = 1000;  		 
          S_go( id,  pulse_num );	
			}
	}		
  motor.id[id].angle = target_angle; 							// 更新角度
	if(SysValue.DebugStart)	TIM_Cmd(TIM6, ENABLE);	// 重启定时器中断  		  					
}

/**************************************************************************
函数功能：不带加减速的角度控制( -∞ ~ +∞ )  电机设置上电自动回中,角度顺增逆减
入口参数：speed:无量纲转速（0-65535越大速度越大）angle:目标角度;id:电机码; 
返回  值：无
**************************************************************************/
void no_angle_control(u16 speed,float target_angle,char id)
{
	float angle_difference; 	// 角度差
	long pulse_num;
	u32 r,step ;    					// 整圈数、余步数
	if(SysValue.DebugStart)	
		TIM_Cmd(TIM6, DISABLE); // 关定时中断，减少对紧急任务影响 						
	id_motor = 1; 
	TIM8->PSC = 72 ;   				// 装载分频值
  TIM8->ARR = 65535 - speed;// 装载计数值
	switch(id){
		case id_1:{angle_difference = target_angle - motor.id[id_1].angle;}break;
		case id_2:{angle_difference = target_angle - motor.id[id_2].angle;}break;
		case id_3:{angle_difference = target_angle - motor.id[id_3].angle;}break;
		case id_4:{angle_difference = target_angle - motor.id[id_4].angle;}break; 
		default:break;
	}
	pulse_num = motor.Subdivision_coefficient * 200 * angle_difference / 360.0; // 得到脉冲差
	if(pulse_num >= 0)	dir_control(1,id); 							 	 			// 正转
	else{																												// 反转
		pulse_num = -pulse_num;
		dir_control(0,id);
	} 
	r = pulse_num / (motor.Subdivision_coefficient * 200);  		// 整圈数
	step = pulse_num % (motor.Subdivision_coefficient * 200); 	// 余步数
	circle_control(r);
	step_control(step);
  motor.id[id].angle = target_angle; 							// 更新角度
	if(SysValue.DebugStart)	TIM_Cmd(TIM6, ENABLE);	// 重启定时器中断  					
}
/**************************************************************************
函数功能：电机及转动方向选择  
入口参数：Dir:true正转 false反转；ID 选择电机 
返回  值：无
**************************************************************************/
void dir_control(u8 k,char id){
	switch(id){
    case id_1:{ 
				id_4_val = 0;id_3_val = 0;id_2_val = 0;
				id_1_val = TIM8->ARR / 2;
				if(k == 1)	 id_1_DIR = 1; 	
				else 				 id_1_DIR = 0;
			}break;			
		case id_2:{
				id_4_val = 0;id_3_val = 0;id_1_val = 0;
			  id_2_val = TIM8->ARR / 2;
				if(k == 1)	 id_2_DIR=1; 	
				else 				 id_2_DIR=0;
			}break;
		case id_3:{
				id_4_val = 0;id_2_val = 0;id_1_val = 0;
				id_3_val = TIM8->ARR / 2; 
				if(k == 1)	 id_3_DIR = 1; 
				else 				 id_3_DIR = 0;
			}break;
		case id_4:{
				id_3_val = 0;id_2_val = 0;id_1_val = 0;
				id_4_val = TIM8->ARR / 2;
				if(k == 1)	 id_4_DIR = 1;  
				else 				 id_4_DIR = 0;
			}break;
		default:break;
	}
}

/**************************************************************************
函数功能：控制电机转动步数
入口参数：step 步数
返回  值：无
**************************************************************************/
 void step_control(u16 step) 			// 控制转动步数
{
	  motor.step_max = step;
		TIM_Cmd(TIM8,ENABLE);	  			//使能 
		while(!motor.sign_1_ok_step); //等待步数完成标志
	  motor.sign_1_ok_step = 0;			//重置标志
}

/**************************************************************************
函数功能：控制电机转动圈数
入口参数：circle 圈数
返回  值：无
**************************************************************************/
void circle_control(u32 circle) 		// 控制转动圈数
{
	u32 count;
	motor.step_max = 200 * motor.Subdivision_coefficient / motor.single_pause_NUM;  //恢复一次一圈
	for(count = 0;count < circle;count++){
		TIM_Cmd(TIM8, ENABLE);	  			//使能 TIM8
		while(!motor.sign_1_ok_step);	 	//等待一圈完成标志
		motor.sign_1_ok_step = 0;  			//重置标志
	}
}

