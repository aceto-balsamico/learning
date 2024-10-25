#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 任意の配列をシャッフルするFisher-Yatesアルゴリズム関数
void shuffle_array(void *array, size_t element_count, size_t element_size) {
    char *arr = (char *)array; // void*をchar*にキャスト
    for (size_t i = element_count - 1; i > 0; i--) {
        // ランダムにインデックスを選択
		srand(0xAA);
        size_t j = rand() % (i + 1);
        
        // 配列内の要素をスワップ
        char *elem_i = arr + (i * element_size);
        char *elem_j = arr + (j * element_size);
        
        // スワップには一時バッファを使用
        char *temp = (char *)malloc(element_size);
        if (temp == NULL) {
            perror("メモリ割り当てに失敗しました");
            exit(EXIT_FAILURE);
        }
        
        memcpy(temp, elem_i, element_size);
        memcpy(elem_i, elem_j, element_size);
        memcpy(elem_j, temp, element_size);
        
        free(temp);
    }
}

typedef struct testtest
{
	int a;
	int b;
	int c;
}TEST;


int main() {
    srand(time(NULL));

    // 使用例: int型の配列をシャッフル
    int int_array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	unsigned char char_array[] = {1,2,3,4,5,6,7,8,9,10};
	TEST test_array[] = {{1,1,1},{2,2,2},{3,3,3},{4,4,4},{5,5,5},{6,6,6},{7,7,7},{8,8,8},{9,9,9},{10,10,10}};

    size_t int_count = sizeof(int_array) / sizeof(int_array[0]);

    
    printf("シャッフル前: ");
    for (size_t i = 0; i < int_count; i++) {
        printf("%d ", int_array[i]);
    }
    printf("\n");

    shuffle_array(int_array, int_count, sizeof(int));
	shuffle_array(char_array, int_count, sizeof(unsigned char));
	shuffle_array(test_array, int_count, sizeof(TEST));
    
    printf("シャッフル後: ");
    for (size_t i = 0; i < int_count; i++) {
        printf("%d ", int_array[i]);
    }
    printf("\n");

	printf("シャッフル後: ");
	for (size_t i = 0; i < int_count; i++) {
		printf("%d ", char_array[i]);
	}
	printf("\n");

	printf("シャッフル後: ");
	for (size_t i = 0; i < int_count; i++) {
		printf("%d ", test_array[i].a);
	}
	printf("\n");

    return 0;
}