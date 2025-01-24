#include <custom_common.h>

#define ARRAY_SIZE 512
#define SEGMENTS 8
#define THRESHOLD_INTEGRAL 5.0 // 積分値の差の閾値

// 段階的なシグモイドリファレンスデータを生成する関数
void generate_reference_data(double *data, size_t size)
{
	size_t segment = size / SEGMENTS; // セグメントのサイズ
	for (size_t i = 0; i < size; i++)
	{
		if (i < segment)
		{
			data[i] = 0.0;
		}
		else if (i < 3 * segment)
		{
			data[i] = 0.5 * (1.0 - cos(M_PI * (i - segment) / (2 * segment)));
		}
		else if (i < 5 * segment)
		{
			data[i] = 1.0;
		}
		else if (i < 7 * segment)
		{
			data[i] = 1.0 + 0.5 * (1.0 - cos(M_PI * (i - 5 * segment) / (2 * segment)));
		}
		else
		{
			data[i] = 2.0;
		}
	}
}

// セグメントごとの積分値を計算する関数
double calculate_integral(const double *data, size_t start, size_t end)
{
	double integral = 0.0;
	for (size_t i = start; i < end - 1; i++)
	{
		integral += (data[i] + data[i + 1]) / 2.0; // 台形公式
	}
	return integral;
}

// 積分値を比較し異常を検知する関数
void detect_anomalies_with_integral(const double *reference, const double *target, size_t size)
{
	size_t segment_size = size / SEGMENTS;

	for (size_t i = 0; i < SEGMENTS; i++)
	{
		size_t start = i * segment_size;
		size_t end = (i + 1) * segment_size;

		double ref_integral = calculate_integral(reference, start, end);
		double tgt_integral = calculate_integral(target, start, end);

		if (fabs(ref_integral - tgt_integral) > THRESHOLD_INTEGRAL)
		{
			printf("異常検知: セグメント %zu, リファレンス積分値 %f, 対象積分値 %f\n", i, ref_integral, tgt_integral);
		}
	}
}
//@@@function
void Segment_Integral()
{
	double reference[ARRAY_SIZE];
	double target[ARRAY_SIZE];

	// リファレンスデータ生成
	generate_reference_data(reference, ARRAY_SIZE);

	// 比較対象データの生成（例: ランダムにノイズを追加）
	for (size_t i = 0; i < ARRAY_SIZE; i++)
	{
		target[i] = reference[i] + ((rand() % 100) / 100.0 - 0.5); // ノイズを追加
	}

	// 積分値による異常検知
	detect_anomalies_with_integral(reference, target, ARRAY_SIZE);

	return;
}
