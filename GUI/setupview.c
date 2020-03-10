#include "common.h"
#include "Setupview.h"
#include "view.h"

static const RECTANGLE touch_xy[] = {
	{220, 300, 380, 450},
	{220, 500, 380, 650},
};

void Touch_process(u32 xy, u32 flag)
{
	u16 x = xy & 0xFFFF;
	u16 y = xy >> 16;
	int select;
	for (int i = 0; i < ARR_NUM(touch_xy); i++) {
		if (x >= touch_xy[i].x && y >= touch_xy[i].y
		&& x <= touch_xy[i].x_end && y <= touch_xy[i].y_end) {
			select = i;
			break;
		}
	}
	if (0 == select) {
		View_switch(HOME_INDEX, 0);
	}
	if (1 == select) {
		View_switch(POPOUT_INDEX, 0);
	}
}

void Setup_show(void)
{
	LCD_Clear(DARKBLUE);
	LCD_Fill(100, 120, 300, 180, GRED);
	LCD_ShowString(150, 135, 100, 24, 24, "Setup");
	LCD_Fill(220, 300, 380, 450, BROWN);
	LCD_Fill(220, 500, 380, 650, BLUE);
}

int Setupview_process(MESSAGE msg, u32 arg1, u32 arg2)
{
	switch (msg) {
		case TOUCH_MSG:
			Touch_process(arg1, arg2);
			break;
		case SHOW_MSG:
			Setup_show();
			break;
		case KEY_MSG:
			break;
		default :
			return -1;
	}
	return 0;
}

void Setupview_create(u32 arg1)
{
	View_create(Setupview_process, 0);
	Setup_show();	//»­view	
}


