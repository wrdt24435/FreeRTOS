#include "mon.h"
#include <string.h>
#include "usart.h"
#include "FreeRTOS.h" 
#include "semphr.h"
#include "lcd.h"
#include "timers.h"

u8 mon_flag;
SemaphoreHandle_t BinarySem_Handle = NULL;

int Mon_hello(int argc, char **argv);
//int Heap_state(int argc, char **argv);
int send_touch(int argc, char **argv);
int background(int argc, char **argv);
int soft_timer(int argc, char **argv);


mon_list command_list[] = {
	{Mon_hello, "hello"},
	{background, "bk"},
	{soft_timer, "timer1"},
	//{Heap_state, "memory"},
	{send_touch, "touch"},
	{NULL,NULL}
};
	


//转换0~9之间的字符串
int to_int(const char *str)
{
	u32 sum = 0;
	while (*str != '\0') {
		if (*str >= '0' && *str <= '9') {
			sum = sum*10 + *str - '0';
		}
		else {
			ERR;
			return -1;
		}
		str++;
	}
	return sum;
}

int Mon_hello(int argc, char **argv)
{
	PRINTF("hello monitor!\r\n");
	if (argc) {
		while (argc--) {
			PRINTF("参数%d: %s\r\n",argc, argv[argc]);
		}
	}
	return 0;
}

TimerHandle_t Swtmr1_Handle = NULL;
void test_timer1(void *arg)
{
	static u32 count = 0;
	printf("time1 in %d\r\n", count);
	if (5 <= count++) {
		count = 0;
		xTimerStop(Swtmr1_Handle,0);
	}
}
int soft_timer(int argc, char **argv)
{
	Swtmr1_Handle=xTimerCreate((const char*)"AutoReloadTimer", 
							(TickType_t)2000,/*定时器周期 1000(tick) */ 
							(UBaseType_t)pdTRUE,/* 周期模式 */ 
							(void*)1,/*为每个计时器分配一个索引的唯一 ID */ 
							(TimerCallbackFunction_t)test_timer1); 
	if (Swtmr1_Handle != NULL) { 
	/*************************************************************** 
	* xTicksToWait:如果在调用 xTimerStart()时队列已满，则以 tick 为单位指定调用任务应保持 
	* 在 Blocked(阻塞)状态以等待 start 命令成功发送到 timer 命令队列的时间。 
	* 如果在启动调度程序之前调用 xTimerStart()，则忽略 xTicksToWait。在这里设置等待时间为 0 
	***********************************************************/ 

		xTimerStart(Swtmr1_Handle, 0);  //开启周期定时器 
	}
	return 0;
}

int background(int argc, char **argv)
{
	if (argc) {
		if (!strncmp("on", argv[0], 2)) {	
			LCD_LED(BG_ON);
		}
		else if (!strncmp("off", argv[0], 3)) {
			LCD_LED(BG_OFF);
		}
		else {
			PRINTF("參數 err\r\n");
		}
	}
	return 0;
}

#include "view.h"
int send_touch(int argc, char **argv)
{
	if (argc == 2) {
		Send_message(TOUCH_MSG , to_int(argv[0]), 0);
	}
	else {
		ERR;
	}
	return 0;
}

void set_mon_flag(void)
{
	mon_flag = 1;
}

//找(返回)字符串下标到某个字符的长度
int search_len(u8 *buf, int subscripts, char ch)
{
	int start = subscripts;
	for(;(buf[subscripts] != '\0') && (buf[subscripts] != ch); subscripts++);
	return ch == buf[subscripts]? subscripts - start : -1;
}

//返回命令函数
//len : 命令长度
command_func search_command(u8 *buf, int len)
{
	int n = 0;
	command_func func = NULL;
	if (len) {
		while (command_list[n].func) {
			if (!strncmp(command_list[n].name, (const char *)buf, len)) {	
				func = command_list[n].func;
				break;
			}
			n++;
		}
	}
	return func;
}

//参数在monitor.buf里，将参数后面的空格和回车换成\0，
//参数字符串的起始下标就能成为参数
//参数2是数组下标
int search_arg(u8 *buf, int subscripts, char **argv)
{
	int num = 0;	//下标比数量少1
	int len = search_len(buf, subscripts, ' ');
	while (len != -1) {
		if (num < 4) {
			buf[subscripts + len] = '\0';	//把空格换成\0
			argv[num++] = (char *)&buf[subscripts];
			subscripts = subscripts + len + 1;
			len = search_len(buf, subscripts, ' ');
		}
		else {
			PRINTF("arg too much\r\n");
		}
	}
	argv[num] = (char *)&buf[subscripts];	//最后一个参数以\0结束
	return ++num;
}

/*//argc: 参数个数
//argv: 参数字串数组，成员是参数字串地址
command_func search_command_and_arg(u8 *buf, int *argc, char **argv)
{
	int count = 0;
	command_func func;
	*argc = 0;
	func = search_command(buf, &count);
	if (' ' == buf[count]) {	//说明有参数
		search_arg(buf, count++, argv);	//++后是参数下标
	}		
}*/

void command_handler(u8 *buf)
{
	int argc = 0;
	char *argv[4];	//最多只有4个参数
	//int count = 0;
	command_func func;
	int len = search_len(buf, 0, ' ');
	if(len == -1) {
		func = search_command(buf, strlen((const char *)buf));	//没有参数
	}
	else {	//说明有参数
		func = search_command(buf, len);
		if (func) {
			argc = search_arg(buf, ++len, argv);	//++后是参数下标
		}
	}
	if (func) {
		func(argc, argv);
	}
	else {
		PRINTF("connot found command!\r\n");
	}
}

void Mon_task(void* arg)
{
	BinarySem_Handle = xSemaphoreCreateBinary();	 
	if(NULL != BinarySem_Handle)
		printf("BinarySem_Handle二值信号量创建成功!\r\n");
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
	while (1) {
	    //获取二值信号量 xSemaphore,没获取到则一直等待
	 	xReturn = xSemaphoreTake(BinarySem_Handle, portMAX_DELAY);
	    if(pdTRUE == xReturn) {
			u8 *buf = get_mon_buf();
			mon_flag_rw(1);
			command_handler(buf);
			reset_mon_buf();
	    }
	}
}
