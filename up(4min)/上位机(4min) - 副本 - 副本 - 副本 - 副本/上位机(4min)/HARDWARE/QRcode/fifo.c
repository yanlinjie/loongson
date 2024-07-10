#include "fifo.h"

/*
配置初始化
//	fifo_initQueue(); //初始化队列
串口接收：
//	u8 receive;
//	if(USART_GetITStatus(DEBUG_USART3,USART_IT_RXNE)!=RESET){  //如果接收到了消息
//		receive = USART_ReceiveData(USART3);  	//取出接收寄存器数据
//    fifo_enQueue(receive);  								//入队
//	}
*/

__IO FIFO_t rxFIFO = {0};

/**
	* @brief   初始化队列
	* @param   无
	* @retval  无
	*/
void fifo_initQueue(void)
{
	rxFIFO.ptrRead  = 0;
	rxFIFO.ptrWrite = 0;
}

/**
	* @brief   入队
	* @param   无
	* @retval  无
	*/
void fifo_enQueue(uint8_t data)
{
	rxFIFO.buffer[rxFIFO.ptrWrite] = data;
	
	++rxFIFO.ptrWrite;
	
	++rxFIFO.Length;
	if(rxFIFO.ptrWrite >= FIFO_SIZE)
	{
		rxFIFO.ptrWrite = 0;
	}
}

/**
	* @brief   出队
	* @param   无
	* @retval  无
	*/
uint8_t fifo_deQueue(void)
{
	uint8_t element = 0;

	element = rxFIFO.buffer[rxFIFO.ptrRead];

	++rxFIFO.ptrRead;
  --rxFIFO.Length;
	if(rxFIFO.ptrRead >= FIFO_SIZE)
	{
		rxFIFO.ptrRead = 0;
	}

	return element;
}

/**
	* @brief   判断空队列
	* @param   无
	* @retval  无
	*/
bool fifo_isEmpty(void)
{
	if(rxFIFO.ptrRead == rxFIFO.ptrWrite)
	{
		return true;
	}

	return false;
}

/**
	* @brief   计算队列长度
	* @param   无
	* @retval  无
	*/
uint8_t fifo_queueLength(void)
{
	if(rxFIFO.ptrRead <= rxFIFO.ptrWrite)
	{
		return (rxFIFO.ptrWrite - rxFIFO.ptrRead);
	}
	else
	{
		return (FIFO_SIZE - rxFIFO.ptrRead + rxFIFO.ptrWrite);
	}
}
