#include "common.h"
#include "usart.h"
#include "string.h"
#include "gpio.h"

/*******************************************************/
#define DEBUG_USART                         USART1    
#define DEBUG_USART_BAUDRATE                115200
#define DEBUG_USART_RX_AF                   GPIO_AF_USART1
#define DEBUG_USART_TX_AF                   GPIO_AF_USART1
/************************************************************/

static debug_struct debug;
static MONITOR_STRUCT monitor;

/**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Ƕ�������жϿ�������ѡ�� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
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
	USART_InitTypeDef USART_InitStructure;
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_PIN | DEBUG_USART_RX_PIN;  
	GPIO_Init(DEBUG_USART_TX_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(DEBUG_USART_TX_PORT,GPIO_PinSource10,DEBUG_USART_RX_AF);
	GPIO_PinAFConfig(DEBUG_USART_TX_PORT,GPIO_PinSource9,DEBUG_USART_TX_AF);

	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	/* Ӳ�������ƣ���ʹ��Ӳ���� */
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(DEBUG_USART, &USART_InitStructure); 
#if 1
	/* Ƕ�������жϿ�����NVIC���� */
	NVIC_Configuration();
	/* ʹ�ܴ��ڽ����ж� */
	USART_ITConfig(DEBUG_USART, USART_IT_RXNE, ENABLE);
#endif
	/* ʹ�ܴ��� */
	USART_Cmd(DEBUG_USART, ENABLE);
}

void reset_data(void)
{
	memset(&debug, 0, sizeof(debug));
}
/*****************  ����һ���ַ� **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	USART_SendData(pUSARTx,ch);
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/*****************  �����ַ��� **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* �ȴ�������� */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
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
#endif
//__use_no_semihosting was requested, but _ttywrch was 
void _ttywrch(int ch)
{
    ch = ch;
}

//void receive_IRQ(void)
void USART1_IRQHandler(void)
{
#ifdef MONITOR
	u8 ret;
	if (USART_GetITStatus(DEBUG_USART, USART_IT_RXNE) != RESET) { //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		ret = USART_ReceiveData(DEBUG_USART);	//(DEBUG_USART->DR);  //��ȡ���յ�������
		if (!monitor.use_flag) {	//������һ������Ž���һ��
			debug.receive_buf[debug.receive_count++] = ret;	//++���ǳ��ȣ�Ҳ����һ�ε��±�
			Usart_SendByte(DEBUG_USART, ret);	//��ӡ��������ʲô
			if (0x0D == ret) {
				Usart_SendString(DEBUG_USART, "\r\n");	
				debug.receive_buf[debug.receive_count - 1] = '\0';	//�ѻس�����\0
				//debug.receive_buf[debug.receive_count++] = '\n';
				monitor.use_flag = 1;
				/*else {	//monitorǰ���¼���ûִ�С�
					Usart_SendString(DEBUG_USART, "UR ERR\r\n");	
				}*/
				//reset_data();
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


#ifdef MONITOR
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

