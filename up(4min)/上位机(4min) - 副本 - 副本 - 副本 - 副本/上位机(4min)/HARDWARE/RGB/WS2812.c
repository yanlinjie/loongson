#include "WS2812.h" 

/* 单个灯珠的编码对应的比较值数组 */
uint16_t Single_LED_Buffer[DATA_SIZE*LED_NUM]; 

/**************************************************************************
函数功能：WS2812灯带DMA初始化
入口参数：void
返回  值：void
**************************************************************************/
void WS2812_DMA_Init(void){
	DMA_InitTypeDef DMA_InitStructure;
	 
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);														//使能DMA1时钟    
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(TIM2->CCR2); 				//设置DMA目的地址
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Single_LED_Buffer;					//设置DMA源地址
 	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;													//方向：从内存SendBuff到内存ReceiveBuff  
 	DMA_InitStructure.DMA_BufferSize = DATA_SIZE; 															//一次传输大小DMA_BufferSize=SendBuffSize    
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 						//ReceiveBuff地址不增
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;											//SendBuff地址自增
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//ReceiveBuff数据单位,16bit
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;					//SENDBUFF_SIZE数据单位,16bit
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;																//DMA模式：正常模式(传输一次)
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;											//优先级：最高
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 																//内存到内存的传输
  DMA_Init(DMA1_Channel7, &DMA_InitStructure);																//配置DMA1的7通道(不同定时器的通道不一样)
  
  DMA_Cmd(DMA1_Channel7, DISABLE);																						//失能DMA1的7通道，因为一旦使能就开始传输
}

/**************************************************************************
函数功能：复位灯带
入口参数：void
返回  值：void
**************************************************************************/
void WS2812B_Reset(void)
{
	TIM_Cmd(TIM2, DISABLE);
	WS2812B_LOW;    
	delay_ms(1);	//此处延时时间最小值大于50us即可
}

/**************************************************************************
函数功能：写数据编码
入口参数：void
返回  值：void
**************************************************************************/
void PWM_WS2812B_Write_24Bits(u8 num,uint32_t GRB_Data)
{
  uint8_t i,j;
  for(j = 0; j < num; j++)
  {
  	for(i = 0; i < DATA_SIZE; i++)
		{
			/*因为数据发送的顺序是GRB，高位先发，所以从高位开始判断，判断后比较值先放入缓存数组*/
			Single_LED_Buffer[i+j*DATA_SIZE] = ((GRB_Data << i) & 0x800000) ? T1H : T0H;
		}
  }
}

/**************************************************************************
函数功能：点亮灯珠
入口参数：void
返回  值：void
注意事项：移植时此处对应的通道和中断标志都需要更改
**************************************************************************/
void PWM_WS2812B_Show(u8 num) 
{
  	DMA_SetCurrDataCounter(DMA1_Channel7, num*DATA_SIZE);
  	DMA_Cmd(DMA1_Channel7, ENABLE);
  	TIM_Cmd(TIM2, ENABLE);
  	while(DMA_GetFlagStatus(DMA1_FLAG_TC7) != SET);
  	DMA_Cmd(DMA1_Channel7, DISABLE);
  	DMA_ClearFlag(DMA1_FLAG_TC7);
  	TIM_Cmd(TIM2, DISABLE);
}

/**************************************************************************
函数功能：灯环整体设置颜色
入口参数：red、green、blue 三原色
返回  值：void
注意事项：green << 16 | red << 8 | blue 将三原色单独数据合并为24位数据
**************************************************************************/
void WS2812B_SetColor(Color_WS2812 Color)
{
	uint8_t red,green,blue;
	switch(Color){
		case Red:		red = 255,green = 0,  blue = 0;break;
		case Green:	red = 0,  green = 255,blue = 0;break;
		case Blue:	red = 0,  green = 0,  blue = 255;break;
		case White: red = 255,green = 255,blue = 150;break;
		case None: 	red = 0,  green = 0,  blue = 0;break;
	}
	PWM_WS2812B_Write_24Bits(LED_NUM,green << 16 | red << 8 | blue);
	PWM_WS2812B_Show(LED_NUM);
	delay_ms(2);
	PWM_WS2812B_Show(LED_NUM);	
	delay_ms(2);
	PWM_WS2812B_Show(LED_NUM);	//传输三次避免出错
}

/**************************************************************************
函数功能：灯环第n位设置颜色
入口参数：void
返回  值：void
注意事项：先将Num位数组填充所需颜色，再把前Num - 1为数组清空
**************************************************************************/
void WS2812B_SetColor_N(u8 Num,uint8_t red,uint8_t green,uint8_t blue)
{
	PWM_WS2812B_Write_24Bits(Num,green << 16 | red << 8 | blue);
	PWM_WS2812B_Write_24Bits(Num - 1,0x000000);
	PWM_WS2812B_Show(Num);
}
