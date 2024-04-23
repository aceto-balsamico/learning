#include <stdio.h>
#include <time.h>

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
void sleep_ms(unsigned int milliseconds)
{
	struct timespec ts;
	ts.tv_sec = milliseconds / 1000;
	ts.tv_nsec = (milliseconds % 1000) * 1000000;
	nanosleep(&ts, NULL);
}

int main()
{
	int totalIterations = 1000;

	for (int i = 0; i < totalIterations; i++)
	{
		sleep_ms(10);

		progressBar(i + 1, totalIterations, 100);
	}

	return 0;
}