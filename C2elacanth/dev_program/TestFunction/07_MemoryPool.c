#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BLOCK_SIZES 3  // ブロックサイズの種類
#define NUM_ALLOCS 100000  // 確保・解放を繰り返す回数

// メモリプール構造体
typedef struct {
    char *pool;       // プールの開始アドレス
    size_t pool_size; // プールのサイズ
    size_t offset;    // 現在のオフセット
} MemoryPool;

// メモリプールの初期化
MemoryPool* init_pool(size_t size) {
    MemoryPool *mp = (MemoryPool *)malloc(sizeof(MemoryPool));
    mp->pool = (char *)malloc(size);
    mp->pool_size = size;
    mp->offset = 0;
    return mp;
}

// メモリプールからメモリを割り当て
void* pool_alloc(MemoryPool *mp, size_t size) {
    if (mp->offset + size > mp->pool_size) {
        return NULL; // メモリ不足
    }
    void *ptr = mp->pool + mp->offset;
    mp->offset += size;
    return ptr;
}

// メモリプールのリセット
void pool_reset(MemoryPool *mp) {
    mp->offset = 0; // 先頭にリセット
}

// メモリプールの解放
void free_pool(MemoryPool *mp) {
    free(mp->pool);
    free(mp);
}

// ベンチマーク関数（malloc/free）
void benchmark_malloc(size_t block_size) {
    char *ptrs[NUM_ALLOCS];
    for (int i = 0; i < NUM_ALLOCS; ++i) {
        ptrs[i] = (char *)malloc(block_size);
        if (ptrs[i] != NULL) {
            memset(ptrs[i], 0, block_size); // メモリを初期化
        }
    }
    for (int i = 0; i < NUM_ALLOCS; ++i) {
        free(ptrs[i]);
    }
}

// ベンチマーク関数（メモリプール）
void benchmark_pool(MemoryPool *pool, size_t block_size) {
    void *ptrs[NUM_ALLOCS];
    for (int i = 0; i < NUM_ALLOCS; ++i) {
        ptrs[i] = pool_alloc(pool, block_size);
        if (ptrs[i] != NULL) {
            memset(ptrs[i], 0, block_size); // メモリを初期化
        }
    }
    pool_reset(pool); // メモリプールをリセット
}

//@@@function
void MemoryPool_benchmark() {
    size_t sizes[BLOCK_SIZES] = {32, 512, 8192}; // 小・中・大のメモリブロックサイズ
    clock_t start, end;

    printf("Benchmarking malloc/free vs MemoryPool\n");
    printf("Allocations: %d\n\n", NUM_ALLOCS);

    for (int i = 0; i < BLOCK_SIZES; ++i) {
        size_t block_size = sizes[i];
        printf("Block Size: %zu bytes\n", block_size);

        // malloc/free ベンチマーク
        start = clock();
        benchmark_malloc(block_size);
        end = clock();
        printf("  malloc/free Time: %.6f seconds\n",
               (double)(end - start) / CLOCKS_PER_SEC);

        // メモリプール ベンチマーク
        MemoryPool *pool = init_pool(NUM_ALLOCS * block_size);
        start = clock();
        benchmark_pool(pool, block_size);
        end = clock();
        printf("  MemoryPool Time: %.6f seconds\n\n",
               (double)(end - start) / CLOCKS_PER_SEC);
        free_pool(pool);
    }

    return;
}