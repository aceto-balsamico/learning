#include <stdio.h>
#include <stdlib.h>

typedef struct 
{
    int data1;
    float data2;
    char data3;
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

int main() 
{
    initializeMemoryPool(5);

    MemoryBlock* block1 = allocateMemory(42, 3.14f, 'A');
    if (block1 != NULL) 
	{
        printf("メモリブロック取得: %d, %f, %c\n", block1->data1, block1->data2, block1->data3);
    }

    MemoryBlock* block2 = allocateMemory(99, 2.718f, 'B');
    if (block2 != NULL) 
	{
        printf("メモリブロック取得: %d, %f, %c\n", block2->data1, block2->data2, block2->data3);
    }

    printf("残りのメモリブロック数: %d\n", getRemainingCount());

    deallocateMemory(block1);
    deallocateMemory(block2);

    printf("残りのメモリブロック数: %d\n", getRemainingCount());

    free(pool.blocks); // プログラム終了前にメモリ解放

    return 0;
}
