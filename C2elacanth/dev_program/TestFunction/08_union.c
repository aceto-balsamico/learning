#include <stdio.h>

typedef struct {
    union {
        unsigned char uc_array[10];  // unsigned char型の配列（10要素）
        unsigned int ui_array[1000]; // unsigned int型の配列（1000要素）
    } data; // 共用体
    int is_using_ui; // どちらを使用中かを示すフラグ (0: uc_array, 1: ui_array)
} DataContainer;

// デモ関数
void demo() {
    DataContainer container;

    // unsigned char型配列を使用する例
    container.is_using_ui = 0; // フラグを設定
    for (int i = 0; i < 10; i++) {
        container.data.uc_array[i] = (unsigned char)(i + 65); // A, B, C...
    }
    printf("Using unsigned char array:\n");
    for (int i = 0; i < 10; i++) {
        printf("%c ", container.data.uc_array[i]);
    }
    printf("\n");

    // unsigned int型配列を使用する例
    container.is_using_ui = 1; // フラグを設定
    for (int i = 0; i < 1000; i++) {
        container.data.ui_array[i] = i * 10;
    }
    printf("Using unsigned int array:\n");
    for (int i = 0; i < 10; i++) { // 10個だけ出力
        printf("%d ", container.data.ui_array[i]);
    }
    printf("\n");
}
//@@@function
void union_example() 
{
    demo();
}