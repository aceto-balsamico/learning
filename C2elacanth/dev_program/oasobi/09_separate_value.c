#include "custom_common.h"

void shuffle_array(int *array, int size) {
	for (int i = size - 1; i > 0; i--) {
		int j = rand() % (i + 1); // 0からiまでのランダムなインデックス
		// 配列の要素を交換
		int temp = array[i];
		array[i] = array[j];
		array[j] = temp;
	}
}
void random_split_even(int value, int separate, int *result)
{
	// 配列を2で初期化（偶数の最低値を確保）
	for (int i = 0; i < separate; i++)
	{
		result[i] = 2;
	}

	// 残りの値を計算
	int remaining = value - 2 * separate;

	// separate回だけ値を割り振る
	for (int i = 0; i < separate && remaining > 0; i++)
	{
		int max_add = (remaining / 2) * 2; // 残り値以下の最大偶数
		if (i == separate - 1)
		{
			// 最後の要素には残りをすべて割り当てる
			result[i] += remaining;
		}
		else if (max_add > 0)
		{
			// ランダムに偶数を選択（最小値2以上、最大値max_add以下）
			int add = (rand() % (max_add / 2 + 1)) * 2;
			result[i] += add;
			remaining -= add; // 残り値を更新
		}
	}
	shuffle_array(result, separate);
}
void print_array(const char *title, int *array, int size)
{
	int sum = 0;
	printf("%s: ", title);
	for (int i = 0; i < size; i++)
	{
		printf("%d ", array[i]);
		sum += array[i];
	}
	printf("\n");
	printf("sum:%d\n", sum);
}

//@@@function
void separate_value()
{
	int value = 20000; // 初期値
	int separate = 10; // 分割数
	int result1[separate];

	// srand((unsigned int)time(NULL)); // 乱数シード

	random_split_even(value, separate, result1);
	print_array("1st Split", result1, separate);

}