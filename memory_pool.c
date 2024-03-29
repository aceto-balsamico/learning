#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct 
{
    int data1;
    float data2;
    char data3;
	unsigned char array[10000];
} MemoryBlock;

typedef struct 
{
    MemoryBlock* blocks;
    int size;
    int count;
} MemoryPool;

MemoryPool pool;

void initializeMemoryPool(int size) 
{
    pool.blocks = (MemoryBlock*)malloc(size * sizeof(MemoryBlock));
    pool.size = size;
    pool.count = size;
}

MemoryBlock* allocateMemory(int data1, float data2, char data3) 
{
    if (pool.count > 0) 
	{
        pool.count--;
        MemoryBlock* block = &(pool.blocks[pool.count]);
        block->data1 = data1;
        block->data2 = data2;
        block->data3 = data3;
        return block;
    }
	else 
	{
        printf("メモリブロックが不足しています。\n");
        return NULL;
    }
}

void deallocateMemory(MemoryBlock* block) 
{
    if (pool.count < pool.size) 
	{
        pool.blocks[pool.count] = *block;
        pool.count++;
    } 
	else 
	{
        printf("メモリブロックがいっぱいです。\n");
    }
}

int getRemainingCount() 
{
    return pool.count;
}
void deallocateMultipleMemory(MemoryBlock* blocks, int numBlocks) 
{
    for (int i = 0; i < numBlocks; i++) 
	{
        deallocateMemory(&blocks[i]);
    }
}
void deallocateAndShiftMemory(int numBlocksToDeallocate) 
{
    if (numBlocksToDeallocate >= pool.count) 
	{
        printf("解放するメモリブロックの数が残りのメモリブロック数を超えています。\n");
        return;
    }

    // 解放するメモリブロックの数だけ前半を解放
    pool.count += numBlocksToDeallocate;

    // 解放された後のメモリブロックを解放されたブロックの位置に移動
    for (int i = pool.size - 1; i >= pool.size - pool.count; i--) 
	{
        pool.blocks[i] = pool.blocks[i - numBlocksToDeallocate];
    }
}
void printUsedData1() 
{
    printf("\n使用中のメモリブロックの data1:");
    for (int i = pool.count; i < pool.size; i++) 
	{
        printf("[%p]%d ", &pool.blocks[i].data1, pool.blocks[i].data1);
    }
    printf("\n");
}
void deallocateAllMemory() 
{
    for (int i = pool.count; i < pool.size; i++) 
	{
        deallocateMemory(&pool.blocks[i]);
    }
    pool.count = pool.size;
}
int main() 
{
    initializeMemoryPool(10000);

    MemoryBlock* block1 = allocateMemory(42, 3.14f, 'A');
	for(int i = 0; i < 10000; i++)
	{
		block1->array[i] = i;
	}
    if (block1 != NULL) 
	{
        printf("メモリブロック取得: %d, %f, %c\n", block1->data1, block1->data2, block1->data3);
		for(int i = 0; i < 10000; i++)
		{
			printf("%d,", block1->array[i]);
		}
    }

    MemoryBlock* block2 = allocateMemory(99, 2.718f, 'B');

	memset(block2->array, 0xFF, 10000);
	
    if (block2 != NULL) 
	{
        printf("メモリブロック取得: %d, %f, %c\n", block2->data1, block2->data2, block2->data3);
		for(int i = 0; i < 10000; i++)
		{
			printf("%02X,", block2->array[i]);
		}
    }
	MemoryBlock* block_A = allocateMemory(999, 1.2, 65);
	MemoryBlock* block_B = allocateMemory(888, 1.2, 65);
	MemoryBlock* block_C = allocateMemory(777, 1.2, 65);
	MemoryBlock* block_D = allocateMemory(666, 1.2, 65);
	MemoryBlock* block_E = allocateMemory(555, 1.2, 65);
	printUsedData1();
	deallocateAndShiftMemory(3);
	printUsedData1();


    printf("残りのメモリブロック数: %d\n", getRemainingCount());

    // deallocateMemory(block1);
    // deallocateMemory(block2);
	MemoryBlock blocksToDeallocate[] = {*block1, *block2};
    deallocateMultipleMemory(blocksToDeallocate, sizeof(blocksToDeallocate) / sizeof(MemoryBlock));
	// deallocateAllMemory();
	

    printf("残りのメモリブロック数: %d\n", getRemainingCount());

    free(pool.blocks); // プログラム終了前にメモリ解放

    return 0;
}
