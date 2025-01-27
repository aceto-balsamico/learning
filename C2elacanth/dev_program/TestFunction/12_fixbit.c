#include <stdio.h>
#include <math.h>

#define NUM_SAMPLES 128 // 入力範囲 (0～127)
#define THRESHOLD 0.5  // 傾きの閾値（平行判定のため）

// 仮の機械応答関数 (bit固定をシミュレート)
double machine_response(int input) {
    // 正常なシグモイド関数 (例: 1 / (1 + exp(-0.1 * (x - 64))))
    double sigmoid = 1.0 / (1.0 + exp(-0.1 * (input - 64)));
    // 例: 6bit目 (0x40) が1固定されている場合を模擬
    int modified_input = input | 0x80; 
    // int modified_input = input; 
    // double modified_sigmoid = 1.0 / (1.0 + exp(-0.1 * (modified_input - 64)));
	double modified_sigmoid = 5 * modified_input;
    return modified_sigmoid;
}

// 線形回帰を用いた傾き計算
double calculate_slope(double *responses, int start, int end) {
    double sum_x = 0.0, sum_y = 0.0, sum_xy = 0.0, sum_x2 = 0.0;
    int n = end - start + 1;

    for (int i = start; i <= end; i++) {
        sum_x += i;
        sum_y += responses[i];
        sum_xy += i * responses[i];
        sum_x2 += i * i;
    }

    // 傾き (slope) を計算
    double slope = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x * sum_x);
    return slope;
}

// bit固定を検出
void detect_fixed_bits() {
    double responses[NUM_SAMPLES];

    // 0～127の範囲で応答を計算し配列に格納
    for (int i = 0; i < NUM_SAMPLES; i++) {
        responses[i] = machine_response(i);
    }

    int bit_fixed = -1; // 固定bit（-1は未検出）

    for (int bit = 4; bit > 0; bit--) {
        int segment_size = 1 << bit; // 区間のサイズ
        int num_segments = NUM_SAMPLES / segment_size; // 区間数

        double previous_slope = calculate_slope(responses, 0, NUM_SAMPLES - 1);
        int is_all_parallel = 1; // すべて平行かどうかのフラグ

		printf("check bit: %d, num_segments: %d, segment_size: %d, previous_slope: %f\n", bit, num_segments, segment_size, previous_slope);
        // 各区間の傾きを比較
        for (int i = 0; i < num_segments; i++) {
            int start = i * segment_size;
            int end = start + segment_size - 1;
            double current_slope = calculate_slope(responses, start, end);

            if (fabs(previous_slope - current_slope) > THRESHOLD) {
				printf("差が%.6fであります。\n", fabs(previous_slope - current_slope));
                is_all_parallel = 0;
                break;
            }
			
			printf("segment: %2d, check slope: start:%3d, end:%3d,", i, start, end);
			printf(" prev:%.6f, current:%.6f\n", previous_slope, current_slope);
        }

        if (is_all_parallel) {
            bit_fixed = bit;
            printf("%d bit目が1固定されている可能性があります。\n", bit);
            return;
        }
    }

    if (bit_fixed == -1) {
        printf("1固定されたbitは検出されませんでした。\n");
    }
}

//@@@function
int fixbit()
{
	detect_fixed_bits();

	double temp[10] = { 1, 1, 1, 1, 1};
	printf("slope:%f\n", calculate_slope(temp, 0, 4));
	return 0;
}