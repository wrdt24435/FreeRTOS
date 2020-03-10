#include "common.h"  
#include "sys.h"  
#include "stm32f4xx.h"

#if DEBUG
#define SYS_PRINTF PRINTF
#else
#define SYS_PRINTF
#endif

/***************************DMA**********************************/
//DMA���ã��洢�������裨8λ��������
//DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMAͨ��ѡ��,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
//par:�����ַ
//mar:�洢����ַ
//ndtr:���ݴ�����  
//MYDMA_Config(DMA2_Stream7,DMA_Channel_4,(u32)&USART1->DR,(u32)wakeup, sizeof(wakeup));
//MYDMA_Enable(DMA2_Stream7, sizeof(wakeup));
void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr)
{ 

	DMA_InitTypeDef  DMA_InitStructure;
	if((u32)DMA_Streamx>(u32)DMA2)//�õ���ǰstream������DMA2����DMA1
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2ʱ��ʹ��		
	}else 
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1ʱ��ʹ�� 
	}
	DMA_DeInit(DMA_Streamx);
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}//�ȴ�DMA������ 
	/* ���� DMA Stream */
	DMA_InitStructure.DMA_Channel = chx;  //ͨ��ѡ��
	DMA_InitStructure.DMA_PeripheralBaseAddr = par;//DMA�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = mar;//DMA �洢��0��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//�洢��������ģʽ
	DMA_InitStructure.DMA_BufferSize = ndtr;//���ݴ����� 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�������ݳ���:8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�洢�����ݳ���:8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// ʹ����ͨģʽ 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//�е����ȼ�
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
	DMA_Init(DMA_Streamx, &DMA_InitStructure);//��ʼ��DMA Stream
} 
//����һ��DMA����
//DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7 
//ndtr:���ݴ�����  
void MYDMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)
{
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);  //ʹ�ܴ���1��DMA����
 
	DMA_Cmd(DMA_Streamx, DISABLE);                      //�ر�DMA����	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//ȷ��DMA���Ա�����  
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);          //���ݴ�����  
	DMA_Cmd(DMA_Streamx, ENABLE);                      //����DMA���� 

	u32 time = check_tick();
    while(1)
    {
		if(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7)!=RESET)//�ȴ�DMA2_Steam7�������
		{ 
			DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);//���DMA2_Steam7������ɱ�־
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
	//DMA2,STEAM7,CH4,����Ϊ����1,�洢��ΪSendBuff,����Ϊ:SEND_BUF_SIZE.
 	MYDMA_Config(DMA2_Stream7,DMA_Channel_4,(u32)&USART1->DR,(u32)wakeup, sizeof(wakeup));
	MYDMA_Enable(DMA2_Stream7,sizeof(wakeup));
	for(int i = 0;i < TEST_SIZE;i++)//���ASCII�ַ�������
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
	
  	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);//����RNGʱ��,����PLL48CLK
	
	RNG_Cmd(ENABLE);	//ʹ��RNG
	
	while(RNG_GetFlagStatus(RNG_FLAG_DRDY)==RESET&&retry<10000)	//�ȴ����������
	{
		retry++;
		delay_ms(100);
	}
	if(retry>=10000) {
		ERR;
		return 1;//���������������������
	}
	return 0;
}
//�õ������
//����ֵ:��ȡ���������
u32 RNG_Get_RandomNum(void)
{	 
	while(RNG_GetFlagStatus(RNG_FLAG_DRDY)==RESET);	//�ȴ����������  
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


/***************************�͹���**********************************/
//ϵͳ�������ģʽ
void Sys_Enter_Standby(void)
{		
	RCC_AHB1PeriphResetCmd(0X04FF,ENABLE);//��λ����IO��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//ʹ��PWRʱ��		 
	PWR_BackupAccessCmd(ENABLE);//���������ʹ��
	PWR_ClearFlag(PWR_FLAG_WU);//���Wake-up ��־
	PWR_WakeUpPinCmd(ENABLE);//����WKUP���ڻ���
	//RTC����!
	SYS_PRINTF("Sleep in ...");
	PWR_EnterSTANDBYMode();	//�������ģʽ
	SYS_PRINTF("Sleep out!");
	//���´�����
	 
}
/***************************�͹���**********************************/

//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
__asm void WFI_SET(void)
{
	WFI;		  
}
//�ر������ж�(���ǲ�����fault��NMI�ж�)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
//���������ж�
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}
















