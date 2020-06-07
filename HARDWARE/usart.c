#include "usart.h"
#include "string.h"
#include "gpio.h"
#include "FreeRTOS.h" 
#include "semphr.h"
#include "task.h"

//static void D2_NVIC_Configuration(void);
static void NVIC_Configuration(void);


/********************UART 驱动**************************/
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

//mode : tx rx 中断接收
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
	/* 硬件流控制：不使用硬件流 */
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init((USART_TypeDef *)uart_port[port], &USART_InitStructure); 
	/* 嵌套向量中断控制器NVIC配置 */
	NVIC_Configuration();
	/* 使能串口接收中断 */
	USART_ITConfig((USART_TypeDef *)uart_port[port], USART_IT_RXNE, ENABLE);
	/* 使能串口 */
	USART_Cmd((USART_TypeDef *)uart_port[port], ENABLE);
}


/*****************  发送一个字符 **********************/
void Usart_SendByte(u32 port, uint8_t ch)
{
	USART_SendData((USART_TypeDef *)uart_port[port],ch);
	while (USART_GetFlagStatus((USART_TypeDef *)uart_port[port], USART_FLAG_TXE) == RESET);	
}

/*****************  发送字符串 **********************/
void Usart_SendString( u32 port, char *str)
{
	unsigned int k=0;
	do 
	{
		Usart_SendByte(port, *(str + k) );
		k++;
	} while(*(str + k)!='\0');

	/* 等待发送完成 */
	//while(USART_GetFlagStatus(uart_port[port], USART_FLAG_TC)==RESET);
}
/*****************  发送BUF **********************/
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

/********************UART 驱动end**************************/

/************************UART 设备相关***************************/
#if DEBUG


/**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 嵌套向量中断控制器组选择 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  /* 抢断优先级为1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  /* 子优先级为1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}


 /**
  * @brief  DEBUG_USART GPIO 配置,工作模式配置。115200 8-N-1 ，中断接收模式
  * @param  无
  * @retval 无
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
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
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

//读写
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

//Uart设备2
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

	/* 嵌套向量中断控制器组选择 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/* 配置USART为中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	/* 抢断优先级为1 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	/* 子优先级为1 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	/* 使能中断 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* 初始化配置NVIC */
	NVIC_Init(&NVIC_InitStructure);
}
#endif
void USART1_IRQHandler(void)
{
#ifdef MONITOR
	u8 ret;
	if (USART_GetITStatus((USART_TypeDef *)uart_port[DEBUG_USART], USART_IT_RXNE) != RESET) { //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		ret = USART_ReceiveData((USART_TypeDef *)uart_port[DEBUG_USART]);	//(DEBUG_USART->DR);  //读取接收到的数据
		if (!monitor.use_flag) {	//处理完一条命令才接下一条
			debug.receive_buf[debug.receive_count++] = ret;	//++后是长度，也是下一次的下标
			Usart_SendByte(DEBUG_USART, ret);	//打印出接收了什么
			if (0x0D == ret) {
				Usart_SendString(DEBUG_USART, "\r\n");	
				debug.receive_buf[debug.receive_count - 1] = '\0';	//把回车换成\0
				monitor.use_flag = 1;
				 BaseType_t pxHigherPriorityTaskWoken; 
			     u32 ulReturn; 
			     /* 进入临界段，临界段可以嵌套 */ 
			     ulReturn = taskENTER_CRITICAL_FROM_ISR(); 
			  
		         //释放二值信号量，发送接收到新数据标志，供前台程序查询 
		         xSemaphoreGiveFromISR(BinarySem_Handle,&pxHigherPriorityTaskWoken); 
			         
			     //portYIELD_FROM_ISR(pxHigherPriorityTaskWoken); //如果需要的话进行一次任务切换，系统会判断是否需要进行切换 
			     /* 退出临界段 */ 
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
	if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET) { //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		ret = USART_ReceiveData(UART4);	//(DEBUG_USART->DR);  //读取接收到的数据
		Usart_SendByte(UART_DEVICE2_USART, ret);	//打印出接收了什么
	}
}

