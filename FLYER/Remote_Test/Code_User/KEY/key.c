#include "key.h"
#include "oled.h"

//���尴��ɨ��������
u8 KEY_BUF[KEY_NUM]={0};
u16 KEY_COUNT[KEY_NUM]={0};
u8 KEY_PORT[KEY_NUM]={PORT_A,PORT_A,PORT_A,PORT_B,PORT_B,PORT_B,PORT_B,PORT_B,PORT_A,PORT_C,PORT_A,PORT_C};
u8 KEY_PIN[KEY_NUM]={11,12,15,3,6,7,8,9,0,14,8,15};
u8 KEY_STATE[KEY_NUM]={KEY_L,KEY_L,KEY_L,KEY_L,KEY_L,KEY_L,KEY_L,KEY_L,KEY_H,KEY_L,KEY_L,KEY_L};
// ������ʼ��
//---------------------
void KEY_Init(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;						// ����GPIO_InitTypeDef�ͽṹ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		// JTAGʧ��
	
	// ʹ��GPIOA��GPIOC�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	// ʹ��GPIOʱ��
	
	// ������WKUP--PA0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				// WKUP--PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 			// ��������
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// ����ģʽ����Ҫ��˿��ٶ�
	GPIO_Init(GPIOA, &GPIO_InitStructure);					// �����趨������ʼ��PA0
	
	// ������KEY_LeftRocker--PA8��KEY_UP--PA11��KEY_DOWN--PA12��KEY_LEFT--PA15
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			// ��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);					// �����趨������ʼ��
	
	// ������KEY_RIGHT--PB3��KEY_A--PB6��KEY_A+--PB7��KEY_B--PB8��KEY_B+--PB9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			// ��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);					// �����趨������ʼ��
	
	// ������KEY_1--PC14��KEY_RightRocker--PC15
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			// ��������
	GPIO_Init(GPIOC, &GPIO_InitStructure);					// �����趨������ʼ��
}





void KEY_Scan(void)
{
	if(KEY_UP_in==0) KEY_COUNT[KEY_UP]++;
	else
	{
		if((KEY_COUNT[KEY_UP]>=KEY_SHORT_TIME)&&(KEY_COUNT[KEY_UP]<=KEY_LONG_TIME)) KEY_BUF[KEY_UP]=KEY_SHORT;
		else if((KEY_COUNT[KEY_UP]>KEY_LONG_TIME)&&(KEY_COUNT[KEY_UP]<=KEY_ERROR_TIME)) KEY_BUF[KEY_UP]=KEY_LONG;
		else if(KEY_COUNT[KEY_UP]>KEY_ERROR_TIME) KEY_BUF[KEY_UP]=KEY_ERROR;
//		else KEY_BUF[KEY_UP]=KEY_NONE;
		KEY_COUNT[KEY_UP]=0;
	}
}

//���ذ��µİ�������
u8 KEY_Scan2(void)
{
	u8 i=0;
	u8 Pin_State=1;
	u8 key=0;
	for(i=0;i<KEY_NUM;i++)
	{
		//��ȡ����������
		if(KEY_PORT[i]==PORT_A) Pin_State=PA_in(KEY_PIN[i]);
		else if(KEY_PORT[i]==PORT_B) Pin_State=PB_in(KEY_PIN[i]);
		else if(KEY_PORT[i]==PORT_C) Pin_State=PC_in(KEY_PIN[i]);
		//�жϰ����Ƿ���
		if(Pin_State==KEY_STATE[i])  //�������£����м���
		{
			//����ʱ��������ж�Ϊ��������
			if(KEY_COUNT[i]++>KEY_ERROR_TIME)  //��������ʱ�����
			{
				KEY_BUF[i]=KEY_ERROR; //�ж�Ϊ�����ֵ
				KEY_COUNT[i]=KEY_ERROR_TIME;  //�����޷�
			}
			key++;
		}
		else  //�����ɿ����жϰ�������
		{
			if((KEY_COUNT[i]>=KEY_SHORT_TIME)&&(KEY_COUNT[i]<=KEY_LONG_TIME)) KEY_BUF[i]=KEY_SHORT;  //�̰�
			else if((KEY_COUNT[i]>KEY_LONG_TIME)&&(KEY_COUNT[i]<=KEY_ERROR_TIME)) KEY_BUF[i]=KEY_LONG;  //����
//			else KEY_BUF[i]=KEY_NONE;
			KEY_COUNT[i]=0;  //��������
		}
	}
	return key;
}

