#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// メモリプール構造体
typedef struct MemoryPool {
	char* pool;
	size_t block_size;
	size_t used;
} MemoryPool;

// メモリプールの初期化
void initMemoryPool(MemoryPool* mp, size_t size) {
	mp->pool = (char*)malloc(size * sizeof(char));
	mp->block_size = size;
	mp->used = 0;
}

// メモリプールからメモリを割り当て
void* poolAlloc(MemoryPool* mp, size_t size) {
	if (mp->used + size > mp->block_size) {
		return NULL; // メモリが足りない場合はNULLを返す
	}
	void* ptr = mp->pool + mp->used;
	mp->used += size;
	return ptr;
}

// メモリプールの解放
void freeMemoryPool(MemoryPool* mp) {
	free(mp->pool);
	mp->pool = NULL;
	mp->block_size = 0;
	mp->used = 0;
}
typedef struct {
	int* data;
	size_t size;
	size_t capacity;
	MemoryPool* mem_pool;
} DynamicArray;

// 動的配列の初期化
void initArray(DynamicArray* a, size_t initialSize, MemoryPool* mp) {
	a->mem_pool = mp;
	a->data = (int*)poolAlloc(mp, initialSize * sizeof(int));
	a->size = 0;
	a->capacity = initialSize;
}

// 要素の追加
void insertArray(DynamicArray* a, int element) {
	if (a->size == a->capacity) {
		size_t new_capacity = a->capacity * 2;
		int* new_data = (int*)poolAlloc(a->mem_pool, new_capacity * sizeof(int));
		if (!new_data) {
			fprintf(stderr, "Out of memory\n");
			exit(EXIT_FAILURE);
		}
		memcpy(new_data, a->data, a->size * sizeof(int));
		a->data = new_data;
		a->capacity = new_capacity;
		printf("debug: capacity increased to %zu (Array Byte: %zu)\n", a->capacity, a->capacity * sizeof(int));
	}
	a->data[a->size++] = element;
}

// 動的配列の解放
void freeArray(DynamicArray* a) {
	// メモリプールの一部として管理されているため、特別な解放操作は不要
	a->data = NULL;
	a->size = 0;
	a->capacity = 0;
}
int main() {
	clock_t start, end;
	double cpu_time_used;
	MemoryPool mp;
	size_t pool_size = 1024 * 1024 * 1280; // 1.28GBプール
	size_t n = 100000000; // 1億要素の追加(400MB)

	initMemoryPool(&mp, pool_size);

	DynamicArray a;
	initArray(&a, 5, &mp);

	start = clock();
	for (size_t i = 0; i < n; i++) {
		insertArray(&a, i);
	}
	end = clock();

	int sum = 0;
	for(int i=0;i<a.size;i++){
		sum += a.data[i];
	}


	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("C with custom allocator: Time taken: %f seconds, sum: %d\n", cpu_time_used, sum);

	freeMemoryPool(&mp);

	return 0;
}
