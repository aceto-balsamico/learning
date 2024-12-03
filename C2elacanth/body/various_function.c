#include "custom_common.h"


void begin_time()
{
	clock_gettime(CLOCK_MONOTONIC, &ts1);
}
double end_time_ms()
{
	clock_gettime(CLOCK_MONOTONIC, &ts2);

	return (ts2.tv_sec - ts1.tv_sec) * 1000.0 + (ts2.tv_nsec - ts1.tv_nsec) / 1000000.0;
}
double end_time_us()
{
	clock_gettime(CLOCK_MONOTONIC, &ts2);

	return (ts2.tv_sec - ts1.tv_sec) * 1000000.0 + (ts2.tv_nsec - ts1.tv_nsec) / 1000.0;
}

void progressBar(int current, int total, int barWidth) 
{
	// 進捗を計算
	float progress = (float)current / total;
	int progressBarLength = progress * barWidth;

	// プログレスバーを構築
	printf("[");
	for (int i = 0; i < barWidth; ++i) 
	{
		if (i < progressBarLength) 
		{
			printf("=");
		}
		else 
		{
			printf(" ");
		}
	}
	printf("] %.2f%%\r", progress * 100); // カーソルを行の先頭に戻す

	if (current == total) 
	{
		printf("\n");
	}
	fflush(stdout);
}