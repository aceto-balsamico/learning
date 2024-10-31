#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "common.h"

#define NUM_MACHINES 4
#define MAX_LOOPS 810

//1BlockTLC = 810*3=2430Page  *3Loop=7290
//Ave:405*3=1215  *2Loop=2430

typedef struct {
    int machine_id;
    int key[3];
} MachineData;

// ランダムなkeyを生成
void generate_key(int key[]) {
    for (int i = 0; i < 3; i++) {
        key[i] = rand() % 100; // 0〜99の範囲でランダムなkey
    }
}
void shuffle_array2(void *array, size_t element_count, size_t element_size) {
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

		// if(last.machine_id == array[start].machine_id)
		// {
		// 	// printf("same\n");
		// 	int random_element = rand() % (count-1) + start+1;
		// 	printf("count:%d,random:%d\n", count, random_element);
		// 	last 			= array[start];
		// 	array[start] 	= array[random_element];
		// 	array[random_element] = last;
		// }
        
        free(temp);
    }
}
// 構造体配列を初期化し、keyと機械IDを設定
void initialize_data(MachineData data[], int size) {
    int last_machine_id = -1; // 初回は不正な値で設定
    for (int i = 0; i < size; i++) {
        int machine_id = rand() % NUM_MACHINES;
        
        // 同じ機械IDが連続しないように設定
        while (machine_id == last_machine_id) {
            machine_id = rand() % NUM_MACHINES;
        }

        data[i].machine_id = machine_id;
        generate_key(data[i].key); // keyを生成
        last_machine_id = machine_id;
    }
}

// Read動作の表示
void perform_read(const MachineData *machine) {
    printf("Read: Machine ID = %d, Key = [%d, %d, %d]\n",
           machine->machine_id, machine->key[0], machine->key[1], machine->key[2]);
}

// Out動作の表示
void perform_out(const MachineData *machine) {
    printf("Out: Machine ID = %d, Key = [%d, %d, %d]\n",
           machine->machine_id, machine->key[0], machine->key[1], machine->key[2]);
}

// 配列の範囲内でシャッフル
void shuffle_array1(MachineData array[], int start, int count) 
{
	MachineData last = array[start + count - 1];
    for (int i = 0; i < count; i++) 
	{
        int j = start + rand() % count;
        MachineData temp = array[start + i];
        array[start + i] = array[j];
        array[j] = temp;
    }
	// printf("last:%d, first:%d\n", last.machine_id, array[start].machine_id);
	if(last.machine_id == array[start].machine_id)
	{
		// printf("same\n");
		int random_element = rand() % (count-1) + start+1;
		// printf("count:%d,random:%d\n", count, random_element);
		last 			= array[start];
		array[start] 	= array[random_element];
		array[random_element] = last;
	}
}

int main() {
    srand(time(NULL));

    MachineData read_data[MAX_LOOPS];
    MachineData out_data[MAX_LOOPS];

    // 初期化
    initialize_data(read_data, MAX_LOOPS);
    for (int i = 0; i < MAX_LOOPS; i++) {
        out_data[i] = read_data[i]; // Out用配列にコピー
    }

    int read_index = 0; // Read配列の先頭インデックス
    int out_index = 0;  // Out配列の先頭インデックス

    // 全データの処理が終了するまでループ
    while (read_index < MAX_LOOPS) {
        int read_count = 1;
        int seen_machines[NUM_MACHINES] = {0}; // 機械IDの重複チェック用
        seen_machines[read_data[read_index].machine_id] = 1;

        // 機械IDが重ならない範囲でReadを実行
        while(1) {
            if (read_index + read_count >= MAX_LOOPS) {
                break;
            }

            int current_machine_id = read_data[read_index + read_count].machine_id;
            if (seen_machines[current_machine_id]) {
                break;
            }

            seen_machines[current_machine_id] = 1;
            read_count++;
        }

        // Read動作の出力
        printf("\n--- Read Phase ---\n");
        for (int i = 0; i < read_count; i++) {
            perform_read(&read_data[read_index + i]);
        }

        // Out配列の該当範囲をシャッフル
		if(read_count > 2) //2回のときシャッフルしても意味ないので3回以上でシャッフル
		{
			if(rand() % 2 == 0) 
			{
				// begin_time(1);
				shuffle_array1(out_data, out_index, read_count);
				// shuffle_array2(&out_data[out_index], read_count, sizeof(MachineData));
				// printf("\n");
				// end_time(1);
				printf("(shuffle)");
			}
		}
        // shuffle_array(out_data, out_index, read_count);

        // Out動作の出力
        printf("--- Out Phase ---\n");
        for (int i = 0; i < read_count; i++) {
            perform_out(&out_data[out_index + i]);
        }

        // インデックスを更新
        read_index += read_count;
        out_index += read_count;
    }

    printf("\nすべてのデータ処理が終了しました。\n");
    return 0;
}