#ifndef __KEYBOARD_DRIVER_H
#define __KEYBOARD_DRIVER_H

//测试git，看到删掉

typedef enum {
	KEYBOARD_EVENT1,
	KEYBOARD_EVENT2,
	KEYBOARD_EVENT3,

	KEYBOARD_EVENTMAX,
}KEYBOARD_EVENT;

void key_init(void);
void key1_task(void *arg);
void key0_task(void *arg);




#endif

