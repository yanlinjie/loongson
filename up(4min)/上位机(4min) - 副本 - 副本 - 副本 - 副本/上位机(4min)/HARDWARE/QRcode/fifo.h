#ifndef __FIFO_H
#define __FIFO_H

#include "sys.h"

#define 	FIFO_SIZE   210
typedef struct {
	uint8_t buffer[FIFO_SIZE]; //队列
	__IO uint8_t ptrWrite;     //队尾
	__IO uint8_t ptrRead;		   //队头
	__IO uint8_t Length;     	 //队长
}FIFO_t;

extern __IO FIFO_t rxFIFO;

void fifo_initQueue(void);
void fifo_enQueue(uint8_t data);
uint8_t fifo_deQueue(void);
bool fifo_isEmpty(void);
uint8_t fifo_queueLength(void);

#endif
