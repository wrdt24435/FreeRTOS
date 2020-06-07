#include "common.h"
#include "stm32f4xx.h"
#include "wwdg.h"
 
u8 WWDG_CNT=0X7F;
//初始化窗口看门狗, 设置为48ms要喂一次（已经是最迟的）
//tr   :T[6:0],计数器值 
//wr   :W[6:0],窗口值 
//fprer:分频系数（WDGTB）,仅最低2位有效 
//Fwwdg=PCLK1/(4096*2^fprer). 一般PCLK1=42Mhz
void WWDG_Init()
{
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE); //使能窗口看门狗时钟

	//WWDG_CNT=tr&WWDG_CNT;   //初始化WWDG_CNT. 
	WWDG_SetPrescaler(WWDG_Prescaler_8); //设置分频值
	WWDG_SetWindowValue(0X5F); //设置窗口值
	//	WWDG_SetCounter(WWDG_CNT);//设置计数值
	WWDG_Enable(WWDG_CNT);  //开启看门狗

	NVIC_InitStructure.NVIC_IRQChannel=WWDG_IRQn;  //窗口看门狗中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02;  //抢占优先级为2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03;					//子优先级为3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;  //使能窗口看门狗
	NVIC_Init(&NVIC_InitStructure);

	WWDG_ClearFlag();//清除提前唤醒中断标志位
	WWDG_EnableIT();//开启提前唤醒中断
}

void WWDG_feed(u32 counter)
{
	WWDG_SetCounter((u8)counter);
}



//窗口看门狗中断服务程序 
void WWDG_IRQHandler(void)
{
	WWDG_ClearFlag();//清除提前唤醒中断标志位
	WWDG_DeInit(); 
	PRINTF("WDG! pl RES\r\n");
	while(1);
}

