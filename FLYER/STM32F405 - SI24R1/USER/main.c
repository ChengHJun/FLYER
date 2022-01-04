#include "stm32f4xx.h"
#include "led.h"
#include  "sys.h"
#include "delay.h"
#include "usart.h"
#include "ws2812.h"
#include "mpu6050.h"
#include "24l01.h"
#include "usbd_cdc_vcp.h"
#include "usbd_usr.h" 
#include "usbd_desc.h" 
#include "motor.h"

#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
//si24r1调试ok
//ws2812 ok
//电机调试ok
//MPU OK

//串口1发送1个字符 
//c:要发送的字符
void usart1_send_char(u8 c)
{

	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 
    USART_SendData(USART1,c);   

} 
//传送数据给匿名四轴上位机软件(V2.6版本)
//fun:功能字. 0XA0~0XAF
//data:数据缓存区,最多28字节!!
//len:data区有效数据个数
void usart1_niming_report(u8 fun,u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//最多28字节数据 
	send_buf[len+3]=0;	//校验数置零
	send_buf[0]=0X88;	//帧头
	send_buf[1]=fun;	//功能字
	send_buf[2]=len;	//数据长度
	for(i=0;i<len;i++)send_buf[3+i]=data[i];			//复制数据
	for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];	//计算校验和	
	for(i=0;i<len+4;i++)usart1_send_char(send_buf[i]);	//发送数据到串口1 
}
//发送加速度传感器数据和陀螺仪数据
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
void mpu6050_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz)
{
	u8 tbuf[12]; 
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;
	usart1_niming_report(0XA1,tbuf,12);//自定义帧,0XA1
}	
//通过串口1上报结算后的姿态数据给电脑
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
//roll:横滚角.单位0.01度。 -18000 -> 18000 对应 -180.00  ->  180.00度
//pitch:俯仰角.单位 0.01度。-9000 - 9000 对应 -90.00 -> 90.00 度
//yaw:航向角.单位为0.1度 0 -> 3600  对应 0 -> 360.0度
void usart1_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw)
{
	u8 tbuf[28]; 
	u8 i;
	for(i=0;i<28;i++)tbuf[i]=0;//清0
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;	
	tbuf[18]=(roll>>8)&0XFF;
	tbuf[19]=roll&0XFF;
	tbuf[20]=(pitch>>8)&0XFF;
	tbuf[21]=pitch&0XFF;
	tbuf[22]=(yaw>>8)&0XFF;
	tbuf[23]=yaw&0XFF;
	usart1_niming_report(0XAF,tbuf,28);//飞控显示帧,0XAF
} 
  

USB_OTG_CORE_HANDLE    USB_OTG_dev;
void SI24R1_RXTEST(void);
void SI24R1_TXTEST(void);
int main()
{
	delay_init(168);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	uart_init(115200);
	Led_Init();
	NRF24L01_Init();
	while(NRF24L01_Check())
	{
		printf("nrf24l01 error\r\n");
		delay_ms(500);
	}
	printf("nrf24l01 online...\r\n");
	SI24R1_RXTEST();
//	SI24R1_TXTEST();
}


#define BUF_MAX 32
void SI24R1_RXTEST(void)
{
	u8 count=0;
	u8 res=0;
	u8 rx_buf[BUF_MAX]={0};

	NRF24L01_RX_Mode();
	delay_ms(10);
	NRF24L01_Read_Buf(NRF_READ_REG+RX_ADDR_P0,rx_buf,5); 
	printf("读取写入的地址为：%s\r\n",rx_buf);
	while(1)
	{
		res=NRF24L01_RxPacket(rx_buf);
//		printf("接收状态=0x%x\r\n",res);
		if(!res) printf("%s\r\n",rx_buf);
		if(count++>50)
		{
			count=0;
			LED=~LED;
		}
		delay_ms(10);
	}
}


void SI24R1_TXTEST(void)
{
	u8 key=0,mode=0,t=0;
	u8 res=0;
	u8 tx_buf[BUF_MAX+1]={0};
	NRF24L01_TX_Mode();	// 将SI24R1设置为发射模式
	while(1)
	{
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
		if(res==TX_OK)
		{
			printf("SI24R1 TX OK!\r\n");
		}
		LED=~LED;
		delay_ms(500);
	}
}
