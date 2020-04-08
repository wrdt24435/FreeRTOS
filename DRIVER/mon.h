#ifndef _MON_H_
#define _MON_H_

#include "common.h"

//参数1：mon输入参数的个数
//参数2：mon输入参数的数组，里面是字串形式的参数，如argv[0]是参数1
typedef int (*command_func)(int argc, char **argv);

typedef struct {
	command_func func;	//函数
	const char *name;		//mon敲该字串进入函数
}mon_list;


#endif
