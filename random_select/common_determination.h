#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

/* define */
#define INITIAL_CAPACITY 10
#define MAX_MENU_SIZE 64 // メニューの最大数

/* 列挙型 */
typedef enum _config
{
	N_PLANE = 4,
	N_BLOCK = 10,
} CONFIG;

typedef enum _serch_type
{
	FALSE_VALUE = 0,
	TRUE_VALUE = 1,
	MIN_VALUE = 2,
	MAX_VALUE = 3,
} SEARCH_TYPE;

typedef enum _multi_plane_operation_rate
{
	// アルゴリズムの性質上1Plane動作のみを設定するのは可能だが、例えば4Plane動作のみを設定しても1,2,3動作が選択されることがある。
	RATE_1PLANE = 3,
	RATE_2PLANE = 2,
	RATE_3PLANE = 3,
	RATE_4PLANE = 4,
	RATE_5PLANE = 0,
	RATE_6PLANE = 0,
} MULTI_PLANE_OPERATION_RATE;

/* 構造体 */
typedef struct _memory_manager
{
	void **allocs;	 // メモリアドレスの配列
	size_t size;	 // 現在の配列サイズ
	size_t capacity; // 配列の最大容量
} MemoryManager;

typedef struct _blkinfo2dim
{
	int **BlkAddrList;
	int *N_Multi;
	int *data1;
	int *data2;
	bool *flag_add;

	int max_size;
	int used_size;

} SOA_BlockInfo2dim;

typedef struct _blkinfo2dim_individual
{
	int *BlkAddrList;
	int N_Multi;
	int data1;
	int data2;
	bool flag_add;
} SOA_BI2_individual;

typedef struct
{
	int key;
	int probability;
} MenuEntry;

typedef struct
{
	MenuEntry entries[MAX_MENU_SIZE];
	int total_probability; // 全体の確率合計
	int count;			   // メニュー項目数
} MENU;

/* グローバル変数 */
unsigned int g_GetRandom_CallCount;
MENU menu;
MemoryManager malloc_manager1;
SOA_BlockInfo2dim g_blkinfo;
int *Validblock;

/* 関数の外部宣言 */
extern void swap(int *x, int *y);
extern int GetRandom(int min, int max);
extern void Init_MENU(MENU *menu);
extern void Append_MENU(MENU *menu, int key, int probability);
extern int Choice_Select_Rate_MENU(MENU *menu);
extern void Init_SOA_BlockInfo2dim(SOA_BlockInfo2dim *blkinfo, int max_size);
extern void Append_SOA_BlockInfo2dim(SOA_BlockInfo2dim *blkinfo, int *addr, int multi, int d1, int d2);
extern void Fix_SOA_BlockInfo2dim(SOA_BlockInfo2dim *blkinfo);
extern void Init_MemoryManager(MemoryManager *manager);
extern void *Custom_Malloc(size_t size, MemoryManager *manager);
extern void *Custom_Realloc(void *ptr, size_t size, MemoryManager *manager);
extern void Custom_Free_All(MemoryManager *manager);
extern void Destroy_MemoryManager(MemoryManager *manager);
extern void Shuffle_Array(int *array, int size);
extern void determinationBlockArea_body(int *Validblock);
extern void DeterminationBlockArea();
extern void print_block_info(SOA_BlockInfo2dim *blkinfo);
extern void Get_BI2_individual(SOA_BI2_individual *target_BI2, int N_index);
extern int Choice_BI2_Index(void *array, int size, SEARCH_TYPE search_type);
extern void print_BI2(SOA_BI2_individual *target_BI2);

/* 汎用関数処理時間測定用 */
double time_spent, begin, end;
struct timespec ts1, ts2;

void begin_time()
{
	clock_gettime(CLOCK_MONOTONIC, &ts1);
}

void end_time()
{
	clock_gettime(CLOCK_MONOTONIC, &ts2);
	printf(" elapsed time is %8.3lf [ms]", (ts2.tv_sec - ts1.tv_sec) * 1000.0 + (ts2.tv_nsec - ts1.tv_nsec) / 1000000.0); // milisec order
}