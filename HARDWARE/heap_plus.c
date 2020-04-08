#include <stdio.h>
#include "string.h"

#define PRINTF printf
#define ERR		PRINTF("ERR %d\r\n", __LINE__);
#define ARR_NUM(x) (sizeof(x)/sizeof(x[0]))

#define size_t	unsigned int

#define HEAP_SIZE 512
#define heapMINIMUM_BLOCK_SIZE	( ( size_t ) ( xHeapStructSize << 1 ) )
#define portBYTE_ALIGNMENT			4
#define portBYTE_ALIGNMENT_MASK	( portBYTE_ALIGNMENT - 1 )
#define xHeapStructSize		sizeof( BlockLink_t )
#define xBlockAllocatedBit  (1 << 31)	//��ʹ�õ��ڴ�����λ��1

#include <time.h>
void delay(int seconds)
{
	clock_t start = clock();
	clock_t lay = (clock_t)seconds * CLOCKS_PER_SEC;
	while ((clock()-start) < lay);
}


typedef struct A_BLOCK_LINK
{
	struct A_BLOCK_LINK *pxNextFreeBlock;	/*<< The next free block in the list. */
	unsigned int xBlockSize;	//�ڴ���С�����λ��������Ƿ�ʹ��
} BlockLink_t;
static void prvInsertBlockIntoFreeList( BlockLink_t *pxBlockToInsert );


static unsigned char Heap[HEAP_SIZE];
static BlockLink_t xStart;	//�����ڴ������ͷ
static BlockLink_t	*pxEnd = NULL;	//����ָ���ڴ��Heap[]β����ʹ������ͷŸ���һ�㣬����������һ��
static size_t xFreeBytesRemaining = 0U;
static size_t xMinimumEverFreeBytesRemaining = 0U;


void *My_malloc( size_t xWantedSize )
{
	BlockLink_t *pxBlock, *pxPreviousBlock, *pxNewBlockLink;
	void *pvReturn = NULL;

	if( ( xWantedSize & xBlockAllocatedBit ) == 0 )	//���λ�����ж�
	{
		xWantedSize += xHeapStructSize;
		//�������С���ڴ����
		if( ( xWantedSize & portBYTE_ALIGNMENT_MASK ) != 0x00 )
		{
			xWantedSize += ( portBYTE_ALIGNMENT - ( xWantedSize & portBYTE_ALIGNMENT_MASK ) );
		}

		if ( xWantedSize <= xFreeBytesRemaining )
		{
			pxPreviousBlock = &xStart;	//�����ڴ����һ���ڵ㣬����ָ������ڴ����һ���ڵ�
			pxBlock = xStart.pxNextFreeBlock;	//�����ҵ������ڴ��
			// �ҵ����ϴ�С���ڴ��
			while( ( pxBlock->xBlockSize < xWantedSize ) && ( pxBlock->pxNextFreeBlock != NULL ) )
			{
				pxPreviousBlock = pxBlock;
				pxBlock = pxBlock->pxNextFreeBlock;
			}

			if( pxBlock != pxEnd )
			{
				pvReturn = (void *)(((unsigned char *)pxPreviousBlock->pxNextFreeBlock) + xHeapStructSize);
				pxPreviousBlock->pxNextFreeBlock = pxBlock->pxNextFreeBlock;

				//������ڴ��ʣ�µĴ�С���󣬾Ͱ�ʣ�µķֳ�һ�������ڴ�飬������ڴ��ȫ����ȥʹ��
				if( ( pxBlock->xBlockSize - xWantedSize ) > heapMINIMUM_BLOCK_SIZE )
				{

					pxNewBlockLink = ( void * ) ( ( ( unsigned char * ) pxBlock ) + xWantedSize );

					pxNewBlockLink->xBlockSize = pxBlock->xBlockSize - xWantedSize;
					pxBlock->xBlockSize = xWantedSize;	

					/* Insert the new block into the list of free blocks. */
					prvInsertBlockIntoFreeList( pxNewBlockLink );
				}

				xFreeBytesRemaining -= pxBlock->xBlockSize;

				if( xFreeBytesRemaining < xMinimumEverFreeBytesRemaining )
				{
					xMinimumEverFreeBytesRemaining = xFreeBytesRemaining;
				}

				pxBlock->xBlockSize |= xBlockAllocatedBit;
				pxBlock->pxNextFreeBlock = NULL;
			}
			else
			{
				ERR;
			}
		}
		else
		{
			ERR;
		}
	}
	else
	{
		ERR;
	}
	return pvReturn;
}
/*-----------------------------------------------------------*/

void My_free( void *pv )
{
	unsigned char *puc = ( unsigned char * ) pv;
	BlockLink_t *pxLink;

	if( pv != NULL )
	{
		puc -= xHeapStructSize;
		pxLink = ( void * ) puc;

		/* Check the block is actually allocated. */
		if (((pxLink->xBlockSize & xBlockAllocatedBit) == 0) && (pxLink->pxNextFreeBlock != NULL)) {
				ERR;
		}
		/* The block is being returned to the heap - it is no longer
		allocated. */
		pxLink->xBlockSize &= ~xBlockAllocatedBit;

		/* Add this block to the list of free blocks. */
		xFreeBytesRemaining += pxLink->xBlockSize;
		prvInsertBlockIntoFreeList( ( ( BlockLink_t * ) pxLink ) );
	}
}

static void My_heap_init( void )
{
	BlockLink_t *pxFirstFreeBlock;
	unsigned char *pucAlignedHeap;
	size_t uxAddress = ( size_t ) Heap;
	size_t xTotalHeapSize = HEAP_SIZE;
	//�ڴ�heap[]���룬��������xTotalHeapSize����
	if( ( uxAddress & portBYTE_ALIGNMENT_MASK ) != 0 )
	{
		PRINTF("align!\r\n");
		uxAddress += ( portBYTE_ALIGNMENT - 1 );
		uxAddress &= ~( ( size_t ) portBYTE_ALIGNMENT_MASK );
		xTotalHeapSize -= portBYTE_ALIGNMENT;
	}

	pucAlignedHeap = ( unsigned char * ) uxAddress;	//heapͷ

	xStart.pxNextFreeBlock = ( void * ) pucAlignedHeap;
	xStart.xBlockSize = ( size_t ) 0;

	uxAddress = ( ( size_t ) pucAlignedHeap ) + xTotalHeapSize;
	uxAddress -= xHeapStructSize;	//����ڴ�ռ���pxEnd�ṹ��
	uxAddress &= ~( ( size_t ) portBYTE_ALIGNMENT_MASK );
	pxEnd = ( void * ) uxAddress;
	pxEnd->xBlockSize = 0;
	pxEnd->pxNextFreeBlock = NULL;

	pxFirstFreeBlock = ( void * ) pucAlignedHeap;
	pxFirstFreeBlock->xBlockSize = uxAddress - (size_t)pxFirstFreeBlock;
	pxFirstFreeBlock->pxNextFreeBlock = pxEnd;

	xMinimumEverFreeBytesRemaining = pxFirstFreeBlock->xBlockSize;
	xFreeBytesRemaining = pxFirstFreeBlock->xBlockSize;
}

//�������ڴ�飬Ȼ����������ڴ������
static void prvInsertBlockIntoFreeList( BlockLink_t *pxBlockToInsert )
{
	BlockLink_t *pxIterator;
	unsigned char *puc;

	//�ҵ������ڴ����heap[]�ڴ���λ�ã��ҵ�����pxIterator->pxNextFreeBlock����
	for( pxIterator = &xStart; pxIterator->pxNextFreeBlock < pxBlockToInsert;pxIterator = pxIterator->pxNextFreeBlock);

	//���黹���ڴ������һ�������ڴ���Ƿ���Ҫ�ϲ�
	puc = ( unsigned char * ) pxIterator;
	if((puc + pxIterator->xBlockSize) == (unsigned char *) pxBlockToInsert)
	{
		pxIterator->xBlockSize += pxBlockToInsert->xBlockSize;
		pxBlockToInsert = pxIterator;
	}

	//���黹���ڴ������һ�������ڴ���Ƿ���Ҫ�ϲ�
	puc = ( unsigned char * ) pxBlockToInsert;
	if( ( puc + pxBlockToInsert->xBlockSize ) == ( unsigned char * ) pxIterator->pxNextFreeBlock )
	{
		if( pxIterator->pxNextFreeBlock != pxEnd )
		{
			/* Form one big block from the two blocks. */
			pxBlockToInsert->xBlockSize += pxIterator->pxNextFreeBlock->xBlockSize;
			pxBlockToInsert->pxNextFreeBlock = pxIterator->pxNextFreeBlock->pxNextFreeBlock;
		}
		else
		{
			pxBlockToInsert->pxNextFreeBlock = pxEnd;
		}
	}
	else
	{
		pxBlockToInsert->pxNextFreeBlock = pxIterator->pxNextFreeBlock;
	}

	//�������һ�������ڴ�ϲ��ˣ����ø��£�������һ�������ڴ�ָ�򻹵��ڴ��
	if( pxIterator != pxBlockToInsert )
	{
		pxIterator->pxNextFreeBlock = pxBlockToInsert;
	}
}

/*
int main() 
{
	int n = 0;
	HeapInit();
	PRINTF("free num %d\r\n", xFreeBytesRemaining);
	PRINTF("Heap S: %x  xS.next:%x  end:%x\r\n", Heap, xStart.pxNextFreeBlock, pxEnd);
	
	char *a = My_malloc(128);
	char *b = My_malloc(40);
	char *c = My_malloc(22);
	char *d = My_malloc(33);
	char *e = My_malloc(33);
	char *f = My_malloc(16);
	memset(a, 0x01, 128);
	memset(b, 0x07, 40);
	memset(c, 0x02, 22);
	memset(d, 0x03, 33);
	memset(e, 0x04, 33);
	memset(f, 0x0B, 16);
	

	while (n < ARR_NUM(Heap) ) {
		PRINTF(" %d:%x",n/4,*((int *)&Heap[n]));
		n +=4;
	}
	PRINTF("\r\n\r\n");
	memset(a, 0, 128);
	memset(b, 0, 40);
	memset(c, 0, 22);
	memset(d, 0, 33);
	//memset(e, 0x04, 33);
	memset(f, 0, 16);
	My_free(a);
	My_free(b);
	My_free(c);
	My_free(d);
	My_free(f);
	
	n = 0;
	while (n < ARR_NUM(Heap) ) {
		PRINTF(" %d:%x",n/4,*((int *)&Heap[n]));
		n +=4;
	}
	return 0;
}*/


