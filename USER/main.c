#include "common.h"
#include "init.h"  
#include "my_task.h"  
#include "FreeRTOS.h" 
#include "task.h"

static TaskHandle_t AppTaskCreate_Handle = NULL;

static void AppTaskCreate(void *arg)
{
	taskENTER_CRITICAL();
	LED_test_task();
	keyboard_task();
	vTaskDelete(AppTaskCreate_Handle); //ɾ��AppTaskCreate����
	taskEXIT_CRITICAL(); //�˳��ٽ���
}

void BSP_Init(void)
{ 
	//HSE_SetSysClock(8, 336, 2, 7);			
	//SysTick_init();
	Debug_USART_Config();
	LED_Init();
	key_init();
	//my_heap_init();
	LCD_Init();
	rtc_init();
	//WWDG_Init();
	RNG_Init();
	W25QXX_Init();
}

int main(void)
{ 
	BSP_Init();		        //��ʼ��LED�˿�
	View_init();
	LCD_ShowString(30,40,210,24,24,"Hello my STM pennel!");
	PRINTF("RTOS \r\n");

	BaseType_t ret;
	ret = xTaskCreate(AppTaskCreate, "tc", 128*4, NULL, 2, &AppTaskCreate_Handle);
	if (pdPASS == ret) 
		vTaskStartScheduler(); /* �������񣬿������� */ 
	else {
		ERR;
		return -1; 
	}
	while(1);
}
