#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#define N 128
#define CURVATURE_THRESHOLD 0.1 // 曲率の閾値

// シグモイド関数
double sigmoid(double x)
{
	return 1.0 / (1.0 + exp(-x));
}

// 曲率を計算する (改良版)
double calculate_curvature(double x1, double y1, double x2, double y2, double x3, double y3)
{
	double dx1 = x2 - x1, dy1 = y2 - y1; // 1次微分
	double dx2 = x3 - x2, dy2 = y3 - y2; // 2次微分

	double numerator = fabs(dx1 * dy2 - dy1 * dx2);		  // 曲率の分子
	double denominator = pow(dx1 * dx1 + dy1 * dy1, 1.5); // 曲率の分母

	if (denominator < 1e-9)
	{ // 分母がゼロに近い場合を防ぐ
		return 0.0;
	}

	return numerator / denominator;
}

// 局所的な異常検知
bool is_local_outlier(double curvature, double threshold)
{
	return curvature > threshold;
}
//@@@function
void curvature_detection()
{
	double x[N], y[N];
	double curvatures[N - 2];

	// データ生成 (谷状のシグモイド)
	for (int i = 0; i < N; i++)
	{
		x[i] = i; // x: 0 ~ N-1
		if (i < N / 2)
			y[i] = sigmoid(x[i] - N / 2); // 上り基調
		else
			y[i] = sigmoid(-(x[i] - N / 2)); // 下り基調

		// デバッグ出力: xとyの確認
		printf("x[%d] = %.2f, y[%d] = %.5f\n", i, x[i], i, y[i]);
	}

	// 曲率を計算
	for (int i = 1; i < N - 1; i++)
	{
		curvatures[i - 1] = calculate_curvature(x[i - 1], y[i - 1], x[i], y[i], x[i + 1], y[i + 1]);
		// デバッグ出力: 曲率の確認
		printf("Curvature at index %d: %.5f\n", i, curvatures[i - 1]);
	}

	// 曲率に基づいて飛び値を検出
	for (int i = 0; i < N - 2; i++)
	{
		if (is_local_outlier(curvatures[i], CURVATURE_THRESHOLD))
		{
			printf("Outlier detected at index %d: Curvature=%.5f\n", i, curvatures[i]);
		}
	}

	return;
}