#include "common.h"
#include "homeview.h"
#include "view.h"

static const RECTANGLE touch_xy[] = {
	{50, 400, 180, 600},
	{300, 400, 430, 600},
};

static void Touch_process(u32 xy, u32 flag)
{
	u16 x = xy & 0xFFFF;
	u16 y = xy >> 16;
	int select = -1;
	for (int i = 0; i < ARR_NUM(touch_xy); i++) {
		if (x >= touch_xy[i].x && y >= touch_xy[i].y
		&& x <= touch_xy[i].x_end && y <= touch_xy[i].y_end) {
			select = i;
			break;
		}
	}
	if (0 == select) {
		View_switch(POPOUT_INDEX, 0);
	}
	else if (1 == select) {
		View_switch(SETUP_INDEX, 0);
	}
}

void home_show(void)
{
	LCD_Clear(GRAY);
	LCD_Fill(100, 120, 300, 180, BRED);
	LCD_ShowString(150, 135, 100, 24, 24, "Homeview");
	LCD_Fill(50, 400, 180, 600, GBLUE);
	LCD_Fill(300, 400, 430, 600, MAGENTA);
}

int Homeview_process(MESSAGE msg, u32 arg1, u32 arg2)
{
	switch (msg) {
		case TOUCH_MSG:
			Touch_process(arg1, arg2);
			break;
		case SHOW_MSG:
			home_show();
			break;
		case KEY_MSG:
			break;
		default :
			return -1;
	}
	return 0;
}

void Homeview_create(u32 arg1)
{
	View_create(Homeview_process, 0);
	home_show();	//»­view	
}


