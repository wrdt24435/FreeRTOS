#ifndef __FLASH_DRIVER_H
#define __FLASH_DRIVER_H 		

#include "service.h"
#include "w25q16.h"

//参数1: 数据存储区
//2: 开始读取的地址(24bit)
//3: 要读取的字节数(最大65535)
#define Flash_read W25QXX_Read;

typedef enum {
	PERIPHERAL_FLASH_READ,
	PERIPHERAL_FLASH_WRITE,
}FLASH_COMMAND;

typedef struct {
	u8 *buf;
	u16 len;
	u32 addr;
}FLASH_SERVICE_PACKET;



#endif

