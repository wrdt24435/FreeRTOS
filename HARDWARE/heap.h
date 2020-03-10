#ifndef _HEAD_H_
#define _HEAD_H_

#define BYTE_20_NUM	5
#define BYTE_40_NUM	5
#define BYTE_128_NUM 5
#define BYTE_512_NUM 4

#define BYTE_20_SIZE	(20 * BYTE_20_NUM)
#define BYTE_40_SIZE	(40 * BYTE_40_NUM)
#define BYTE_128_SIZE	(128 * BYTE_128_NUM)
#define BYTE_512_SIZE	(512 * BYTE_512_NUM)
#define HEAP_SIZE	(BYTE_20_SIZE + BYTE_40_SIZE + BYTE_128_SIZE + BYTE_512_SIZE)

typedef struct {
	void *start;
	void *free;
	u16 free_num;
	u16 total_num;
	u8 block_size;
}DYNAMIC_MEMORY;

void *my_malloc(int size);
void my_free(void *addr);
void my_heap_init(void);



#endif
