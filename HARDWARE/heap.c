#include "common.h"
#include <string.h>
#include "heap.h"

#if DEBUG
#define HEAP_PRINTF PRINTF
#else
#define HEAP_PRINTF
#endif
#define ALIGN	4

char heap_addr[HEAP_SIZE];

#define memory20	&heap_addr[0]
#define memory40	&heap_addr[BYTE_20_SIZE]
#define memory128	&heap_addr[BYTE_20_SIZE + BYTE_40_SIZE]
#define memory512	&heap_addr[BYTE_20_SIZE + BYTE_40_SIZE + BYTE_128_SIZE]
#define HEAP_END	(void *)(heap_addr + HEAP_SIZE)
#define HEAP_START	(void *)(heap_addr)

typedef struct {
	void *start;
	void *free;
	unsigned short block_size;
	unsigned char total_num;
	unsigned char free_num;
	unsigned char mini;	//可用块最小值
}HEAP;

HEAP BYTE_20 = {
	.start = (void *)memory20,
	.free = (void *)memory20,
	.block_size = 20,
	.free_num = 5,
	.total_num = 5,
	.mini = 5,
};
HEAP BYTE_40 = {
	.start = memory40,
	.free = memory40,
	.block_size = 40,
	.free_num = 5,
	.total_num = 5,
	.mini = 5,
};
HEAP BYTE_128 = {
	.start = memory128,
	.free = memory128,
	.block_size = 128,
	.free_num = 5,
	.total_num = 5,
	.mini = 5,
};
HEAP BYTE_512 = {
	.start = memory512,
	.free = memory512,
	.block_size = 512,
	.free_num = 4,
	.total_num = 4,
	.mini = 4,
};


HEAP *sector_addr[] = {&BYTE_20, &BYTE_40, &BYTE_128, &BYTE_512, NULL};


void my_heap_init()
{
	int i;
	int max;
	int n = 0;
	void *temp;
	
	/*u32 uxAddress = (u32)sector_addr[0]->start;
	if((uxAddress & (ALIGN - 1)) != 0)	{	//四字节对齐
		uxAddress += (ALIGN - 1);
		uxAddress &= ~((u32)(ALIGN - 1));
		sector_addr[0]->start = sector_addr[0]->free = (void *)uxAddress;
		sector_addr[0]->total_num -= 1;
		sector_addr[0]->free_num -= 1;
		sector_addr[0]->mini -= 1;
	}*/
	for (i = 0; i < (ARR_NUM(sector_addr) - 1); i++) {	//sector_addr成员末尾是NULL，所以要-1
		//sector_addr[i]->free = sector_addr[i]->start &= ~((u32)(ALIGN - 1));	//对齐
		temp = sector_addr[i]->start;
		max = sector_addr[i]->total_num * sector_addr[i]->block_size;
		while (n < max) {
			*(void **)temp = (void *)((u32)temp + sector_addr[i]->block_size);
			temp = *(void **)temp;
			n += sector_addr[i]->block_size;
		}
		n = 0;
	}
}

void *my_malloc(int size)
{
	int i = 0;
	void *temp;
	while (size > sector_addr[i]->block_size) {
		i++;
		if (!sector_addr[i]) {
			HEAP_PRINTF("MM s err %d\r\n", i);
			return NULL;
		}
	}
	if (!sector_addr[i]->free_num) {
		HEAP_PRINTF("MM num error\r\n");
		return NULL;
	}
	temp = sector_addr[i]->free;
	sector_addr[i]->free = *(void **)temp;
	sector_addr[i]->free_num--;
	if (sector_addr[i]->free_num < sector_addr[i]->mini) {
		sector_addr[i]->mini = sector_addr[i]->free_num;
	}
	return temp;
}

void my_free(void *addr)
{
	int i = 1;
	void *temp;
	if (addr >= HEAP_END || addr < HEAP_START) {
		HEAP_PRINTF("free error3\r\n");
		return;
	}

	while (sector_addr[i]) {
		if (addr < sector_addr[i]->start) {
			break;
		}
		i++;
	}
	i--;
	if (sector_addr[i]->total_num == sector_addr[i]->free_num) {
		HEAP_PRINTF("free error2\r\n");
		return;
	}
	memset(addr, 0, sector_addr[i]->block_size);
	temp = sector_addr[i]->free;
	sector_addr[i]->free = addr;
	*(void **)addr = temp;
	sector_addr[i]->free_num++;
}

int heap_test() 
{
	int n = 0;
	HEAP_PRINTF("1: %x  2: %x  3: %x\r\n",(int)BYTE_20.start, (int)BYTE_40.start, (int)BYTE_128.start);
	my_heap_init();

	char *a = my_malloc(111);
	char *b = my_malloc(40);
	char *d = my_malloc(22);
	char *c = my_malloc(33);
	char *e = my_malloc(33);
	char *f = my_malloc(16);
	memset(a, 0x01, 128);
	memset(b, 0x07, 40);
	memset(c, 0x02, 40);
	memset(d, 0x03, 40);
	memset(e, 0x04, 40);
	memset(f, 0x0B, 20);
	
	for (n = 0; n < 3; n++)
		HEAP_PRINTF("free%d %x\n", n, sector_addr[n]->free);
	n = 0;
	while (n < ARR_NUM(heap_addr) ) {
		HEAP_PRINTF(" %d:%x",n/4,*((int *)&heap_addr[n]));
		n +=4;
	}
	HEAP_PRINTF("\r\n");
	my_free(a);
	my_free(b);
	my_free(c);
	my_free(d);
	my_free(f);
	
	n = 0;
	while (n < ARR_NUM(heap_addr) ) {
		HEAP_PRINTF(" %d:%x",n/4,*((int *)&heap_addr[n]));
		n +=4;
	}
	return 0;
}

int Heap_state(int argc, char **argv)
{
	int i = 0;
	while (sector_addr[i]) {
		HEAP_PRINTF("size:%d free:%d mini:%d\r\n",
		sector_addr[i]->block_size, sector_addr[i]->free_num,
		sector_addr[i]->mini);
		i++;
	}
	return 0;
}

/*int main()
{
	test();
}	*/

