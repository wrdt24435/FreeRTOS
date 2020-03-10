#include "common.h"
#include "stm32f4xx.h"
#include "wwdg.h"
 
u8 WWDG_CNT=0X7F;
//��ʼ�����ڿ��Ź�, ����Ϊ48msҪιһ�Σ��Ѿ�����ٵģ�
//tr   :T[6:0],������ֵ 
//wr   :W[6:0],����ֵ 
//fprer:��Ƶϵ����WDGTB��,�����2λ��Ч 
//Fwwdg=PCLK1/(4096*2^fprer). һ��PCLK1=42Mhz
void WWDG_Init()
{
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE); //ʹ�ܴ��ڿ��Ź�ʱ��

	//WWDG_CNT=tr&WWDG_CNT;   //��ʼ��WWDG_CNT. 
	WWDG_SetPrescaler(WWDG_Prescaler_8); //���÷�Ƶֵ
	WWDG_SetWindowValue(0X5F); //���ô���ֵ
	//	WWDG_SetCounter(WWDG_CNT);//���ü���ֵ
	WWDG_Enable(WWDG_CNT);  //�������Ź�

	NVIC_InitStructure.NVIC_IRQChannel=WWDG_IRQn;  //���ڿ��Ź��ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02;  //��ռ���ȼ�Ϊ2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03;					//�����ȼ�Ϊ3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;  //ʹ�ܴ��ڿ��Ź�
	NVIC_Init(&NVIC_InitStructure);

	WWDG_ClearFlag();//�����ǰ�����жϱ�־λ
	WWDG_EnableIT();//������ǰ�����ж�
}

void WWDG_feed(u32 counter)
{
	WWDG_SetCounter((u8)counter);
}



//���ڿ��Ź��жϷ������ 
void WWDG_IRQHandler(void)
{
	WWDG_ClearFlag();//�����ǰ�����жϱ�־λ
	WWDG_DeInit(); 
	PRINTF("WDG! pl RES\r\n");
	while(1);
}

