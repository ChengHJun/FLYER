#ifndef __KEY_H
#define __KEY_H


#include "stm32f10x.h"
#include "bit_band.h"
#include "delay.h"

//定义按键数量
#define KEY_NUM 12
//定义按键长按、短按时间（ms）
#define KEY_SHORT_TIME 10
#define KEY_LONG_TIME 500
#define KEY_ERROR_TIME 5000
//定义键值
#define KEY_NONE 0
#define KEY_SHORT 1
#define KEY_LONG 2
#define KEY_ERROR 3
//定义按键端口
#define PORT_A 0
#define PORT_B 1
#define PORT_C 2
//定义按键按下的电平
#define KEY_L 0
#define KEY_H 1
// 按键输入宏定义
//------------------------------------------------------------------------
#define 	WKUP_in					PA_in(0)	// WKUP--PA0
#define 	KEY1_in					PC_in(14)	// KEY_1--PC14

#define 	KEY_UP_in				PA_in(11)	// KEY_UP--PA11
#define 	KEY_DOWN_in				PA_in(12)	// KEY_DOWN--PA12
#define 	KEY_LEFT_in				PA_in(15)	// KEY_LEFT--PA15
#define 	KEY_RIGHT_in			PB_in(3)	// KEY_RIGHT--PB3

#define 	KEY_A_in				PB_in(6)	// KEY_A  -- PB6
#define 	KEY_AA_in				PB_in(7)	// KEY_A+ -- PB7
#define 	KEY_B_in				PB_in(8)	// KEY_B  -- PB8
#define 	KEY_BB_in				PB_in(9)	// KEY_B+ -- PB9

#define 	KEY_Rocker1_in			PA_in(8)	// KEY_LeftRocker--PA8
#define 	KEY_Rocker2_in			PC_in(15)	// KEY_RightRocker--PC15
//------------------------------------------------------------------------
#define KEY_UP 0
#define KEY_DOWN 1
#define KEY_LEFT 2
#define KEY_RIGHT 3
#define KEY_A  4
#define KEY_AA 5
#define KEY_B  6
#define KEY_BB 7
#define KEY_WAKE 8
#define KEY_ELSE 9
#define KEY_LROCKER 10
#define KEY_RROCKER 11

extern u8 KEY_BUF[KEY_NUM];
// 按键初始化
//----------------------------
void KEY_Init(void);

// 按键扫描函数
//----------------------------
void KEY_Scan(void);
u8 KEY_Scan2(void);

#endif	/* __KEY_H */
