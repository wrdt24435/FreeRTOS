#include "common.h"  
#include "sys.h"  
#include "stm32f4xx.h"

#if DEBUG
#define SYS_PRINTF PRINTF
#else
#define SYS_PRINTF
#endif

/***************************DMA**********************************/
//DMA配置，存储器到外设（8位）等配置
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMA通道选择,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
//par:外设地址
//mar:存储器地址
//ndtr:数据传输量  
//MYDMA_Config(DMA2_Stream7,DMA_Channel_4,(u32)&USART1->DR,(u32)wakeup, sizeof(wakeup));
//MYDMA_Enable(DMA2_Stream7, sizeof(wakeup));
void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr)
{ 

	DMA_InitTypeDef  DMA_InitStructure;
	if((u32)DMA_Streamx>(u32)DMA2)//得到当前stream是属于DMA2还是DMA1
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2时钟使能		
	}else 
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1时钟使能 
	}
	DMA_DeInit(DMA_Streamx);
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}//等待DMA可配置 
	/* 配置 DMA Stream */
	DMA_InitStructure.DMA_Channel = chx;  //通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr = par;//DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = mar;//DMA 存储器0地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//存储器到外设模式
	DMA_InitStructure.DMA_BufferSize = ndtr;//数据传输量 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度:8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 使用普通模式 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//中等优先级
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
	DMA_Init(DMA_Streamx, &DMA_InitStructure);//初始化DMA Stream
} 
//开启一次DMA传输
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7 
//ndtr:数据传输量  
void MYDMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)
{
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);  //使能串口1的DMA发送
 
	DMA_Cmd(DMA_Streamx, DISABLE);                      //关闭DMA传输	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//确保DMA可以被设置  
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);          //数据传输量  
	DMA_Cmd(DMA_Streamx, ENABLE);                      //开启DMA传输 

	u32 time = check_tick();
    while(1)
    {
		if(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7)!=RESET)//等待DMA2_Steam7传输完成
		{ 
			DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);//清除DMA2_Steam7传输完成标志
			break; 
        }
    }
	//delay_ms(900);
	SYS_PRINTF("DMA Done! S:%d  E:%d\n",time, check_tick());
}	  

/*const char wakeup[] = "So wake me up when it's all over, When I'm wiser and I'm older. All this time I was finding myself and I didn't kown I was lost.";
#define TEST_SIZE 8192	
char test_buf[TEST_SIZE];
void DMA_test()
{	
	//DMA2,STEAM7,CH4,外设为串口1,存储器为SendBuff,长度为:SEND_BUF_SIZE.
 	MYDMA_Config(DMA2_Stream7,DMA_Channel_4,(u32)&USART1->DR,(u32)wakeup, sizeof(wakeup));
	MYDMA_Enable(DMA2_Stream7,sizeof(wakeup));
	for(int i = 0;i < TEST_SIZE;i++)//填充ASCII字符集数据
    {
		test_buf[i] = '7'; 	   
    }	
 	MYDMA_Config(DMA2_Stream7, DMA_Channel_4, (u32)&USART1->DR, (u32)test_buf, TEST_SIZE);
	MYDMA_Enable(DMA2_Stream7, TEST_SIZE);
}*/
/***************************DMA**********************************/


/***************************RNG**********************************/
u8 RNG_Init(void)
{
	u16 retry=0; 
	
  	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);//开启RNG时钟,来自PLL48CLK
	
	RNG_Cmd(ENABLE);	//使能RNG
	
	while(RNG_GetFlagStatus(RNG_FLAG_DRDY)==RESET&&retry<10000)	//等待随机数就绪
	{
		retry++;
		delay_ms(100);
	}
	if(retry>=10000) {
		ERR;
		return 1;//随机数产生器工作不正常
	}
	return 0;
}
//得到随机数
//返回值:获取到的随机数
u32 RNG_Get_RandomNum(void)
{	 
	while(RNG_GetFlagStatus(RNG_FLAG_DRDY)==RESET);	//等待随机数就绪  
	return RNG_GetRandomNumber();	
}

void rng_test()
{
	u32 arr[6];
	arr[0] = RNG_Get_RandomNum();
	arr[1] = RNG_Get_RandomNum();
	arr[2] = get_random_number()%((u32)(10-0+1)) +0;
	arr[3] = get_random_number()%((u32)(10-0+1)) +0;
	arr[4] = get_random_number()%((u32)(77-7+1)) +7;
	arr[5] = get_random_number()%((u32)(77-7+1)) +7;
	SYS_PRINTF("%u, %u\r\n0-10:%u, %u\r\n7-77:%u, %u\r\n",arr[0] ,arr[1], arr[2], arr[3], arr[4], arr[5]);
}
/***************************RNG**********************************/


/***************************低功耗**********************************/
//系统进入待机模式
void Sys_Enter_Standby(void)
{		
	RCC_AHB1PeriphResetCmd(0X04FF,ENABLE);//复位所有IO口
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//使能PWR时钟		 
	PWR_BackupAccessCmd(ENABLE);//后备区域访问使能
	PWR_ClearFlag(PWR_FLAG_WU);//清除Wake-up 标志
	PWR_WakeUpPinCmd(ENABLE);//设置WKUP用于唤醒
	//RTC唤醒!
	SYS_PRINTF("Sleep in ...");
	PWR_EnterSTANDBYMode();	//进入待机模式
	SYS_PRINTF("Sleep out!");
	//重新打开外设
	 
}
/***************************低功耗**********************************/

//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
__asm void WFI_SET(void)
{
	WFI;		  
}
//关闭所有中断(但是不包括fault和NMI中断)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
//开启所有中断
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}
















