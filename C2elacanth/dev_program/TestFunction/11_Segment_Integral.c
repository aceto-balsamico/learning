#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SEGMENTS 8
#define DATA_SIZE 512
#define THRESHOLD_PERCENTAGE 0.2  // リファレンス比率での閾値
#define SLOPE_THRESHOLD 0.5      // 傾き評価の閾値

// アスキーアート出力関数
void print_ascii_art(double *reference, double *target, size_t size, size_t width) {
    double max_value = -1e9, min_value = 1e9;

    // データ範囲を求める
    for (size_t i = 0; i < size; i++) {
        if (reference[i] > max_value) max_value = reference[i];
        if (target[i] > max_value) max_value = target[i];
        if (reference[i] < min_value) min_value = reference[i];
        if (target[i] < min_value) min_value = target[i];
    }

    double scale = (max_value - min_value) / (double)(width - 1);
    size_t compression_factor = size / width;

    printf("\nリファレンス vs ターゲット (アスキーアート)\n");
    for (size_t i = 0; i < width; i++) {
        size_t start = i * compression_factor;
        size_t end = (i + 1) * compression_factor;
        if (i == width - 1) end = size;

        // 区間の平均値を計算
        double ref_avg = 0.0, tgt_avg = 0.0;
        for (size_t j = start; j < end; j++) {
            ref_avg += reference[j];
            tgt_avg += target[j];
        }
        ref_avg /= (end - start);
        tgt_avg /= (end - start);

        // アスキーアートで描画
        int ref_pos = (int)((ref_avg - min_value) / scale);
        int tgt_pos = (int)((tgt_avg - min_value) / scale);

        for (int j = 0; j < width; j++) {
            if (j == ref_pos && j == tgt_pos) {
                printf("*");  // 両方のデータが一致する場合
            } else if (j == ref_pos) {
                printf("R");  // リファレンスの位置
            } else if (j == tgt_pos) {
                printf("T");  // ターゲットの位置
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

// 異常検知方法を実装する関数
void detect_anomalies(double *reference, double *target, size_t size) {
    size_t segment_size = size / SEGMENTS;
    double total_integral = 0.0, total_squared = 0.0;
    double mean, stddev;

    // 1. リファレンスデータの統計情報を計算
    for (size_t i = 0; i < size - 1; i++) {
        double integral = (reference[i] + reference[i + 1]) / 2.0;
        total_integral += integral;
        total_squared += integral * integral;
    }
    mean = total_integral / SEGMENTS;
    stddev = sqrt(total_squared / SEGMENTS - mean * mean);

    // 各セグメントごとに異常検知
    for (size_t seg = 0; seg < SEGMENTS; seg++) {
        size_t start = seg * segment_size;
        size_t end = start + segment_size;

        // リファレンスとターゲットの積分値を計算
        double ref_integral = 0.0, tgt_integral = 0.0;
        for (size_t i = start; i < end - 1; i++) {
            ref_integral += (reference[i] + reference[i + 1]) / 2.0;
            tgt_integral += (target[i] + target[i + 1]) / 2.0;
        }

		printf("Segment %zu:\n", seg);
        // リファレンス比率による閾値
        if (fabs(ref_integral - tgt_integral) > fabs(ref_integral) * THRESHOLD_PERCENTAGE) {
            printf("[割合閾値検知]   セグメント %zu 異常: リファレンス %f, 対象 %f\n", seg, ref_integral, tgt_integral);
        }
		else
		{
			printf("[割合閾値検知]   セグメント %zu 正常: リファレンス %f, 対象 %f\n", seg, ref_integral, tgt_integral);
		}

        // 正規化評価
        double diff_rate = fabs(ref_integral - tgt_integral) / (fabs(ref_integral) + 1e-6);
        if (diff_rate > THRESHOLD_PERCENTAGE) {
            printf("[正規化評価検知] セグメント %zu 異常: 差異率 %f, リファレンス %f, 対象 %f\n", seg, diff_rate, ref_integral, tgt_integral);
        }
		else
		{
			printf("[正規化評価検知] セグメント %zu 正常: 差異率 %f, リファレンス %f, 対象 %f\n", seg, diff_rate, ref_integral, tgt_integral);
		}

        // 平均・標準偏差評価
        if (fabs(tgt_integral - mean) > 2 * stddev) {
            printf("[統計的手法検知] セグメント %zu 異常: 積分値 %f, 平均 %f, 標準偏差 %f\n", seg, tgt_integral, mean, stddev);
        }
		else
		{
			printf("[統計的手法検知] セグメント %zu 正常: 積分値 %f, 平均 %f, 標準偏差 %f\n", seg, tgt_integral, mean, stddev);
		}

        // 傾き評価
        double ref_slope = reference[end - 1] - reference[start];
        double tgt_slope = target[end - 1] - target[start];
        if (fabs(ref_slope - tgt_slope) > SLOPE_THRESHOLD) {
            printf("[傾き評価検知]   セグメント %zu 異常: リファレンス傾き %f, 対象傾き %f\n", seg, ref_slope, tgt_slope);
        }
		else
		{
			printf("[傾き評価検知]   セグメント %zu 正常: リファレンス傾き %f, 対象傾き %f\n", seg, ref_slope, tgt_slope);
		}

		printf("\n");
    }
}

//@@@function
int Segment_Integral() 
{
    double reference[DATA_SIZE];
    double target[DATA_SIZE];

    // リファレンスとターゲットのサンプルデータを生成
    for (size_t i = 0; i < DATA_SIZE; i++) {
        reference[i] = 1.0 / (1.0 + exp(-((double)i - 256.0) / 40.0)); // シグモイド
        target[i] = reference[i] + ((rand() % 400 - 200) / 500.0);    // ノイズ付与
        if (i % 128 == 0) target[i] += 0.5;                            // 大きな異常値を意図的に追加
    }

    // 異常検知
    detect_anomalies(reference, target, DATA_SIZE);

	print_ascii_art(reference, target, DATA_SIZE, 50);


    return 0;
}
/* 
リファレンス積分値の割合を基準にする

メリット
	•	データのスケールが大きく変わる場合（リファレンスが非常に大きい、または小さい）でも適切に異常を検出できる。
	•	「20%外れたら異常」という評価基準が直感的で理解しやすい。

注意点
	•	リファレンス積分値が0付近（非常に小さい値）の場合、閾値が過度に小さくなる可能性があります。この場合は最小閾値を設定する対策が必要です。

各セグメントのリファレンス積分値と対象積分値を正規化し、その差を基準に異常検知を行う方法です。

メリット
	•	正規化することで、各セグメントの値のスケールが異なっていても一貫性を保てる。
	•	スケール依存の影響を排除。

リファレンスデータ全体または各セグメント内の積分値の平均値と標準偏差を計算し、標準偏差を基準に異常を検出します。

メリット
	•	統計的な基準を用いるため、異常検出が客観的。
	•	特に異常な変動が稀なデータセットで有効。

注意点
	•	リファレンスデータの分布が極端に偏っている場合、正確な平均値や標準偏差を計算できないことがある。

積分値の差だけでなく、データの局所的な傾き（勾配）も評価する方法です。リファレンスと対象データの傾きの差が大きい場合も異常と判定します。

メリット
	•	積分値と傾きの両方を組み合わせることで、変化の特徴をより正確に捉えられる。
	•	値が急激に変動する異常も検知可能。
 */