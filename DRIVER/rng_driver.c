#include "common.h"
#include "sys.h"

u32 get_random_number(void)
{	   
	return RNG_Get_RandomNum();	
}

//����[min,max]��Χ�������
int RNG_Get_RandomRange(int min,int max)
{ 
   return get_random_number()%(max-min+1) +min;
}


