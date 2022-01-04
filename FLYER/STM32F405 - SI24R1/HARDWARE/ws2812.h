#ifndef __WS2812_H__
#define __WS2812_H__
#include <stdbool.h>
#include "sys.h"
#include "delay.h"
#include "stdlib.h"

//#define   RGB_H     GPIOB->BSRR |= GPIO_Pin_9   //配置对应RGB引脚为 高电平
//#define   RGB_L     GPIOB->BRR  |= GPIO_Pin_9	//配置对应RGB引脚为 低电平
#define   RGB_H     GPIO_SetBits(GPIOB,GPIO_Pin_14)   //配置对应RGB引脚为 高电平
#define   RGB_L     GPIO_ResetBits(GPIOB,GPIO_Pin_14)	//配置对应RGB引脚为 低电平

void RGBLED_Init(void);
void LED_Run(void);
void Write24Bit(uint8_t green, uint8_t red, uint8_t blue);
void RGB_LED_Rand(void);
void RGB_LED_green(void);
void RGB_LED_Red(void);
void RGB_LED_Runing(void);
void RGB_LED_Off(void);
#endif /*__WS2812_H__*/
