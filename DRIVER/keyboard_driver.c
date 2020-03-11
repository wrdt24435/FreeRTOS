/*ÿ��40msִ��һ�ΰ�����������ⰴ��
*
*/
#include "common.h"
#include "keyboard_driver.h"
#include "clk_timer.h"
#include "gpio.h"
#include "FreeRTOS.h" 
#include "task.h"

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




