#include "common.h"
#include "sys.h"
#include "gpio.h"
#include "keyboard_driver.h"

void enter_standby(void)
{
	//�ر���������
	Sys_Enter_Standby();
	key_init();
	//����������
}



