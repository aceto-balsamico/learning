#include "custom_common.h"


// 単純なバブルソート関数（整数専用）
void simple_sort(void *array, size_t element_count, size_t element_size)
{
	char *arr = (char *)array; // ジェネリックポインタをchar型にキャスト
	for (size_t i = 0; i < element_count - 1; i++)
	{
		for (size_t j = 0; j < element_count - i - 1; j++)
		{
			// 2つの要素を比較
			if (memcmp(arr + j * element_size, arr + (j + 1) * element_size, element_size) > 0)
			{
				// 要素を交換
				char temp[element_size];
				memcpy(temp, arr + j * element_size, element_size);
				memcpy(arr + j * element_size, arr + (j + 1) * element_size, element_size);
				memcpy(arr + (j + 1) * element_size, temp, element_size);
			}
		}
	}
}
void General_Shuffle_Array(void* array, size_t element_count, size_t element_size) 
{
	char* arr = (char*)array; // 1byteで管理したいのでvoid*をchar*にキャスト
	for (size_t i = element_count - 1; i > 0; i--) 
	{
		// ランダムにインデックスを選択
		srand(0xBB);
		size_t j = rand() % (i + 1);
		
		char* element_i = arr + (i * element_size);
		char* element_j = arr + (j * element_size);
		
		char* temp = (char*)malloc(element_size);
		if (temp == NULL) 
		{
			printf("malloc error\n");
			exit(1);
		}
		
		memcpy(temp, element_i, element_size);
		memcpy(element_i, element_j, element_size);
		memcpy(element_j, temp, element_size);
		
		free(temp);
	}
}
//@@@function
void sort_and_shuffle()
{
	// 整数型の配列
	int int_array[] = {5, 1, 4, 2, 8};
	size_t int_count = sizeof(int_array) / sizeof(int_array[0]);

	// ソートを実行
	simple_sort(int_array, int_count, sizeof(int));

	// ソート結果を表示
	printf("Sorted integers: ");
	for (size_t i = 0; i < int_count; i++)
	{
		printf("%d ", int_array[i]);
	}
	printf("\n");

	General_Shuffle_Array(int_array, int_count, sizeof(int));

	// ソート結果を表示
	printf("Shuffled integers: ");
	for (size_t i = 0; i < int_count; i++)
	{
		printf("%d ", int_array[i]);
	}
	printf("\n");

}