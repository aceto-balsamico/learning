
#include <arm_neon.h>
#include "common.h"

uint32_t sum_neon32(uint32_t* arr, size_t len) {
    uint32x4_t sum_vec = vdupq_n_u32(0);
    for (size_t i = 0; i < len; i += 4) {
        uint32x4_t data_vec = vld1q_u32(arr + i);
        sum_vec = vaddq_u32(sum_vec, data_vec);
    }
    uint32_t sum = vgetq_lane_u32(sum_vec, 0);
    sum += vgetq_lane_u32(sum_vec, 1);
    sum += vgetq_lane_u32(sum_vec, 2);
    sum += vgetq_lane_u32(sum_vec, 3);
    for (size_t i = len & ~0x3; i < len; ++i) {
        sum += arr[i];
    }
    return sum;
}
uint32_t sum_neon8(uint8_t* input, int size) {
    uint32x4_t sum_vec = vdupq_n_u32(0);
    uint32x4_t ones = vdupq_n_u32(0x01010101);
    int i;

    for (i = 0; i < size - 15; i += 16) {
        uint8x16_t input_vec = vld1q_u8(input + i);
        uint16x8_t input_low = vmovl_u8(vget_low_u8(input_vec));
        uint16x8_t input_high = vmovl_u8(vget_high_u8(input_vec));
        uint32x4_t input_low_sum = vpaddlq_u16(input_low);
        uint32x4_t input_high_sum = vpaddlq_u16(input_high);
        sum_vec = vaddq_u32(sum_vec, vaddq_u32(input_low_sum, input_high_sum));
    }

    uint32_t sum = 0;
    for (; i < size; i++) {
        sum += input[i];
    }
    sum += vgetq_lane_u32(sum_vec, 0);
    sum += vgetq_lane_u32(sum_vec, 1);
    sum += vgetq_lane_u32(sum_vec, 2);
    sum += vgetq_lane_u32(sum_vec, 3);
    return sum;
}
uint32_t normal_sum32(uint32_t* arr, size_t len)
{
	uint32_t sum = 0;;
	for(int i = 0; i < len; i++)
	{
		sum += arr[i];
	}
	return sum;
}
uint32_t normal_sum8(uint8_t* arr, size_t len)
{
	uint32_t sum = 0;
	for(int i = 0; i < len; i++)
	{
		sum += arr[i];
	}
	return sum;
}
uint32_t PopcntAlgorithm(u_int8_t* datax, u_int8_t* datay, size_t len)
{
	uint32_t data = 0;
	uint32_t array_1count = 0;
	for(int i = 0; i < len; i++)
	{
	 	data 			= datax[i] ^ datay[i];
		data 			= (data & 0x55) + ((data & 0xAA) >> 1);
		data 			= (data & 0x33) + ((data & 0xCC) >> 2);
		array_1count	+= (data & 0x0F) + ((data & 0xF0) >> 4);
	}
	return array_1count;
}

uint32_t fbc_uint8_t_neon(uint8_t* arrayA, uint8_t* arrayB, size_t len)
{
	//16Byte(128bit)並列同時動作
    size_t i;
    uint32x4_t count = vdupq_n_u32(0);	//countはpopcountの総数を格納するSIMDレジスタ.vdupq_n_u32は、4つのuint32_t型の値を持つレジスタを作成し、すべての値を0に初期化
    for (i = 0; i < len; i+=16) 
	{
        uint8x16_t vecA = vld1q_u8(arrayA + i);	//vld1q_u8は、uint8_t型の値を16個持つレジスタに読み込む
        uint8x16_t vecB = vld1q_u8(arrayB + i);
		uint8x16_t vecXOR = veorq_u8(vecA, vecB);
		uint8x16_t popcount = vcntq_u8(vecXOR);
		count = vaddq_u8(count, popcount);	//vaddq_u32は、2つのレジスタの値を足し合わせ
    }
	// printf("%u, %u, %u, %u\n", count[0], count[1], count[2], count[3]);
	uint32_t result = 0;
	for(int i = 0; i < 16; i++)
	{
		result += count[i];
	}
	
    return result;
}
int main()
{
	uint8_t array8[100000] = {};
	uint32_t array32[100000] = {};
	uint8_t* aline_array8 = (uint8_t*)aligned_alloc(16, 100000 * sizeof(uint8_t));
	uint32_t* aline_array32 = (uint32_t*)aligned_alloc(16, 100000 * sizeof(uint32_t));
	for(int i = 0; i < 100000; i++)
	{
		array8[i] = 0x01;
		array32[i] = 0x01;
		aline_array8[i] = 0x01;
		aline_array32[i] = 0x01;
	}
	uint8_t array8_temp[100000] = {};
	uint32_t array32_temp[100000] = {};
	uint8_t* aline_array8_temp = (uint8_t*)aligned_alloc(16, 100000 * sizeof(uint8_t));
	uint32_t* aline_array32_temp = (uint32_t*)aligned_alloc(16, 100000 * sizeof(uint32_t));
	for(int i = 0; i < 100000; i++)
	{
		array8_temp[i] = 0x10;
		array32_temp[i] = 0x10;
		aline_array8_temp[i] = 0x10;
		aline_array32_temp[i] = 0x10;
	}
	uint32_t result0 = 0;
	uint32_t result1 = 0;

	begin_time(1);
	for(int i = 0; i < 100000; i++)
	result0 = __builtin_popcount(array32[i]);
	end_time(1);
	begin_time(1);
	for(int i = 0; i < 100000; i++)
	result1 = __builtin_popcount(aline_array32[i]);
	end_time(1);
	begin_time(1);
	for(int i = 0; i < 100000; i++)
	result0 = PopcntAlgorithm(array8, array8_temp, 100000);
	end_time(1);
	begin_time(1);
	for(int i = 0; i < 100000; i++)
	result1 = PopcntAlgorithm(aline_array8, aline_array8_temp, 100000);
	end_time(1);
	begin_time(1);
	for(int i = 0; i < 100000; i++)
	result0 = fbc_uint8_t_neon(array8, array8_temp, 100000);
	end_time(1);
	begin_time(1);
	for(int i = 0; i < 100000; i++)
	result1 = fbc_uint8_t_neon(aline_array8, aline_array8_temp, 100000);
	end_time(1);

	printf("result0:%u, ,result1:%u\n", result0, result1);
	printf("sum_neon8:%u, sum_neon32:%u\n", sum_neon8(array8, 100000), sum_neon32(array32, 100000));

	free(aline_array8);
	free(aline_array32);
	free(aline_array8_temp);
	free(aline_array32_temp);

}
