#ifndef _MON_H_
#define _MON_H_

#include "common.h"

typedef int (*command_func)(int argc, char **argv);

typedef struct {
	command_func func;
	const char *name;
}mon_list;


#endif
