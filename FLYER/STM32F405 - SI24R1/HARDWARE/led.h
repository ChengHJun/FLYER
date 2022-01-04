#ifndef __LED_H
#define __LED_H

//#include "stm32f4xx_gpio.h"
#include "stm32f4xx.h"
#define LED PCout(14)
#define LED_OFF PCout(14)=1
#define LED_ON PCout(14)=0

void Led_Init(void);
#endif
