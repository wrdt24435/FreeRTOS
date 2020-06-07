#include "usart.h"
#include "string.h"
#include "gpio.h"
#include "FreeRTOS.h" 
#include "semphr.h"
#include "task.h"

//static void D2_NVIC_Configuration(void);
static void NVIC_Configuration(void);


/********************UART ����**************************/
static debug_struct debug;
static MONITOR_STRUCT monitor;
static void NVIC_Configuration(void);

const USART_TypeDef *uart_port[] = {
	0,
	USART1,
	USART2,
	USART3,
	UART4,
};

//mode : tx rx �жϽ���
void Usart_open( u32 port, u32 baudrate, int mode)
{
	USART_InitTypeDef USART_InitStructure;

	switch (port) {
		case 4:
			RCC_APB2PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
			//D2_NVIC_Configuration();
			break;
		default:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
			NVIC_Configuration();
	}

	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	/* Ӳ�������ƣ���ʹ��Ӳ���� */
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init((USART_TypeDef *)uart_port[port], &USART_InitStructure); 
	/* Ƕ�������жϿ�����NVIC���� */
	NVIC_Configuration();
	/* ʹ�ܴ��ڽ����ж� */
	USART_ITConfig((USART_TypeDef *)uart_port[port], USART_IT_RXNE, ENABLE);
	/* ʹ�ܴ��� */
	USART_Cmd((USART_TypeDef *)uart_port[port], ENABLE);
}


/*****************  ����һ���ַ� **********************/
void Usart_SendByte(u32 port, uint8_t ch)
{
	USART_SendData((USART_TypeDef *)uart_port[port],ch);
	while (USART_GetFlagStatus((USART_TypeDef *)uart_port[port], USART_FLAG_TXE) == RESET);	
}

/*****************  �����ַ��� **********************/
void Usart_SendString( u32 port, char *str)
{
	unsigned int k=0;
	do 
	{
		Usart_SendByte(port, *(str + k) );
		k++;
	} while(*(str + k)!='\0');

	/* �ȴ�������� */
	//while(USART_GetFlagStatus(uart_port[port], USART_FLAG_TC)==RESET);
}
/*****************  ����BUF **********************/
void Usart_Send_buf( u32 port, char *buf, u16 len)
{
//	unsigned int k=0;
	while(len--) 
	{
		Usart_SendByte(port, *(buf));
		buf++;
	}
	//while(USART_GetFlagStatus(uart_port[port], USART_FLAG_TC)==RESET);
}

/********************UART ����end**************************/

/************************UART �豸���***************************/
#if DEBUG


/**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Ƕ�������жϿ�������ѡ�� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  /* �������ȼ�Ϊ1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  /* �����ȼ�Ϊ1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}


 /**
  * @brief  DEBUG_USART GPIO ����,����ģʽ���á�115200 8-N-1 ���жϽ���ģʽ
  * @param  ��
  * @retval ��
  */
void Debug_USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_PIN | DEBUG_USART_RX_PIN;  
	GPIO_Init(DEBUG_USART_TX_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(DEBUG_USART_TX_PORT,GPIO_PinSource10,DEBUG_USART_RX_AF);
	GPIO_PinAFConfig(DEBUG_USART_TX_PORT,GPIO_PinSource9,DEBUG_USART_TX_AF);

	Usart_open(DEBUG_USART, DEBUG_USART_BAUDRATE, 0);
}

void reset_data(void)
{
	memset(&debug, 0, sizeof(debug));
}

#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	Usart_SendByte(DEBUG_USART, ch);   
	return ch;
}

//__use_no_semihosting was requested, but _ttywrch was 
void _ttywrch(int ch)
{
    ch = ch;
}
#endif
#endif


#ifdef MONITOR
extern SemaphoreHandle_t BinarySem_Handle;

//��д
u32 mon_flag_rw(u32 flag)
{
	if (flag) {
		monitor.use_flag = flag;
	}
	return monitor.use_flag;
}

u8 *get_mon_buf(void)
{
	memcpy(monitor.buf, debug.receive_buf, debug.receive_count);
	reset_data();
	return monitor.buf;
}
void reset_mon_buf(void)
{
	memset(&monitor, 0, sizeof(monitor));
}
#endif

//Uart�豸2
#ifdef Uart_device2
void UART_DEVICE2_OPEN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = UART_DEVICE2_TX_PIN | UART_DEVICE2_RX_PIN;  
	GPIO_Init(UART_DEVICE2_TX_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(UART_DEVICE2_TX_PORT,GPIO_PinSource10,GPIO_AF_UART4);
	GPIO_PinAFConfig(UART_DEVICE2_TX_PORT,GPIO_PinSource11,GPIO_AF_UART4);

	Usart_open(UART_DEVICE2_USART, UART_DEVICE2_USART_BAUDRATE, 0);
}

static void D2_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Ƕ�������жϿ�������ѡ�� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/* ����USARTΪ�ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	/* �������ȼ�Ϊ1 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	/* �����ȼ�Ϊ1 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	/* ʹ���ж� */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* ��ʼ������NVIC */
	NVIC_Init(&NVIC_InitStructure);
}
#endif
void USART1_IRQHandler(void)
{
#ifdef MONITOR
	u8 ret;
	if (USART_GetITStatus((USART_TypeDef *)uart_port[DEBUG_USART], USART_IT_RXNE) != RESET) { //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		ret = USART_ReceiveData((USART_TypeDef *)uart_port[DEBUG_USART]);	//(DEBUG_USART->DR);  //��ȡ���յ�������
		if (!monitor.use_flag) {	//������һ������Ž���һ��
			debug.receive_buf[debug.receive_count++] = ret;	//++���ǳ��ȣ�Ҳ����һ�ε��±�
			Usart_SendByte(DEBUG_USART, ret);	//��ӡ��������ʲô
			if (0x0D == ret) {
				Usart_SendString(DEBUG_USART, "\r\n");	
				debug.receive_buf[debug.receive_count - 1] = '\0';	//�ѻس�����\0
				monitor.use_flag = 1;
				 BaseType_t pxHigherPriorityTaskWoken; 
			     u32 ulReturn; 
			     /* �����ٽ�Σ��ٽ�ο���Ƕ�� */ 
			     ulReturn = taskENTER_CRITICAL_FROM_ISR(); 
			  
		         //�ͷŶ�ֵ�ź��������ͽ��յ������ݱ�־����ǰ̨�����ѯ 
		         xSemaphoreGiveFromISR(BinarySem_Handle,&pxHigherPriorityTaskWoken); 
			         
			     //portYIELD_FROM_ISR(pxHigherPriorityTaskWoken); //�����Ҫ�Ļ�����һ�������л���ϵͳ���ж��Ƿ���Ҫ�����л� 
			     /* �˳��ٽ�� */ 
			     taskEXIT_CRITICAL_FROM_ISR( ulReturn ); 

			}
			else {
				debug.receive_count &= 0x1F;
			}
		}
		else {
			Usart_SendString(DEBUG_USART, "MON busy\r\n");		
		}
	}
#endif
}

u8 *get_uart1_buf(void)
{
	return debug.receive_buf;
}

void UART4_IRQHandler(void)
{
	u8 ret;
	if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET) { //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		ret = USART_ReceiveData(UART4);	//(DEBUG_USART->DR);  //��ȡ���յ�������
		Usart_SendByte(UART_DEVICE2_USART, ret);	//��ӡ��������ʲô
	}
}

