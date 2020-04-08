#ifndef __FLASH_DRIVER_H
#define __FLASH_DRIVER_H 		

#include "service.h"
#include "w25q16.h"


typedef enum {
	PERIPHERAL_FLASH_READ,
	PERIPHERAL_FLASH_WRITE,
}FLASH_COMMAND;

/*typedef struct {
	u8 *buf;
	u16 len;
	u32 addr;
}FLASH_SERVICE_PACKET;*/



#endif

