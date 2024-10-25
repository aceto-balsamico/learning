#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_MACHINES 4
#define MAX_LOOPS 30

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
void shuffle_array(MachineData array[], int start, int count) {
    for (int i = 0; i < count; i++) {
        int j = start + rand() % count;
        MachineData temp = array[start + i];
        array[start + i] = array[j];
        array[j] = temp;
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
		if(rand() % 2 == 0) {
			shuffle_array(out_data, out_index, read_count);
			printf("(shuffle)");
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