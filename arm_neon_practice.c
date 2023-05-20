#include "common.h"
#include <arm_neon.h>
#pragma GCC optimize("O3")
// #pragma GCC optimize("unroll-loops")

/*
	命名規則
	v:ベクトル演算を表すプレフィックス
	ld:ロード（Load）の略称
	st:ストア（Store）の略称
	add:加算（Addition）の略称
	sub:減算（Subtraction）の略称
	mul:乗算（Multiplication）の略称
	div:除算（Division）の略称
	eor:排他的論理和（Exclusive OR）の略称
	and:論理積（AND）の略称
	or:論理和（OR）の略称
	not:ビット反転（NOT）の略称
	shl:左シフト（Shift Left）の略称
	shr:右シフト（Shift Right）の略称
	padal:ペア加算（Pairwise Addition and Accumulation）の略称
	paddl:ペア加算（Pairwise Addition）の略称 

	データサイズ用語
	ビット（bit）: 最小のデータ単位であり、0または1の値を表す
	バイト（byte）: 8ビットのデータ単位であり、一般的に文字や数値を表現するのに使用される
	ワード（word）: プロセッサが一度に扱うことができるデータのサイズを指します。16ビット、32ビット、64ビットと一定ではない。
	ダブルワード（double word）: ワードの2倍のサイズを持つデータのこと。一般的に32ビットまたは64ビット。
	クワッドワード（quad word）: ワードの4倍のサイズを持つデータのこと。一般的に64ビットまたは128ビット。
 */
uint32_t PopcntAlgorithm(uint8_t* datax, uint8_t* datay, size_t len)
{
	uint32_t data = 0;
	uint32_t result = 0;
	
	for(int i = 0; i < len; i++)
	{
		data 	= datax[i] ^ datay[i];
		data 	= (data & 0x55) + ((data & 0xAA) >> 1);
		data 	= (data & 0x33) + ((data & 0xCC) >> 2);
		result	+= (data & 0x0F) + ((data & 0xF0) >> 4);
	}
	return result;
}

void count_ones_neon_partitioned(uint8_t* datax, uint8_t* datay, const int size, uint32_t* resultArray, const int numPartitions) 
{
	const int elementsPerPartition = size / numPartitions;
	const int vecSize = elementsPerPartition / 16;	//128bit neon register / uint8_t
	const int remainvecSize = elementsPerPartition % 16;	//partition中の16個同時ロードできない余りの数

	for (int p = 0; p < numPartitions; p++) 
	{
		uint8_t* dataxPartition = datax + p * elementsPerPartition;
		uint8_t* datayPartition = datay + p * elementsPerPartition;
		uint32_t* resultPartition = resultArray + p;

		uint8x16_t dataxVec, datayVec;
		uint8x16_t data16;
		uint8x8_t data8;
		uint16x8_t tmpResult16_8;
		uint16x4_t tmpResult16_4;
		uint32x4_t resultVec16 = vdupq_n_u32(0);
		uint32x2_t resultVec8 = vdup_n_u32(0);

		for (int i = 0; i < vecSize; i++) 
		{
			__builtin_prefetch(&dataxPartition[i * 16], 0, 3);
			__builtin_prefetch(&datayPartition[i * 16], 0, 3);
		}
		//128bit neon registerにuint8_t型を16個ロードして処理。
		for (int i = 0; i < vecSize; i+=1)
		{
			// XOR演算
			data16 = veorq_u8(vld1q_u8(dataxPartition), vld1q_u8(datayPartition));

			// ビットの1の数を数える
			data16 = vaddq_u8(vandq_u8(data16, vdupq_n_u8(0x55)), vshrq_n_u8(vandq_u8(data16, vdupq_n_u8(0xAA)), 1));
			data16 = vaddq_u8(vandq_u8(data16, vdupq_n_u8(0x33)), vshrq_n_u8(vandq_u8(data16, vdupq_n_u8(0xCC)), 2));
			data16 = vaddq_u8(vandq_u8(data16, vdupq_n_u8(0x0F)), vshrq_n_u8(vandq_u8(data16, vdupq_n_u8(0xF0)), 4));

			//隣り合う8bit整数をそれぞれ加算し、16bit整数ベクトルにする
			tmpResult16_8 = vpaddlq_u8(data16);

			// 結果を32bit整数ベクトルに加算
			resultVec16 = vpadalq_u16(resultVec16, tmpResult16_8);
			
			dataxPartition += 16;	//128bit neon register / uint8_t
			datayPartition += 16;
		}

		//16個ロードできなかったpartitionの余り処理。0~15個の余りが存在する。
		//余りの要素数が8以上だったら64bit neon registerに8個ロードする
		if(remainvecSize >= 8)
		{
			data8 = veor_u8(vld1_u8(dataxPartition), vld1_u8(datayPartition));
			data8 = vadd_u8(vand_u8(data8, vdup_n_u8(0x55)), vshr_n_u8(vand_u8(data8, vdup_n_u8(0xAA)), 1));
			data8 = vadd_u8(vand_u8(data8, vdup_n_u8(0x33)), vshr_n_u8(vand_u8(data8, vdup_n_u8(0xCC)), 2));
			data8 = vadd_u8(vand_u8(data8, vdup_n_u8(0x0F)), vshr_n_u8(vand_u8(data8, vdup_n_u8(0xF0)), 4));
			
			tmpResult16_4 = vpaddl_u8(data8);
			
			resultVec8 = vpadal_u16(resultVec8, tmpResult16_4);
			resultPartition[0] += vaddv_u32(resultVec8);

			dataxPartition += 8;
			datayPartition += 8;
		}
		if(remainvecSize%8 > 0)	//余りが8以下になったらneon registerは使用せずアルゴリズムで処理する
		{
			resultPartition[0] += PopcntAlgorithm(dataxPartition, datayPartition, remainvecSize%8);

			dataxPartition += remainvecSize%8;
			datayPartition += remainvecSize%8;
		}

		// 32ビット整数ベクトルの要素を水平方向に加算
		resultPartition[0] += vaddvq_u32(resultVec16);
	}
}
uint32_t count_ones_neon_algorithm(const uint8_t* datax, const uint8_t* datay, const int size) 
{
	const int vecSize = size / 16;

	uint8x16_t dataxVec, datayVec;
	uint8x16_t data;
	uint16x8_t tmpResult16;
	uint32x4_t resultVec = vdupq_n_u32(0);

	for (int i = 0; i < vecSize; i++) 
	{
		__builtin_prefetch(&datax[i * 16], 0, 3);
		__builtin_prefetch(&datay[i * 16], 0, 3);
	}
	for (int i = 0; i < vecSize; i++) {
		// 16個の要素をロード
		dataxVec = vld1q_u8(datax);
		datayVec = vld1q_u8(datay);

		// XOR演算
		data = veorq_u8(dataxVec, datayVec);

		// ビットの1の数を数える
		data = vaddq_u8(vandq_u8(data, vdupq_n_u8(0x55)), vshrq_n_u8(vandq_u8(data, vdupq_n_u8(0xAA)), 1));
		data = vaddq_u8(vandq_u8(data, vdupq_n_u8(0x33)), vshrq_n_u8(vandq_u8(data, vdupq_n_u8(0xCC)), 2));
		data = vaddq_u8(vandq_u8(data, vdupq_n_u8(0x0F)), vshrq_n_u8(vandq_u8(data, vdupq_n_u8(0xF0)), 4));

		tmpResult16 = vpaddlq_u8(data);

		// 結果を32ビット整数ベクトルに加算
		resultVec = vpadalq_u16(resultVec, tmpResult16);

		datax += 16;
		datay += 16;
	}

	// 32ビット整数ベクトルの要素を水平方向に加算
	uint32_t result = vaddvq_u32(resultVec);

	return result;
}

#define Num_Partition 16
int main() 
{
	// 配列の要素数
	int arraySize = 100000;	//numPartitionの倍数

	// 配列のメモリアラインメント
	const int alignment = 64;

	// 配列のメモリ領域を確保し、メモリアラインメントを行う
	uint8_t* array1;
	uint8_t* array2;
	uint32_t* array3;

	posix_memalign((void**)&array1, alignment, arraySize * sizeof(uint8_t));
	posix_memalign((void**)&array2, alignment, arraySize * sizeof(uint8_t));
	posix_memalign((void**)&array3, alignment, arraySize * sizeof(uint32_t));
	uint32_t* align32;
	posix_memalign((void**)&align32, alignment, Num_Partition * sizeof(uint32_t));

	// 配列に値を設定
	for (int i = 0; i < arraySize; i++) 
	{
		array1[i] = 0x00;
		array2[i] = 0x11;
		array3[i] = 0x11;
	}
	for (int i = 0; i < Num_Partition; i++) 
	{
		align32[i] = 0x00;
	}

	int onesCount = 0;

	begin_time(1);
	for(int i = 0; i < arraySize; i++)
	onesCount += __builtin_popcount(array3[i]);//0.013ms
	end_time(1);
	printf("\tbuiltin popcount:%d\n",onesCount);

	begin_time(1);
	onesCount = PopcntAlgorithm(array1, array2, arraySize);//0.041ms
	end_time(1);
	printf("\tpopcount algorithm:%d\n",onesCount);

	begin_time(1);
	onesCount = count_ones_neon_algorithm(array1, array2, arraySize);	//simdでアルゴリズム0.011ms
	end_time(1);
	printf("\tSIMD algorithm:%d\n",onesCount);

	begin_time(1);
	count_ones_neon_partitioned(array1, array2, arraySize, align32, Num_Partition);	//配列を指定したpartitionで出力する。0.012ms
	end_time(1);
	printf("\tSIMD algorithm partitioned: ");for(int i = 0; i < Num_Partition; i++) printf("%d,", align32[i]);

	// メモリを解放
	free(array1);
	free(array2);
	free(array3);
	free(align32);

	return 0;
}
