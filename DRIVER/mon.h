#ifndef _MON_H_
#define _MON_H_

#include "common.h"

//����1��mon��������ĸ���
//����2��mon������������飬�������ִ���ʽ�Ĳ�������argv[0]�ǲ���1
typedef int (*command_func)(int argc, char **argv);

typedef struct {
	command_func func;	//����
	const char *name;		//mon�ø��ִ����뺯��
}mon_list;


#endif
