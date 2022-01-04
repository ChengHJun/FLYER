#include "usb_bsp.h"
#include "sys.h"  
#include "delay.h" 
#include "usart.h" 

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//USB-BSP 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/1/21
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//*******************************************************************************
//修改信息
//无
////////////////////////////////////////////////////////////////////////////////// 	   
 
  
//USB OTG 底层IO初始化
//pdev:USB OTG内核结构体指针
void USB_OTG_BSP_Init(USB_OTG_CORE_HANDLE *pdev)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS,ENABLE);        //使能OTG FS时钟

        //配置PA11 12
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;           //无上下拉
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
  GPIO_Init(GPIOA, &GPIO_InitStruct);         //初始化

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_OTG_FS); //PB3复用为 SPI1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_OTG_FS); //PB4复用为 SPI1
}

//USB OTG 中断设置,开启USB FS中断
//pdev:USB OTG内核结构体指针
void USB_OTG_BSP_EnableInterrupt(USB_OTG_CORE_HANDLE *pdev)
{  	
	NVIC_SetPriority(OTG_FS_IRQn,2);  //抢占优先级0，子优先级3
	NVIC_EnableIRQ(OTG_FS_IRQn);  //使能OTG USB FS中断 
}

//USB OTG 中断设置,开启USB FS中断
//pdev:USB OTG内核结构体指针
void USB_OTG_BSP_DisableInterrupt(void)
{ 
}
//USB OTG 端口供电设置(本例程未用到)
//pdev:USB OTG内核结构体指针
//state:0,断电;1,上电
void USB_OTG_BSP_DriveVBUS(USB_OTG_CORE_HANDLE *pdev, uint8_t state)
{ 
}
//USB_OTG 端口供电IO配置(本例程未用到)
//pdev:USB OTG内核结构体指针
void  USB_OTG_BSP_ConfigVBUS(USB_OTG_CORE_HANDLE *pdev)
{ 
} 
//USB_OTG us级延时函数
//本例程采用SYSTEM文件夹的delay.c里面的delay_us函数实现
//官方例程采用的是定时器2来实现的.
//usec:要延时的us数.
void USB_OTG_BSP_uDelay (const uint32_t usec)
{ 
   	delay_us(usec);
}
//USB_OTG ms级延时函数
//本例程采用SYSTEM文件夹的delay.c里面的delay_ms函数实现
//官方例程采用的是定时器2来实现的.
//msec:要延时的ms数.
void USB_OTG_BSP_mDelay (const uint32_t msec)
{  
	delay_ms(msec);
}






















