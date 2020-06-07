#ifndef __SERVICE_H
#define __SERVICE_H

typedef enum {
	//LED_SERVICE,
	//RTC_SERVICE,
	//FLASH_SERVICE,

	TYPE_MAX = 1,
}SERVICE_TYUPE;

typedef struct {
	void *buf;
	SERVICE_TYUPE type;
	u8 command_id;
}SERVICE_PACKET;

typedef int (*service)(SERVICE_PACKET *);

void Service_init(void);
int submit_service(SERVICE_PACKET *sp);



#endif
