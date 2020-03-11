#include "common.h" 
#include "LED_package.h" 
#include "keyboard_driver.h"

#include "FreeRTOS.h" 
#include "task.h"

static TaskHandle_t LED0_Task_Handle = NULL;
static TaskHandle_t LED1_Task_Handle = NULL;
static TaskHandle_t key_0_Task_Handle = NULL;
static TaskHandle_t key_1_Task_Handle = NULL;




void LED_test_task()
{
	BaseType_t ret;
	taskENTER_CRITICAL(); //进入临界区
	ret = xTaskCreate(LED_0_Task, "led0", 128, NULL, 2, &LED0_Task_Handle);
	if (pdPASS != ret) {
		ERR;
		return; 
	}
	ret = xTaskCreate(LED_1_Task, "led1", 128, NULL, 2, &LED1_Task_Handle);
	if (pdPASS != ret) {
		ERR;
		return; 
	}
}

void keyboard_task()
{
	BaseType_t ret;
	ret = xTaskCreate(key0_task, "key_0", 128, NULL, 7, &key_0_Task_Handle);
	if (pdPASS != ret) {
		ERR;
		return; 
	}
	ret = xTaskCreate(key1_task, "key_1", 128, NULL, 7, &key_1_Task_Handle);
	if (pdPASS != ret) {
		ERR;
		return; 
	}
}

