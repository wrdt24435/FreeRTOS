#ifndef __VIEW_H
#define __VIEW_H 		

#include "lcd.h"

typedef enum {
	SHOW_MSG,
	DESTROY_MSG,
	KEY_MSG,
	TOUCH_MSG,
}MESSAGE;

typedef enum {
	HOME_INDEX,
	SETUP_INDEX,

	MAIN_VIEW = SETUP_INDEX,	//一级View
	
	POPOUT_INDEX,	//次级View
}VIEW_INDEX;


typedef struct {
	int x;
	int y;
	int x_end;
	int y_end;
}RECTANGLE;


typedef int (*PROCESS)(MESSAGE msg, u32 arg1, u32 arg2);
typedef int (*CALL_BACK)(u32 arg1, u32 arg2);
typedef void (*CREATE_VIEW)(u32 arg1);


void View_init(void);
int View_switch(VIEW_INDEX index, u32 arg1);
int Send_message(MESSAGE msg, u32 arg1, u32 arg2);
void View_create(PROCESS proc, u32 mode);
void View_destroy(void);



#endif

