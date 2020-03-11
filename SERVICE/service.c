/*	系统层次: IC外设HARDWARE，外设功能driver: 机器功能:service
*	除了HARDWARE其他做到移植不用改动，service分开if(interface)是为了
*	区分接口和机器功能执行过程，方便修改。
*/
#include "common.h" 
#include "Service.h" 

#if DEBUG
#define SER_PRINTF PRINTF
#else
#define SER_PRINTF
#endif

int led_service(SERVICE_PACKET *sp);
int rtc_service(SERVICE_PACKET *sp);
int flash_service(SERVICE_PACKET *sp);
service service_core[TYPE_MAX + 1] = {
	led_service,
	rtc_service,
	flash_service,
	NULL,
};


void Service_init(void)
{
	/*service_core[LED_SERVICE] = led_service;
	service_core[RTC_SERVICE] = rtc_service;
	service_core[FLASH_SERVICE] = flash_service;*/
}

int submit_service(SERVICE_PACKET *sp)
{
	if(service_core[sp->type]) {
		return service_core[sp->type](sp);
	}
	else {
		ERR;
	}
	return -1;
}




