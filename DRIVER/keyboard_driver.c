/*ÿ��40msִ��һ�ΰ�����������ⰴ��
*
*/
#include "common.h"
#include "keyboard_driver.h"
#include "clk_timer.h"
#include "gpio.h"

#if DEBUG
#define KEY_PRINTF PRINTF
#else
#define KEY_PRINTF
#endif

#define FIRST_TIME 	20	//25*������
#define PRESS_TIME 	4	//4*������

//�����Ż��ɽṹ��
u8 key_1_status = 0;	//��𰴼�1����(bit1)����ס(bit2)
u8 key_1_count = 0;		//��¼����סԼ1s����밴ס״̬
u8 key_1_press = FIRST_TIME;		//25�ǵ�һ�ν��밴ס״̬�������ÿ��200ms���Ͱ�ס״̬
u8 key_0_status = 0;	//��𰴼�1����(bit1)����ס(bit2)
u8 key_0_count = 0;		//��¼����סԼ1s����밴ס״̬
u8 key_0_press = FIRST_TIME;		//25�ǵ�һ�ν��밴ס״̬�������ÿ��200ms���Ͱ�ס״̬
/*TICK_TASK key_1_task = {
	.next = NULL,
	.task = key1_task,
	.arg = NULL,
	.time = 0,
	.timing = 4,
	.interval = INTERVAL_10MS,
	.flag = 0,
};
TICK_TASK key_0_task = {
	.next = NULL,
	.task = key0_task,
	.arg = NULL,
	.time = 0,
	.timing = 4,
	.interval = INTERVAL_10MS,
	.flag = 0,
};*/
static TaskHandle_t key_0_Task_Handle = NULL;
static TaskHandle_t key_1_Task_Handle = NULL;



void key_init(void)	
{
	MY_IO *gpio_config = &temp_config;
	gpio_config->port = KEY_0_PORT;
	gpio_config->sys.GPIO_Mode = GPIO_Mode_IN;
	gpio_config->sys.GPIO_OType = GPIO_OType_PP;
	gpio_config->sys.GPIO_Pin = KEY_PIN;
	gpio_config->sys.GPIO_PuPd = GPIO_PuPd_UP;
	gpio_config->sys.GPIO_Speed = GPIO_Low_Speed;
	gpio_init(gpio_config); //������Ӧ��IO�ڳ�ʼ��
	gpio_config->port = KEY_UP_PORT;
	gpio_config->sys.GPIO_Pin = KEY_UP_PIN;
	gpio_config->sys.GPIO_PuPd = GPIO_PuPd_DOWN;
	gpio_init(gpio_config);
	io_interrupt_init();
}

void key1_task(void *arg)
{
	if (io_read(KEY_1)) {
		if (key_1_status) {
			KEY_PRINTF("KEY1 UP\r\n");
			key_1_status = 0;
			key_1_count = 0;
			key_1_press = FIRST_TIME;
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

void key0_task(void *arg)
{
	if (io_read(KEY_0)) {
		if (key_0_status) {
			KEY_PRINTF("KEY0 UP\r\n");
			key_0_status = 0;
			key_0_count = 0;
			key_0_press = 25;
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


