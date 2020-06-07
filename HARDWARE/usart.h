#ifndef __USART_H
#define __USART_H
#include "common.h"

#ifdef red_printf
#define printf
#endif

#if DEBUG
	#define MONITOR
#endif
typedef enum {
	DEBUG_USART = 1,
	UART_DEVICE2_USART = 4,
}UART_DEVICE;


/*******************************************************/
#define DEBUG_USART_BAUDRATE                115200
#define DEBUG_USART_RX_AF                   GPIO_AF_USART1
#define DEBUG_USART_TX_AF                   GPIO_AF_USART1
/************************************************************/

#ifdef Uart_device2
#define UART_DEVICE2_USART_BAUDRATE                115200
#endif

typedef struct __debug_struct {
	//u8	flag;
	u8 	receive_buf[31];	/*0x1F*/
	//u16 receive_len;
	u16 receive_count;
}debug_struct;

#ifdef MONITOR
typedef struct {
	u8 buf[31];	
	u32 use_flag;
}MONITOR_STRUCT;

u32 mon_flag_rw(u32 flag);
u8 *get_mon_buf(void);
void reset_mon_buf(void);
#endif

//void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void Debug_USART_Config(void);
u8 *get_mon_buf(void);
void clean_mon_buf(void);


#endif


