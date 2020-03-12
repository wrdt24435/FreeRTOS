#include "common.h"
#include "LED_service.h"
#include "gpio.h"

#define LED0(x)	io_set_out(LED_0, x)
#define LED1(x)	io_set_out(LED_1, x)

void LED_switch(u32 LED_num, u32 state)
{
	switch(LED_num) {
		case LED_0:
			LED0(state);
			break;
		case LED_1:
			LED1(state);
			break;
		default:
			break;
	}			   //ÑÓÊ±300ms
}

int led_service(SERVICE_PACKET *sp)
{
	LED_SERVICE_PACKET *led_sp = (LED_SERVICE_PACKET *)sp->buf;
	switch(sp->command_id)
	{
		case LEDXXX:
			LED_switch(led_sp->type, led_sp->state);
			break;
		default :
			break;
	}
	return 0;
}

void LED_Init(void)
{    	
	MY_IO *gpio_config = &temp_config;
	gpio_config->port = LED_0_PORT;										   
	gpio_config->sys.GPIO_Pin = LED_1_PIN | LED_0_PIN;	
	gpio_config->sys.GPIO_Mode = GPIO_Mode_OUT;   
	gpio_config->sys.GPIO_OType = GPIO_OType_PP;
	gpio_config->sys.GPIO_PuPd = GPIO_PuPd_UP; 
	gpio_config->sys.GPIO_Speed = GPIO_Speed_2MHz; 
	gpio_init(gpio_config);	
	
}

void LED_test(void)
{
	LED_switch(LED_0, 1);
	LED_switch(LED_1, 0);
	//delay_ms(500);			   
	LED_switch(LED_1, 1);
	LED_switch(LED_0, 0);
	//delay_ms(500);					 
}

