#include "common.h"
#include "sys.h"
#include "gpio.h"
#include "keyboard_driver.h"

void enter_standby(void)
{
	//关闭所有外设
	Sys_Enter_Standby();
	key_init();
	//打开所有外设
}



