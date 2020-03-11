#ifndef __COMMON_H
#define __COMMON_H 		

#include <stdio.h>

#define DEBUG 1
#define PRINTF	printf
#define ERR		PRINTF("ERR%s %d\r\n", __FILE__, __LINE__);

typedef enum {_DISABLE = 0, _ENABLE = !_DISABLE} MY_STATE;
typedef enum {TRUE = 0, FALSE = !_DISABLE} BOOL;

#define NULL 0
typedef unsigned int  	u32;
typedef unsigned short 	u16;
typedef unsigned char  	u8;

#define ARR_NUM(x) (sizeof(x)/sizeof(x[0]))	//�����Ա����


#define BIT_1	(1)
#define BIT_2	(1<<1)
#define BIT_3	(1<<2)
#define BIT_4	(1<<3)
#define BIT_5	(1<<4)
#define BIT_6	(1<<5)
#define BIT_7	(1<<6)
#define BIT_8	(1<<7)
#define BIT_9	(1<<8)
#define BIT_10	(1<<9)
#define BIT_11	(1<<10)
#define BIT_12	(1<<11)
#define BIT_13	(1<<12)
#define BIT_14	(1<<13)
#define BIT_15	(1<<14)
#define BIT_16	(1<<15)
#define BIT_17	(1<<16)
#define BIT_18	(1<<17)
#define BIT_19	(1<<18)
#define BIT_20	(1<<19)
#define BIT_21	(1<<20)
#define BIT_22	(1<<21)
#define BIT_23	(1<<22)
#define BIT_24	(1<<23)
#define BIT_25	(1<<24)
#define BIT_26	(1<<25)
#define BIT_27	(1<<26)
#define BIT_28	(1<<27)
#define BIT_29	(1<<28)
#define BIT_30	(1<<29)
#define BIT_31	(1<<30)
#define BIT_32	(1<<31)

#define _SET_BIT(x,y) (x |= y) //��λ
#define _CLR_BIT(x,y) (x &= (~y))) //����
#define _CHECK_BIT(x,y) (x & y) //���

void delay_us(u32 count);
#include "heap.h"

#endif

