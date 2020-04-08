#include "common.h"
#include "uart_device2.h"
#include <stdio.h>
#include <stdarg.h>

void d2_init()
{
	UART_DEVICE2_OPEN();
}

void printf2(char* fmt, ...)
{
	u8 i,len;
	va_list ap;
	u8 putBuf[256];
	va_start(ap, fmt);
	len = vsprintf((char*)putBuf, fmt, ap); 
	va_end(ap);
	Usart_Send_buf(UART_DEVICE2_USART, putBuf, len);	//使用uart驱动接口，传设备号
}



