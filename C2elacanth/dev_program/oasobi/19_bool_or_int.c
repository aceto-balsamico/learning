#include "custom_common.h"

#define ARRAY_SIZE 10000000
#define REPEAT 10

// 時間計測用
struct timespec start_time, end_time;

double measure_time(void (*func)(void))
{
	clock_gettime(CLOCK_MONOTONIC, &start_time);
	func();
	clock_gettime(CLOCK_MONOTONIC, &end_time);
	return (end_time.tv_sec - start_time.tv_sec) +
		   (end_time.tv_nsec - start_time.tv_nsec) / 1000000000.0;
}

bool bool_array[ARRAY_SIZE];
int int_array[ARRAY_SIZE];

void reset_bool_array()
{
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		bool_array[i] = false;
	}
}

void reset_int_array()
{
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		int_array[i] = 0;
	}
}
//@@@function
void bool_or_int(void)
{
	double total_bool_time = 0.0, total_int_time = 0.0;

	for (int i = 0; i < REPEAT; i++)
	{
		total_bool_time += measure_time(reset_bool_array);
		total_int_time += measure_time(reset_int_array);
	}

	printf("\n結果 (配列サイズ: %d, 繰り返し回数: %d)\n", ARRAY_SIZE, REPEAT);
	printf("bool 配列の平均処理時間: %.6f 秒\n", total_bool_time / REPEAT);
	printf("int 配列の平均処理時間: %.6f 秒\n", total_int_time / REPEAT);

	return;
}