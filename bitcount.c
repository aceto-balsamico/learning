#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdarg.h>
#include <arm_neon.h>

#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

double time_spent, begin, end;
struct timespec ts1, ts2;
//way:0=clock(), 1=clock_gettime(clock_monotonic)
void begin_time(int select_way)
{
	if(select_way == 0)	begin = clock();
	else if(select_way == 1)	clock_gettime(CLOCK_MONOTONIC, &ts1);
}
//0=CLOCKS_PER_SEC(CPU Execution time), 1=CLOCK_MONOTONIC(elapsed time)
void end_time(int select_way)
{
	end = clock();
	if(select_way == 0)
	{
		time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
		printf(" CPU Execution time is %f [ms]", time_spent * 1000);
	}
	else if(select_way == 1)
	{
		clock_gettime(CLOCK_MONOTONIC, &ts2);
		printf(" elapsed time is %f [ms]", (ts2.tv_sec - ts1.tv_sec) * 1000.0 + (ts2.tv_nsec - ts1.tv_nsec) / 1000000.0);	//milisec order
	}
} 

int numofbits1(unsigned char bits)
{
	int num = 0;
	unsigned char mask = 1;

	for (; mask != 0; mask = mask << 1) {
		if (bits & mask)
			num++;
	}

	return num;
}

const int BITS_COUNT_TABLE[256] = {
	0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
	4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8,
};

int numofbits2(unsigned char bits)
{
	return BITS_COUNT_TABLE[bits];
}

int numofbits3(unsigned char bits)
{
	int num = 0;

	for (num = 0; bits != 0; bits &= bits - 1) 
	{
		num++;
	}

	return num;
}

int numofbits5(unsigned char bits)
{
	bits = (bits & 0x55) + (bits >> 1 & 0x55);
	bits = (bits & 0x33) + (bits >> 2 & 0x33);
	return bits = (bits & 0x0f) + (bits >> 4 & 0x0f);
	// bits = (bits & 0x00ff00ff) + (bits >> 8 & 0x00ff00ff);
	// return (bits & 0x0000ffff) + (bits >> 16 & 0x0000ffff);
}
int builtin(unsigned char bits)
{
	return __builtin_popcount(bits);
}

#define REGISTERSIZE_ARMV7 128
int count_ones_neon_algorithm(const uint8_t* data, const int size) 
{
	const int vecSize = size / (REGISTERSIZE_ARMV7 / __CHAR_BIT__);
	// uint8x16_t dataxVec, datayVec;
	uint8x16_t dataVec;
	uint16x8_t tmpResult16;
	uint32x4_t resultVec = vdupq_n_u32(0);

	for (int i = 0; i < vecSize; i++) 
	{
		dataVec = vld1q_u8(data);

		// 結果を32ビット整数ベクトルに加算
		resultVec = vpadalq_u16(resultVec, vpaddlq_u8(vcntq_u8(dataVec)));

		data += 16;
	}

	// 32ビット整数ベクトルの要素を水平方向に加算
	uint32_t result = vaddvq_u32(resultVec);

	//neonで処理できなかったあまりを計算
	for(int i = 0; i < size % (REGISTERSIZE_ARMV7 / __CHAR_BIT__); i++)
	{
		result += __builtin_popcount(*data);
		data += 1;
	}

	return result;
}
int main()
{
	#define NUM 5000000
	unsigned char data[NUM];
	srand(time(NULL));
	for(int i = 0; i < NUM; i++)
	{
		data[i] = rand();
	}
	int count = 0;
	begin_time(1);
	for(int i = 0; i < NUM; i++)
	{	
		count += numofbits1(data[i]);
	}
	end_time(1);printf("   count:%5d 1bitずつ数える方法\n", count);count = 0;
	
	begin_time(1);
	for(int i = 0; i < NUM; i++)
	{	
		count += numofbits2(data[i]);
	}
	end_time(1);printf("   count:%5d 配列で持っておく方法\n", count);count = 0;
	
	begin_time(1);
	for(int i = 0; i < NUM; i++)
	{	
		count += numofbits5(data[i]);
	}
	end_time(1);printf("   count:%5d 最速アルゴリズム\n", count);count = 0;
	
	begin_time(1);
	for(int i = 0; i < NUM; i++)
	{	
		count += numofbits3(data[i]);
	}
	end_time(1);printf("   count:%5d 最下位のbitを0にするを繰り返す方法\n", count);count = 0;
	
	begin_time(1);
	for(int i = 0; i < NUM; i++)
	{	
		count += builtin(data[i]);
	}
	end_time(1);printf("   count:%5d gcc組み込み関数\n", count);count = 0;
	
	begin_time(1);
	count = count_ones_neon_algorithm(data, NUM);	
	end_time(1);printf("   count:%5d arm intrinsic(128bit)\n", count);count = 0;
	
	return 0;
}