#include "common.h" 
#include "LED_package.h" 
#include "LED_service.h" 

#include "FreeRTOS.h" 
#include "task.h"


/*	简单服务可以跳过service，直接操作IO
*/
void LED_switch_service(LED_X LED, u32 state)
{
	SERVICE_PACKET sp = {0};
	LED_SERVICE_PACKET led_sp;
	led_sp.type = LED;
	led_sp.state = state;
	sp.buf = (void *)&led_sp;
	sp.type = LED_SERVICE;
	sp.command_id = LEDXXX;
	submit_service(&sp);
}


void LED_0_Task(void* parameter)
{ 
	while (1) 
	{ 
		LED_switch_service(LED_0, 1);
		vTaskDelay(500); /* 延时500个tick */
		LED_switch_service(LED_0, 0);; 
		vTaskDelay(500); /* 延时500个tick */
	}
}
void LED_1_Task(void* parameter)
{ 
	while (1) 
	{ 
		LED_switch_service(LED_1, 0);
		vTaskDelay(500); /* 延时500个tick */
		LED_switch_service(LED_1, 1);; 
		vTaskDelay(500); /* 延时500个tick */
	}
}


