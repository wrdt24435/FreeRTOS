#include "common.h"
#include "init.h"  
#include "FreeRTOS.h" 
#include "task.h"

static TaskHandle_t AppTaskCreate_Handle = NULL;

static void AppTaskCreate(void)
{
	taskENTER_CRITICAL();
	LED_test_task()
	vTaskDelete(AppTaskCreate_Handle); //删除AppTaskCreate任务
	taskEXIT_CRITICAL(); //退出临界区
}

void BSP_Init(void)
{ 
	HSE_SetSysClock(8, 336, 2, 7);			
	SysTick_init();
	Debug_USART_Config();
	LED_Init();
	key_init();
	my_heap_init();
	LCD_Init();
	rtc_init();
	//WWDG_Init();
	RNG_Init();
	W25QXX_Init();
}
vTaskStartScheduler(

int main(void)
{ 
	BSP_Init();		        //初始化LED端口
	View_init();
	LCD_ShowString(30,40,210,24,24,"Hello my STM pennel!");
	PRINTF("TICK_TASK size %d\r\n", sizeof(TICK_TASK));

	BaseType_t ret;
	ret = xTaskCreate(AppTaskCreate, "tc", 128*4, NULL, 2, &AppTaskCreate_Handle);
	if (pdPASS == ret) 
		vTaskStartScheduler(); /* 启动任务，开启调度 */ 
	else {
		ERR;
		return -1; 
	}
	while(1);
}
