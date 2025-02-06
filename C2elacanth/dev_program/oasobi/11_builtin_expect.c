#include "custom_common.h"

#define _GNU_SOURCE

#define ITERATIONS 100000000 // 1億回ループ
#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

#define IFLI(cond) if (LIKELY(cond))
#define ELIF(cond) else if (LIKELY(cond))
#define ELSE(cond) else { cond }

// 時間計測用関数
double get_time_diff(struct timespec start, struct timespec end)
{
	return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

//@@@function
void benchmark_LIKELY()
{
	struct timespec start, end;
	volatile int count = 0;

	// 通常の条件分岐
	clock_gettime(CLOCK_MONOTONIC, &start);
	for (int i = 0; i < ITERATIONS; i++)
	{
		if (i % 2 == 0)
		{ // 偶数のときに分岐
			count++;
		}
	}
	clock_gettime(CLOCK_MONOTONIC, &end);
	double normal_time = get_time_diff(start, end);

	// LIKELY を使った条件分岐
	clock_gettime(CLOCK_MONOTONIC, &start);
	for (int i = 0; i < ITERATIONS; i++)
	{
		// if (LIKELY(i % 2 == 0))
		IFLI(i % 2 == 0)
		{ // 偶数のときに分岐
			count++;
		}
	}
	clock_gettime(CLOCK_MONOTONIC, &end);
	double likely_time = get_time_diff(start, end);

	// 結果を表示
	printf("Normal if statement time: %.6f seconds\n", normal_time);
	printf("LIKELY if statement time: %.6f seconds\n", likely_time);
	printf("Performance improvement: %.2f%%\n",
		   (1 - (likely_time / normal_time)) * 100);

}