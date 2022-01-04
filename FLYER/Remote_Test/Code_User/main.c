#include "stm32f10x.h"
#include "bit_band.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "timer.h"
#include "oled.h"
#include "adc.h"
#include "spi.h"
#include "24l01.h"
#include "dma.h"

u8 TIME_FLAG=0;
#define BUF_MAX 32
void NRF24_TEST(void);
void Show_Key(void);

int main(void)
{	
	u8 Event_Count=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //NVIC分组2：2位抢占优先级，2位响应优先级
	delay_Init();				// 延时初始化（注：调用延时函数之前，必须先调用delay_Init()将SysTick初始化）
	LED_Init() ;				// 初始化LED硬件接口：Green--PB4、Blue--PB5
	KEY_Init();
	TIM3_Time_Init();
	NRF24L01_Init();				// 2.4G无线射频初始化
	// 初始化OLED
	//-----------------------------------------
	OLED_Init();				// 初始化OLED  
	OLED_Clear(); 				// 清屏
	OLED_ShowString(0, 0, "2.4G Telecontrol");
	//-----------------------------------------
	
	// 判断SI24R1是否正常
	//-----------------------------------------
	if(NRF24L01_Check())
	{ OLED_ShowString(0, 2, "2.4G ERROR"); }
	else
	{ OLED_ShowString(0, 2, "2.4G OK"); }
	//-----------------------------------------
	
	NRF24L01_TX_Mode();	// 将SI24R1设置为发射模式
	while(1)
	{
		if(TIME_FLAG)
		{
			KEY_Scan2();
				Show_Key();
			Event_Count++;
			TIME_FLAG=0;
		}
		
		if(Event_Count>=200)
		{
			Event_Count=0;
//			OLED_ShowNum(30,4,KEY_BUF[KEY_UP],3,16);
			NRF24_TEST();
		}
	}
}

void Show_Key(void)
{
	u8 i=0;
	for(i=0;i<KEY_NUM;i++)
	{
		OLED_ShowNum(0+i*8,6,KEY_BUF[i],1,16);
	}
}

void NRF24_TEST(void)
{
	u8 key=0,t=0;
	static u8 mode=0;
	u8 res=0;
	u8 tx_buf[BUF_MAX+1]={0};
	key=mode;
		for(t=0;t<32;t++)
		{
			key++;
			if(key>('~'))key=' ';
			tx_buf[t]=key;	
		}
		mode++; 
		if(mode>'~')mode=' ';  	  
		tx_buf[32]=0;//加入结束符	
		
		res=NRF24L01_TxPacket(tx_buf);	// 发射指令
		OLED_ShowNum(0,4,res,3,16);
		if(res==TX_OK)
		{
			LED_Green=~LED_Green;
			OLED_ShowString(40, 4, "TX OK  ");
		}
		else
		{
			OLED_ShowString(40, 4, "TX FAIL");
		}
		LED_Blue=~LED_Blue;
}
