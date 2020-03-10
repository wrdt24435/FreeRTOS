#ifndef __FLASH_DRIVER_H
#define __FLASH_DRIVER_H 		

#include "service.h"
#include "w25q16.h"

//����1: ���ݴ洢��
//2: ��ʼ��ȡ�ĵ�ַ(24bit)
//3: Ҫ��ȡ���ֽ���(���65535)
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

