/*每隔40ms执行一次按键任务来检测按键
*
*/
#include "common.h"
#include "keyboard_driver.h"
#include "clk_timer.h"
#include "gpio.h"
#include "FreeRTOS.h" 
#include "semphr.h"
#include "task.h"
#include "queue.h"

#if DEBUG
#define KEY_PRINTF PRINTF
#else
#define KEY_PRINTF
#endif

#define FIRST_TIME 	20	//25*任务间隔
#define PRESS_TIME 	4	//4*任务间隔

//可以优化成结构体
u8 key_1_status = 0;	//辨别按键1按下(bit1)、按住(bit2)
u8 key_1_count = 0;		//记录按下住约1s后进入按住状态
u8 key_1_press = FIRST_TIME;		//25是第一次进入按住状态，进入后每隔200ms发送按住状态
u8 key_0_status = 0;	//辨别按键1按下(bit1)、按住(bit2)
u8 key_0_count = 0;		//记录按下住约1s后进入按住状态
u8 key_0_press = FIRST_TIME;		//25是第一次进入按住状态，进入后每隔200ms发送按住状态

SemaphoreHandle_t MuxSem_Handle =NULL; 
int global = 0;

void key_init(void)	
{
	MY_IO *gpio_config = &temp_config;
	gpio_config->port = KEY_0_PORT;
	gpio_config->sys.GPIO_Mode = GPIO_Mode_IN;
	gpio_config->sys.GPIO_OType = GPIO_OType_PP;
	gpio_config->sys.GPIO_Pin = KEY_PIN;
	gpio_config->sys.GPIO_PuPd = GPIO_PuPd_UP;
	gpio_config->sys.GPIO_Speed = GPIO_Low_Speed;
	gpio_init(gpio_config); //按键对应的IO口初始化
	gpio_config->port = KEY_UP_PORT;
	gpio_config->sys.GPIO_Pin = KEY_UP_PIN;
	gpio_config->sys.GPIO_PuPd = GPIO_PuPd_DOWN;
	gpio_init(gpio_config);
	io_interrupt_init();
}

void key1_task(void *arg)
{
	while (1) {
		if (io_read(KEY_1)) {
			if (key_1_status) {
				KEY_PRINTF("KEY1 UP\r\n");
				xSemaphoreTake(MuxSem_Handle,portMAX_DELAY);
				KEY_PRINTF("KEY1 lock\r\n");
				key_1_status = 0;
				key_1_count = 0;
				key_1_press = FIRST_TIME;
				vTaskDelay(5000);
				xSemaphoreGive( MuxSem_Handle );
				printf("key1 release\r\n");
			}
		}
		else {//if (key_status & 0x01) {
			if (!_CHECK_BIT(key_1_status, BIT_1)) {
				KEY_PRINTF("KEY1 DOWN\r\n");
				_SET_BIT(key_1_status, BIT_1);
			}
			else {
				if (key_1_count++ > key_1_press) {
					KEY_PRINTF("KEY1 PRESS ON\r\n");
					key_1_press = PRESS_TIME;
					key_1_count = 0;
				}
			}
		}
		vTaskDelay(50);
	}
}

char *msg = "key0 msg\r\n";
extern QueueHandle_t Test_Queue;
void key0_task(void *arg)
{
	MuxSem_Handle = xSemaphoreCreateMutex(); 
    if (NULL != MuxSem_Handle) 
         printf("MuxSem_Handle 互斥量创建成功!\r\n"); 
	printf("msg addr: %x",msg);
	while (1) {
		if (io_read(KEY_0)) {
			if (key_0_status) {
				KEY_PRINTF("KEY0 UP\r\n");
				xQueueSend(Test_Queue, msg, 0);
				xSemaphoreTake(MuxSem_Handle,portMAX_DELAY);
				KEY_PRINTF("KEY0 lock\r\n");
				key_0_status = 0;
				key_0_count = 0;
				key_0_press = FIRST_TIME;
				vTaskDelay(5000);
				xSemaphoreGive( MuxSem_Handle );
				printf("key0 release\r\n");
			}
		}
		else {
			if (!_CHECK_BIT(key_0_status, BIT_1)) {
				KEY_PRINTF("KEY0 DOWN\r\n");
				_SET_BIT(key_0_status, BIT_1);
			}
			else {
				if (key_0_count++ > key_0_press) {
					KEY_PRINTF("KEY0 PRESS ON\r\n");
					key_0_press = 200 / 40;
					key_0_count = 0;
				}
			}
		}
		vTaskDelay(50);
	}
}




