
#include "common_determination.h"

int main()
{
	srand(time(NULL));

	//グローバルのmalloc_manager1をローカルポインタで持っておく
	MemoryManager *MainFunctionEnd = &malloc_manager1;
	Init_MemoryManager(MainFunctionEnd);

	//g_blkinfoの作成と時間計測
	begin_time();
	DeterminationBlockArea();
	end_time();
	printf("\n");

	// g_blkinfoの全内容を表示
	print_block_info(&g_blkinfo);
	for(int i = 0; i < N_BLOCK; i++)printf("%4d,", Validblock[i]);
	printf("\n");

	//SOA_BlockInfo2dimの中から一つ取り出すためのローカル構造体を定義
	SOA_BI2_individual target_BI2;
	//g_blknfoのdata1メンバのなかから最も小さい値のインデックスを取り出す
	int minimum_data1 = Choice_BI2_Index(g_blkinfo.data1, g_blkinfo.used_size, MIN_VALUE);
	//引数で指定したインデックスの構造体を取り出す
	Get_BI2_individual(&target_BI2, minimum_data1);
	printf("MIN_VALUE  target_BI2 -> [%d], ", minimum_data1);
	print_BI2(&target_BI2);

	//取り出した構造体のメンバにフラグを立てる
	g_blkinfo.flag_add[minimum_data1] = TRUE_VALUE;

	//Trueフラグを立った構造体を同じ手法で取り出す
	int true_flag_add = Choice_BI2_Index(g_blkinfo.flag_add, g_blkinfo.used_size, TRUE_VALUE);
	Get_BI2_individual(&target_BI2, true_flag_add);
	//取り出せているか確認
	printf("TRUE_VALUE target_BI2 -> [%d], ", true_flag_add);
	print_BI2(&target_BI2);

	// malloc_manager1のメモリを解放し、ここに来るまでCustom_mallocで確保したメモリをフリーする
	printf("\n");
	Custom_Free_All(MainFunctionEnd);
	//malloc_manager1を破壊
	Destroy_MemoryManager(MainFunctionEnd);

	// GetRandom関数のコールカウントを表示
	printf("g_getrandom_call_count: %d\n", g_GetRandom_CallCount);

	return 0;
}

void swap(int* x, int* y)
{
	int buffer = 0;
	buffer = *x;
	*x = *y;
	*y = buffer;
}
int GetRandom(int min, int max)
{
	if (min > max)
	{
		swap(&min, &max);
	}

	g_GetRandom_CallCount++;

	return rand() % (max - min + 1) + min;
}
// メニューの初期化
void Init_MENU(MENU *menu)
{
	menu->total_probability = 0;
	menu->count = 0;
}

//よく使うのでメニューは1種類のみグローバルにおいておき、使い回す。メニューの要素数はdefineしておく
void Append_MENU(MENU *menu, int key, int probability)
{
	if (menu->count >= MAX_MENU_SIZE)
	{
		printf("Error: Menu size exceeded\n");
		return;
	}
	menu->entries[menu->count].key = key;
	menu->entries[menu->count].probability = probability;
	menu->total_probability += probability;
	menu->count++;
}
//メニューからランダムに選んで返す
int Choice_Select_Rate_MENU(MENU *menu)
{
	int random_value = GetRandom(0, menu->total_probability - 1);
	int sum = 0;

	for (int i = 0; i < menu->count; i++)
	{
		sum += menu->entries[i].probability;
		if (random_value < sum)
		{
			return menu->entries[i].key;
		}
	}

	return -1; // エラー処理（ここには来ないはず）
}
//SOAのハブになる構造体を初期化。大きめにメモリを確保しておく。
void Init_SOA_BlockInfo2dim(SOA_BlockInfo2dim *blkinfo, int max_size)
{
	blkinfo->max_size = max_size;
	blkinfo->used_size = 0;

	// 後でreallocするのでmallocを使用する
	blkinfo->BlkAddrList 	= (int**)malloc(sizeof(int*) * max_size);
	blkinfo->N_Multi 		= (int*)malloc(sizeof(int) * max_size);
	blkinfo->data1 			= (int*)malloc(sizeof(int) * max_size);
	blkinfo->data2 			= (int*)malloc(sizeof(int) * max_size);
	blkinfo->flag_add 		= (bool*)malloc(sizeof(bool) * max_size);

	Validblock = (int*)Custom_Malloc(N_BLOCK * sizeof(int), &malloc_manager1);
}
//SOAにデータを追加していく
void Append_SOA_BlockInfo2dim(SOA_BlockInfo2dim *blkinfo, int* addr, int multi, int d1, int d2)
{
	if (blkinfo->used_size < blkinfo->max_size)
	{
		blkinfo->BlkAddrList[blkinfo->used_size] = addr;
		blkinfo->N_Multi[blkinfo->used_size] = multi;
		blkinfo->data1[blkinfo->used_size] = d1;
		blkinfo->data2[blkinfo->used_size] = d2;
		blkinfo->flag_add[blkinfo->used_size] = false; // 初期値はfalse
		blkinfo->used_size++;
	}
	else
	{
		printf("max_size: %d, used_size: %d, ", blkinfo->max_size, blkinfo->used_size);
		printf("Error: No more space available in the structure. Append_SOA_BlockInfo2dim\n");
		exit(1);
	}
}
//SOAの長さを固定する。大きめに確保したメモリをreallocで小さし、使う以外のメモリを破棄する。ここではCustomReallocを使用し、メモリリーク対策する
void Fix_SOA_BlockInfo2dim(SOA_BlockInfo2dim *blkinfo)
{
	SOA_BlockInfo2dim check_realloc = {};
	check_realloc.BlkAddrList = (int* *)Custom_Realloc(blkinfo->BlkAddrList, sizeof(int*) * blkinfo->used_size, &malloc_manager1);
	check_realloc.N_Multi = (int*)Custom_Realloc(blkinfo->N_Multi, sizeof(int) * blkinfo->used_size, &malloc_manager1);
	check_realloc.data1 = (int*)Custom_Realloc(blkinfo->data1, sizeof(int) * blkinfo->used_size, &malloc_manager1);
	check_realloc.data2 = (int*)Custom_Realloc(blkinfo->data2, sizeof(int) * blkinfo->used_size, &malloc_manager1);
	check_realloc.flag_add = (bool *)Custom_Realloc(blkinfo->flag_add, sizeof(bool) * blkinfo->used_size, &malloc_manager1);

	if (check_realloc.BlkAddrList == NULL || check_realloc.N_Multi == NULL || check_realloc.data1 == NULL || check_realloc.data2 == NULL)
	{
		// realloc失敗
		printf("Error: No more space available in the structure. Fix_SOA_BlockInfo2dim\n");
		free(blkinfo->BlkAddrList);
		free(blkinfo->N_Multi);
		free(blkinfo->data1);
		free(blkinfo->data2);
		free(blkinfo->flag_add);
		exit(1);
	}
	else
	{
		// realloc成功
		blkinfo->BlkAddrList = check_realloc.BlkAddrList;
		blkinfo->N_Multi = check_realloc.N_Multi;
		blkinfo->data1 = check_realloc.data1;
		blkinfo->data2 = check_realloc.data2;
		blkinfo->flag_add = check_realloc.flag_add;
		blkinfo->max_size = blkinfo->used_size;
	}
}

// MemoryManagerの初期化。Destroy_MemoryManagerとペアで使う。
void Init_MemoryManager(MemoryManager *manager)
{
	manager->allocs = malloc(INITIAL_CAPACITY * sizeof(void*));
	if (manager->allocs == NULL)
	{
		perror("Failed to initialize memory manager");
		exit(1);
	}
	manager->size = 0;
	manager->capacity = INITIAL_CAPACITY;
}

// カスタムmalloc: 確保したメモリの先頭アドレスを配列に入れて保存し、任意のタイミングで一斉にフリーする。mallocを参考に作成したのでアドレスの型はsize_tで宣言した
void* Custom_Malloc(size_t size, MemoryManager *manager)
{
	if (manager->size == manager->capacity)
	{
		void* origin_ptr = manager->allocs;
		// 配列が限界に達したら、reallocでサイズを倍に拡張
		manager->allocs = realloc(manager->allocs, manager->capacity * sizeof(void*));
		if (manager->allocs == NULL)
		{
			printf("Failed to reallocate memory");
			free(origin_ptr);
			exit(1);
		}
		printf("capacity expantion: %zu", manager->capacity);
		manager->capacity *= 2;
		printf(" -> %zu\n", manager->capacity); // size_t(unsigned long)は%zu
	}

	void* ptr = malloc(size);
	if (ptr != NULL)
	{
		manager->allocs[manager->size++] = ptr; // 配列にアドレスを保存
	}
	return ptr;
}
//カスタムmallocのrealloc版
void* Custom_Realloc(void* ptr, size_t size, MemoryManager *manager)
{
	if (manager->size == manager->capacity)
	{
		void* origin_ptr = manager->allocs;
		// 配列が限界に達したら、reallocでサイズを倍に拡張
		manager->allocs = realloc(manager->allocs, manager->capacity * sizeof(void*));
		if (manager->allocs == NULL)
		{
			printf("Failed to reallocate memory");
			free(origin_ptr);
			exit(1);
		}
		printf("capacity expantion: %zu", manager->capacity);
		manager->capacity *= 2;
		printf(" -> %zu\n", manager->capacity); // size_t(unsigned long)は%zu
	}

	void* check_ptr = realloc(ptr, size);
	if (check_ptr != NULL)
	{
		manager->allocs[manager->size++] = check_ptr; // 配列にアドレスを保存
	}
	else
	{
		// realloc失敗
		printf("Error: No more space available in the structure. Custom_Realloc\n");
		exit(1);
	}
	return check_ptr;
}
// カスタムfree: 配列内の全てのメモリを解放
void Custom_Free_All(MemoryManager *manager)
{
	for (size_t i = 0; i < manager->size; i++)
	{
		if (manager->allocs[i] != NULL)
		{
			free(manager->allocs[i]);  // メモリ解放
			manager->allocs[i] = NULL; // 二重freeを防止
		}
	}
	// printf("free count:%zu\n", manager->size);
	manager->size = 0; // 配列をリセット
}

// MemoryManagerの破棄
void Destroy_MemoryManager(MemoryManager *manager)
{
	Custom_Free_All(manager);
	free(manager->allocs); 
	manager->allocs = NULL;
	manager->capacity = 0;
}
// Fisher-Yatesアルゴリズムで配列をシャッフルする関数
void Shuffle_Array(int* array, int size)
{
	// 配列の要素をランダムにシャッフル
	for (int i = size - 1; i > 0; i--)
	{
		// 0からiまでのランダムなインデックスを生成
		// int j = rand() % (i + 1);
		int j = GetRandom(0, i);

		// array[i] と array[j] を交換
		int temp = array[i];
		array[i] = array[j];
		array[j] = temp;
	}
}
// シャッフル関数をあらゆる配列の型に対応させた版。第三引数に配列の型を指定する
void General_Shuffle_Array(void* array, size_t element_count, size_t element_size) 
{
	char* arr = (char*)array; // 1byteで管理したいのでvoid*をchar*にキャスト
	for (size_t i = element_count - 1; i > 0; i--) 
	{
		// ランダムにインデックスを選択
		srand(0xAA);
		size_t j = GetRandom(0, i);
		
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


//TRIM Block処理はまだ
void determinationBlockArea_body(int* Validblock)
{
	// この関数内で使用するmallocを管理する用
	MemoryManager manager_determination;
	Init_MemoryManager(&manager_determination);

	// Block数がPlane数で割り切れない製品が来たときの処理
	int mod_block = N_BLOCK % N_PLANE;
	// Plane数分のBlockアドレス配列を確保。
	int* addr_array[N_PLANE] = {};
	for (int plane = 0; plane < N_PLANE; plane++)
	{
		addr_array[plane] = (int*)Custom_Malloc((N_BLOCK / N_PLANE + 1) * sizeof(int), &manager_determination);
	}

	// 全BlockをPlaneにわける
	for (int BlkNum = 0; BlkNum < N_BLOCK; BlkNum++)
	{
		addr_array[BlkNum % N_PLANE][BlkNum / N_PLANE] = BlkNum;
	}

	// Blockのアドレスをシャッフル
	for (int plane = 0; plane < N_PLANE; plane++)
	{
		if (plane < mod_block)
			Shuffle_Array(addr_array[plane], (N_BLOCK / N_PLANE) + 1);
		else
			Shuffle_Array(addr_array[plane], (N_BLOCK / N_PLANE));
	}

	// ランダム選択する用のPlaneアドレスを保持する配列の確保
	int* N_Multi_Array = (int*)Custom_Malloc(N_PLANE * sizeof(int), &manager_determination);
	for (int plane = 0; plane < N_PLANE; plane++)
	{
		N_Multi_Array[plane] = plane;
	}

	int used_block_counter = 0;
	int N_Multi_counter[N_PLANE] = {};
	int addr_counter[N_PLANE] = {};
	while (1)
	{
		// Planeの順番をシャッフル
		Shuffle_Array(N_Multi_Array, N_PLANE);

		// Multi動作数を決定。第一引数はmenuをグローバルにするならいらないかも
		Init_MENU(&menu);
		Append_MENU(&menu, 1, RATE_1PLANE);
		Append_MENU(&menu, 2, RATE_2PLANE);
		Append_MENU(&menu, 3, RATE_3PLANE);
		Append_MENU(&menu, 4, RATE_4PLANE);
		Append_MENU(&menu, 5, RATE_5PLANE);
		Append_MENU(&menu, 6, RATE_6PLANE);

		// 何Plane動作かをランダム選択
		int Multi_Ope = Choice_Select_Rate_MENU(&menu);
		// ValidBlockリストを先頭から順番に指し示すようにポインタを設定
		int* select = &Validblock[used_block_counter];
		// ランダム選択されたPlaneのBlockをaddr配列から一つずつ取り出していき、select配列に格納
		int real_plane_counter = 0;
		for (int rand_plane = 0; rand_plane < Multi_Ope; rand_plane++)
		{
			if (N_Multi_Array[rand_plane] < mod_block)
			{
				if (addr_counter[N_Multi_Array[rand_plane]] == (N_BLOCK / N_PLANE) + 1)
				{
					continue; // 対象のPlaneのBlcokが枯渇したらスキップ
				}
			}
			else
			{
				if (addr_counter[N_Multi_Array[rand_plane]] == N_BLOCK / N_PLANE)
				{
					continue; // 対象のPlaneのBlcokが枯渇したらスキップ
				}
			}

			// ランダム選択されたPlaneのBlockをaddr配列から一つずつ取り出していき、select配列に格納
			select[real_plane_counter] = addr_array[N_Multi_Array[rand_plane]][addr_counter[N_Multi_Array[rand_plane]]];

			// addr_counter配列の値をインクリメント
			addr_counter[N_Multi_Array[rand_plane]]++;

			// select配列に追加できたら(配列作成できたら)カウンターをインクリメント
			real_plane_counter++;
		}
		if (real_plane_counter == 0)
		{
			continue; // 何も追加されなかったらスキップ
		}

		// 実際にN Plane動作のリストがいくつ作られたかカウントする
		N_Multi_counter[real_plane_counter - 1]++;

		Append_SOA_BlockInfo2dim(&g_blkinfo, select, real_plane_counter, GetRandom(-1, 10), GetRandom(0, 100)); // data1, data2にはダミーデータとして100までのランダムな整数を入れておく

		// リストに追加されたBlockの数をカウントする。これがN_BLOCKに達したら終了
		used_block_counter += real_plane_counter;
		if (used_block_counter == N_BLOCK)
		{
			printf("used_blk_counter: %d, Used list size: %d, Max size: %d\n", used_block_counter, g_blkinfo.used_size, g_blkinfo.max_size);
			for (int i = 0; i < N_PLANE; i++)
			{
				printf("N_Multi_counter %d_PlaneOperation: %d\n", i + 1, N_Multi_counter[i]);
			}
			break;
		}
		else if (used_block_counter > N_BLOCK)
		{
			printf("Error: determinationBlockArea body\n");
			exit(1);
		}
	}

	// この関数で使用したmallocをfreeする
	Custom_Free_All(&manager_determination);
	Destroy_MemoryManager(&manager_determination);
}

void DeterminationBlockArea()
{
	SOA_BlockInfo2dim *BlkInfo2dim = &g_blkinfo;
	Init_SOA_BlockInfo2dim(BlkInfo2dim, N_BLOCK);

	// データの追加
	determinationBlockArea_body(Validblock);

	// printf("Used size: %d, Max size: %d\n", blkinfo.used_size, blkinfo.max_size);
	Fix_SOA_BlockInfo2dim(BlkInfo2dim);
	// printf("Used size: %d, Max size: %d\n", blkinfo.used_size, blkinfo.max_size);	//メモリぴったりにreallocできたことを確認
}
void print_block_info(SOA_BlockInfo2dim *blkinfo)
{
	printf("Used size: %d, Max size: %d\n", blkinfo->used_size, blkinfo->max_size);
	for (int i = 0; i < blkinfo->used_size; i++)
	{
		printf("[%4d]:", i);
		printf("N_Multi: %d, ", blkinfo->N_Multi[i]);
		printf("data1: %4d, ", blkinfo->data1[i]);
		printf("data2: %4d, ", blkinfo->data2[i]);
		printf("flag_add: %d, ", blkinfo->flag_add[i]);
		printf("BlkAddrList: ");
		for (int j = 0; j < blkinfo->N_Multi[i]; j++)
		{
			printf("%4d(%d),", blkinfo->BlkAddrList[i][j], blkinfo->BlkAddrList[i][j] % N_PLANE);
		}
		printf("\n");
	}
}
void Get_BI2_individual(SOA_BI2_individual *target_BI2, int N_index)
{
	if (N_index >= g_blkinfo.used_size)
	{
		printf("Error: too large N_index. Get_BI2_individual");
		return;
	}
	target_BI2->BlkAddrList = g_blkinfo.BlkAddrList[N_index];
	target_BI2->N_Multi = g_blkinfo.N_Multi[N_index];
	target_BI2->data1 = g_blkinfo.data1[N_index];
	target_BI2->data2 = g_blkinfo.data2[N_index];
	target_BI2->flag_add = g_blkinfo.flag_add[N_index];
}
// BlockInfo構造体のあらゆる型に対応したいのでジェネリックポインタで受ける
int Choice_BI2_Index(void* array, int size, SEARCH_TYPE search_type)
{
	if (size <= 0)
	{
		printf("Array size must be greater than 0. find_random_min_index\n");
		return -1;
	}

	int target_value = 0;
	int* buffer = (int*)Custom_Malloc(sizeof(int) * size, &malloc_manager1);
	switch (search_type)
	{
		case MIN_VALUE:
		case MAX_VALUE:
		{
			target_value = ((int*)array)[0];
			break;
		}
		case FALSE_VALUE:
		case TRUE_VALUE:
		{
			target_value = ((bool *)array)[0];
			break;
		}
		default:
		{
			printf("Error: invalid search_type. Choice_BI2_Index\n");
			return -1;
		}
	}

	int buffer_count = 0;
	bool flag_found = false;
	switch (search_type)
	{
		case MIN_VALUE:
		{
			for (int i = 0; i < size; i++)
			{
				if (((int*)array)[i] < target_value)
				{
					target_value = ((int*)array)[i];
					buffer[0] = i;
					buffer_count = 1;
					flag_found = true;
				}
				else if (((int*)array)[i] == target_value)
				{
					buffer[buffer_count] = i;
					buffer_count++;
					flag_found = true;
				}
			}
			break;
		}
		case MAX_VALUE:
		{
			for (int i = 0; i < size; i++)
			{
				if (((int*)array)[i] > target_value)
				{
					target_value = ((int*)array)[i];
					buffer[0] = i;
					buffer_count = 1;
					flag_found = true;
				}
				else if (((int*)array)[i] == target_value)
				{
					buffer[buffer_count] = i;
					buffer_count++;
					flag_found = true;
				}
			}
			break;
		}
		case FALSE_VALUE:
		{
			target_value = false;
			for (int i = 0; i < size; i++)
			{
				if (((bool *)array)[i] == false)
				{
					buffer[buffer_count] = i;
					buffer_count++;
					flag_found = true;
				}
			}
			break;
		}
		case TRUE_VALUE:
		{
			target_value = true;
			for (int i = 0; i < size; i++)
			{
				if (((bool *)array)[i] == true)
				{
					buffer[buffer_count] = i;
					buffer_count++;
					flag_found = true;
				}
			}
			break;
		}
		default:
		{
			printf("Error: Not Supported search type: not found!\n");
			return -1;
		}
	}
	if (flag_found == false)
	{
		printf("Error: search_random_index: not found!\n");
		return -1;
	}
	int random_index = GetRandom(0, buffer_count - 1);
	return buffer[random_index];
}

void print_BI2(SOA_BI2_individual *target_BI2)
{
	printf("N_Multi: %d, data1: %d, data2: %d, flag: %d, ", target_BI2->N_Multi, target_BI2->data1, target_BI2->data2, target_BI2->flag_add);
	printf("BlkAddrList: ");
	for (int i = 0; i < target_BI2->N_Multi; i++)
	{
		printf("%4d(%d),", target_BI2->BlkAddrList[i], target_BI2->BlkAddrList[i] % N_PLANE);
	}
	printf("\n");
}
