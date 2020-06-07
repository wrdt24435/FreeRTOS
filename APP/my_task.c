#include "common.h" 
#include "keyboard_driver.h"
#include "LED_service_if.h" 

#include "FreeRTOS.h" 
#include "task.h"

extern void main_thread(void *arg);
extern void Mon_task(void* arg);
extern void LED_0_Task(void *arg);

static TaskHandle_t LED0_Task_Handle = NULL;
//static TaskHandle_t LED1_Task_Handle = NULL;
static TaskHandle_t key_0_Task_Handle = NULL;
static TaskHandle_t key_1_Task_Handle = NULL;
static TaskHandle_t main_Task_Handle = NULL;
static TaskHandle_t mon_Task_Handle = NULL;

void task_create()
{
	BaseType_t ret;
	ret = xTaskCreate(main_thread, "main", 128, NULL, 0, &main_Task_Handle);
	if (pdPASS != ret) {
		ERR;
	}
	
	ret = xTaskCreate(key0_task, "key_0", 128, NULL, 6, &key_0_Task_Handle);
	if (pdPASS != ret) {
		ERR;
	}
	
	ret = xTaskCreate(key1_task, "key_1", 128, NULL, 5, &key_1_Task_Handle);
	if (pdPASS != ret) {
		ERR;
	}
	
	ret = xTaskCreate(LED_0_Task, "led0", 128, NULL, 2, &LED0_Task_Handle);
	if (pdPASS != ret) {
		ERR;
	}

	ret = xTaskCreate(Mon_task, "mon", 128, NULL, 2, &mon_Task_Handle);
	if (pdPASS != ret) {
		ERR;
	}
}


void LED_0_Task(void *arg)
{
	while (1) {
		LED_switch_service(LED_0, 0);
		vTaskDelay(500);
		LED_switch_service(LED_0, 1);
		vTaskDelay(500);
	}
}





