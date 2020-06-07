#include "common.h"
#include "popout.h"
#include "view.h"

static const RECTANGLE touch_xy[] = {
	{200, 280, 300, 480},
};

static void Touch_process(u32 xy, u32 flag)
{
	u16 x = xy & 0xFFFF;
	u16 y = xy >> 16;
	int select;
	for (int i = 0; i < ARR_NUM(touch_xy); i++) {
		if (x >= touch_xy[i].x && y >= touch_xy[i].y
		&& x<= touch_xy[i].x_end && y <= touch_xy[i].y_end) {
			select = i;
			break;
		}
	}
	if (0 == select) {
		View_destroy();
	}
}

void Popout_show(void)
{
	LCD_Fill(200, 280, 300, 480, CYAN);
	LCD_ShowString(220, 350, 70, 24, 24, "Popout");
}

int Popout_process(MESSAGE msg, u32 arg1, u32 arg2)
{
	switch (msg) {
		case TOUCH_MSG:
			Touch_process(arg1, arg2);
			break;
		case SHOW_MSG:
			Popout_show();
			break;
		case KEY_MSG:
			break;
		default :
			return -1;
	}
	return 0;
}

void Popout_create(u32 arg1)
{
	View_create(Popout_process, 1);
	Popout_show();	//»­view	
}


