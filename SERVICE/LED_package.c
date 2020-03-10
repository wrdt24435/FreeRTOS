#include "common.h" 
#include "LED_package.h" 
#include "LED_service.h" 
#include "task.h" 

static TaskHandle_t LED0_Task_Handle = NULL;
static TaskHandle_t LED1_Task_Handle = NULL;


/*....
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


static void LED_0_Task(void* parameter)
{ 
	while (1) 
	{ 
		LED_switch_service(LED_0, 1);
		vTaskDelay(500); /* 延时500个tick */
		LED_switch_service(LED_0, 0);; 
		vTaskDelay(500); /* 延时500个tick */
	}
}
static void LED_1_Task(void* parameter)
{ 
	while (1) 
	{ 
		LED_switch_service(LED_1, 0);
		vTaskDelay(500); /* 延时500个tick */
		LED_switch_service(LED_1, 1);; 
		vTaskDelay(500); /* 延时500个tick */
	}
}


void LED_test_task()
{
	BaseType_t ret;
	taskENTER_CRITICAL(); //进入临界区
	ret = xTaskCreate(LED_0_Task, "led0", 128*4, NULL, 2, &LED0_Task_Handle);
	if (pdPASS != ret) {
		ERR;
		return -1; 
	}
	ret = xTaskCreate(LED_1_Task, "led1", 128*4, NULL, 2, &LED1_Task_Handle);
	if (pdPASS != ret) {
		ERR;
		return -1; 
	}
}

