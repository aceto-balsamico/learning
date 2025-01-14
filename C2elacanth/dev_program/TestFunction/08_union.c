#include "custom_common.h"
typedef struct {
    unsigned char id;      // ID
    unsigned char value;   // データ値
} SmallStruct;

typedef struct {
    unsigned int id;       // ID
    unsigned int values[1000]; // データ配列（大きいデータ）
} LargeStruct;

typedef struct {
    union {
        SmallStruct small; // 小さい構造体
        LargeStruct large; // 大きい構造体
    } data; // 共用体
    int is_large; // フラグ (0: SmallStruct, 1: LargeStruct)
} DataContainer;

// デモ関数
void demo() {
    DataContainer container;

    // SmallStruct を使用
    container.is_large = 0; // フラグを設定
    container.data.small.id = 1;
    container.data.small.value = 42;
    printf("Using SmallStruct: id = %d, value = %d\n",
           container.data.small.id, container.data.small.value);

    // LargeStruct を使用
    container.is_large = 1; // フラグを設定
    container.data.large.id = 1000;
    for (int i = 0; i < 10; i++) {
        container.data.large.values[i] = i * 10;
    }
    printf("Using LargeStruct: id = %d, values[0..9] =",
           container.data.large.id);
    for (int i = 0; i < 10; i++) {
        printf(" %d", container.data.large.values[i]);
    }
    printf("\n");
}

//@@@function
void union_example() 
{
    demo();
}