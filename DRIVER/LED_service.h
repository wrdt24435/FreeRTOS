#ifndef __LED_DRIVER_H
#define __LED_DRIVER_H

#include "service.h"

typedef enum {
	LED_0,
	LED_1,
}LED_X;

typedef enum {
	LEDXXX,
}LED_COMMAND;


typedef struct {
	LED_X type;
	u8 state;
}LED_SERVICE_PACKET;


void LED_Init(void);

#endif

