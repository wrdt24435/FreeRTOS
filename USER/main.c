#include "common.h"
#include "init.h"  
#include "my_task.h"  
#include "FreeRTOS.h" 
#include "task.h"
#include "semphr.h"
#include "queue.h" 

static TaskHandle_t AppTaskCreate_Handle = NULL;
QueueHandle_t Test_Queue =NULL; 

static void AppTaskCreate(void *arg)
{
	taskENTER_CRITICAL();
	/* ���� BinarySem */
	task_create();
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

void main_thread(void *arg)
{
	char r_queue[12];
	View_init();
	LCD_ShowString(30,40,210,24,24,"Hello my STM pennel!");
	Test_Queue = xQueueCreate((UBaseType_t ) 4,(UBaseType_t ) 12);	//����4��ÿ����Ϣ��С12Byte
    if (NULL != Test_Queue) 
         printf("���� Test_Queue ��Ϣ���гɹ�!\r\n"); 
	while (1) {
 		BaseType_t xReturn = xQueueReceive( Test_Queue, &r_queue, portMAX_DELAY); 
		if (pdTRUE == xReturn) 
			printf("msg: %s\r\n addr: %x\r\n", r_queue, r_queue); 
		else 
			ERR;
		vTaskDelay(10);
	}

}

//dev�ϲ���master�Ķ���

