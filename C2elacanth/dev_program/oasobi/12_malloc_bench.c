#include "custom_common.h"
#define NUM_ALLOCS 1000000
#define ALIGNMENT 64 // 例えば、64バイトアライメント

void benchmark_malloc()
{
	clock_t start_time, end_time;

	// mallocのベンチマーク
	start_time = clock();
	for (int i = 0; i < NUM_ALLOCS; i++)
	{
		void *ptr = malloc(64); // 64バイトのメモリを割り当て
		free(ptr);
	}
	end_time = clock();
	printf("malloc time: %lf seconds\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
}

void benchmark_aligned_alloc()
{
	clock_t start_time, end_time;

	// aligned_allocのベンチマーク
	start_time = clock();
	for (int i = 0; i < NUM_ALLOCS; i++)
	{
		void *ptr = aligned_alloc(ALIGNMENT, 64); // 64バイトのメモリをアライメントを指定して割り当て
		free(ptr);
	}
	end_time = clock();
	printf("aligned_alloc time: %lf seconds\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
}

//@@@function
void malloc_benchmark()
{
	benchmark_malloc();
	benchmark_aligned_alloc();
	return;
}