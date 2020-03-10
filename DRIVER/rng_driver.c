#include "common.h"
#include "sys.h"

u32 get_random_number(void)
{	   
	return RNG_Get_RandomNum();	
}

//生成[min,max]范围的随机数
int RNG_Get_RandomRange(int min,int max)
{ 
   return get_random_number()%(max-min+1) +min;
}


