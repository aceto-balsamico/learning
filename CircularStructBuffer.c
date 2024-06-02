#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int data;      // データ
    bool flag;     // 読み出されたかどうかのフラグ
} MyStruct;

typedef struct {
    MyStruct *pool;   // 構造体の配列
    int next_index;   // 次に配置するインデックス
    int used_count;   // 使用中の構造体数
    int size;         // メモリプールのサイズ
	int top_index;
	int end_index;
} MemoryPool;

// メモリプールの初期化
MemoryPool* init_memory_pool(int size) {
    MemoryPool *mp = (MemoryPool*)malloc(sizeof(MemoryPool));
    mp->pool = (MyStruct*)malloc(sizeof(MyStruct) * size);
    mp->next_index = 0;
    mp->used_count = 0;
    mp->size = size;
    for (int i = 0; i < size; i++) {
        mp->pool[i].flag = true;  // 最初はすべて読み出された状態
    }
	mp->top_index = 0;
	mp->end_index = 0;

	// printf("Pool size: %d (%zu Byte)\n", size, size * sizeof(MyStruct));
    return mp;
}

// メモリプールの解放
void free_memory_pool(MemoryPool *mp) {
    free(mp->pool);
    free(mp);
}

// 構造体の挿入
void insert_struct(MemoryPool *mp, int data) {
    if (mp->used_count == mp->size || mp->pool[mp->next_index].flag == false) {
        // 読み出されていない場合、80%読み出し
        int to_read_out = (mp->size * 80) / 100;
        for (int i = 0; i < to_read_out; i++) {
            // int index = (mp->next_index + i) % mp->size;
            int index = (mp->next_index + i) & (mp->size-1);
            if (mp->pool[index].flag == false) {
                mp->pool[index].flag = true;
                mp->used_count--;
            }
        }
		mp->top_index = (mp->next_index + to_read_out) & (mp->size-1);
    }

    // 構造体の配置
    mp->pool[mp->next_index].data = data;
    mp->pool[mp->next_index].flag = false;
    // mp->next_index = (mp->next_index + 1) % mp->size;	//sizeが2のべき乗でない場合剰余計算する
    mp->next_index = (mp->next_index + 1) & (mp->size-1);//sizeが2のべき乗なら&演算で剰余計算できる
    mp->used_count++;
	mp->end_index = mp->next_index;
}

// 上書き可能な構造体数の取得
int get_overwrite_count(MemoryPool *mp) {
    return mp->size - mp->used_count;
}

// メモリプールの状態出力
void print_memory_pool(MemoryPool *mp) {
    printf("Memory Pool State:\n");
    for (int i = 0; i < mp->size; i++) {
        if (i % 30 == 0) {
            if (i == 0) {
                printf("Index: ");
            } else {
                printf("\n       ");
            }
        }
        if (mp->pool[i].flag == true) {
            printf(" true ");
        } else {
            printf("%5d ", mp->pool[i].data);
        }
    }
    printf("\n");
    printf("Overwriteable struct count: %d/%d (used:%d), top_index:%d, end_index:%d\n", get_overwrite_count(mp), mp->size, mp->used_count, mp->top_index, mp->end_index);
}
void read_allstruct(MemoryPool *mp) {
	int index = 0;
	for(int i=0;i< mp->used_count;i++){
		index = (mp->top_index + i) & (mp->size-1);
		// printf("data:%d\n",mp->pool[index].data);
		mp->pool[index].flag = true;
	}
	mp->used_count = 0;
	mp->top_index = 0;
	mp->end_index = 0;
	
}
// メイン関数
#include "common.h"
int main() {
	// print_memory_pool(mp);

    // 構造体を挿入
    // for (int i = 0; i < 120; i++) {
    //     insert_struct(mp, i);
    // }

    // メモリプールの状態を出力
    // print_memory_pool(mp);
	double ave = 0.0;
	int test_count = 100;	//100回繰り返して平均を出す
	int test_struct_count = 100000000;//1億個の構造体を挿入する
	for(int size = 2; size < 1024; size*=2)
	{
		ave = 0.0;
		for(int i = 0; i < test_count; i++)
		{
			MemoryPool *mp = init_memory_pool(size*1024*1024);//2のべき乗にする.32M個の構造体.多くしすぎると他の関数のメモリを圧迫するためむしろ時間がかかるようになる。
			begin_time(1);
			for (int i = 0; i < test_struct_count; i++) {
				insert_struct(mp, i);
			}
			ave += end_time_return();
			free_memory_pool(mp);
		}
		printf("size:%d, ave:%f ms\n", size, ave/test_count);
	}
	// print_memory_pool(mp);
	
	// read_allstruct(mp);
	// print_memory_pool(mp);

    // メモリプールの解放

    return 0;
}
