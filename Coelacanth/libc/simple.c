#include "../src/common_func.h"

int heap_array();
int global_array();
int free_array_all();
//@@@function
void simple()
{
	// printf("simple\n");
	begin_time();
	global_array();printf("global\t");
	end_time();
	printf("\n");
	// begin_time();
	// heap_array();printf("heap\t");
	// end_time();
	printf("\n");
	begin_time();
	free_array_all();printf("freeall\t");
	end_time();
	printf("\n");

	begin_time();
	free_array_all();printf("freeall\t");
	end_time();
	printf("\n");
	begin_time();
	global_array();printf("global\t");
	end_time();
	printf("\n");
	// begin_time();
	// heap_array();printf("heap\t");
	// end_time();
	printf("\n");
	
}

// #include <stdio.h>
// #include <stdlib.h>

#define ARRAY_SIZE 10000
#define ELEMENT_SIZE 10000

// グローバル変数としてポインタ配列を宣言
int* ptr_array[ARRAY_SIZE];

// メモリを一括して解放する関数
void free_all(int** ptr_to_free) {
    free(*ptr_to_free);
}

int free_array_all() {

    // ポインタ配列に動的に確保されたメモリを格納する処理
    for (int i = 0; i < ARRAY_SIZE; i++) {
        ptr_array[i] = (int*)malloc(sizeof(int) * ELEMENT_SIZE);
        if (ptr_array[i] == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
    }

    // 一括解放の計測開始
    int** ptr_to_free = ptr_array;
    free_all(ptr_to_free);

    // 個別解放の計測開始
    // for (int i = 0; i < ARRAY_SIZE; i++) {
    //     free(ptr_array[i]);
    // }

	return 0;
}
typedef struct {
    int data;
    // 他のメンバーがある場合はここに追加
} MyStruct;

int heap_array() {
    // MyStruct* struct_array = (MyStruct*)malloc(sizeof(MyStruct*)*ARRAY_SIZE);
    MyStruct* struct_array[ARRAY_SIZE];

    // ポインタ配列に構造体を動的に確保

	// begin_time();
    for (int i = 0; i < ARRAY_SIZE; i++) {
        struct_array[i] = (MyStruct*)malloc(sizeof(MyStruct));
        if (struct_array[i] == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        // 構造体に値を設定（ここでは0で初期化）
        struct_array[i]->data = 0;
    }
	// end_time();printf("->malloc");


    // 確保したメモリを使用して何らかの処理を行う

    // メモリ解放
	// begin_time();
    for (int i = 0; i < ARRAY_SIZE; i++) {
        free(struct_array[i]);
    }
	// end_time();printf("->free time");

    return 0;
}


// グローバル変数としてポインタ配列を宣言
MyStruct* struct_array[ARRAY_SIZE];

int global_array() {
    // ポインタ配列を初期化
    for (int i = 0; i < ARRAY_SIZE; i++) {
        struct_array[i] = NULL;
    }

    // ポインタ配列に構造体のアドレスを格納
    for (int i = 0; i < ARRAY_SIZE; i++) {
        struct_array[i] = (MyStruct*)malloc(sizeof(MyStruct));
        if (struct_array[i] == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        // 構造体に値を設定（ここでは0で初期化）
        struct_array[i]->data = 0;
    }

    // 確保したメモリを使用して何らかの処理を行う

    // メモリ解放は必要ない（プログラムの終了時に自動的に解放される）

    return 0;
}
