/*	view ֻ����ʾ�����Թ�����APP�ļ�ʵ�֣�
*	ͨ��call_back���õ�APP����
*/
#include "common.h"
#include "view.h"

#include "homeview.h"
#include "setupview.h"
#include "popout.h"

PROCESS process_contianer[3];	//���֧������view
u8 view_layer = 0;
CALL_BACK call_back_func;

static const CREATE_VIEW view_table[] = {	//��VIEW_INDEX��Ӧ
	Homeview_create,
	Setupview_create,
	Popout_create,
};
void View_init()
{
	Homeview_create(0);	
}

int Send_message(MESSAGE msg, u32 arg1, u32 arg2)
{
	process_contianer[view_layer](msg, arg1, arg2);
	return 0;
}

int View_switch(VIEW_INDEX index, u32 arg1)
{
	if (index <= MAIN_VIEW) {
		Send_message(DESTROY_MSG, 0, 0);
		view_layer = 0;
	}
	view_table[index](arg1);
	return 0;
}

//mode : �Ƿ�����һ��
void View_create(PROCESS proc, u32 mode)
{
	if (mode) {
		view_layer++;
	}
	process_contianer[view_layer] = proc;
}
void View_destroy()
{
	if (view_layer) {
		view_layer--;
		Send_message(SHOW_MSG, 0, 0);
	}
}

//ͨ��call_back��������APP�ĺ�����call_back������APP����
int App_call_back(u32 arg1, u32 arg2)
{
	call_back_func(arg1, arg2);
	return 0;
}

int set_call_back(CALL_BACK app)
{
	call_back_func = app;
	return 0;
}


