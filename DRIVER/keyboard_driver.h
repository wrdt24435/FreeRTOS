#ifndef __KEYBOARD_DRIVER_H
#define __KEYBOARD_DRIVER_H

typedef enum {
	KEYBOARD_EVENT1,
	KEYBOARD_EVENT2,
	KEYBOARD_EVENT3,

	KEYBOARD_EVENTMAX,
}KEYBOARD_EVENT;

void key_init(void);
void key_task_init(void);
void insert_task(TICK_TASK *task);


#endif

