#include "custom_common.h"

#define NUM_MACHINES 2
#define MAX_OPERATIONS 40  // 全体で行うReadおよびOutの最大数

// 機械の状態を表す構造体
typedef struct {
    int machine_id;
    bool is_reading; // trueならRead待ち、falseならOut待ち
    int key1, key2, key3; // Readで使用したキーを保持
} Machine;

void perform_read(int machine_id, int *key1, int *key2, int *key3) {
    *key1 = rand() % 100;
    *key2 = rand() % 100;
    *key3 = rand() % 100;
    printf("Machine %d: Read (Keys: %d, %d, %d)\n", machine_id, *key1, *key2, *key3);
}

void perform_out(int machine_id, int key1, int key2, int key3) {
    printf("-Machine %d: Out (Keys: %d, %d, %d)\n", machine_id, key1, key2, key3);
}
//@@@function
void machine_sim_2() {
    srand(time(NULL));
    Machine machines[NUM_MACHINES];
    int total_operations = 0;

    // 初期化: 各機械をRead待ち状態にする
    for (int i = 0; i < NUM_MACHINES; i++) {
        machines[i].machine_id = i;
        machines[i].is_reading = true;
    }

    // メインループ: MAX_OPERATIONS回まで操作を繰り返す
    while (total_operations < MAX_OPERATIONS) {
        // ランダムな機械IDを選択
        int machine_index = rand() % NUM_MACHINES;
        Machine *current_machine = &machines[machine_index];

        if (current_machine->is_reading) {
            // 現在の機械がRead待ちの場合、Readを実行し、キーを保存
            perform_read(current_machine->machine_id, &current_machine->key1, &current_machine->key2, &current_machine->key3);
            current_machine->is_reading = false; // 次はOut待ちに設定
        } else {
            // 現在の機械がOut待ちの場合、保存されたキーでOutを実行
            perform_out(current_machine->machine_id, current_machine->key1, current_machine->key2, current_machine->key3);
            current_machine->is_reading = true; // 次はRead待ちに設定
        }

        total_operations++; // 操作数をカウント
    }

    printf("シミュレーション終了: 合計 %d 回の操作が行われました。\n", total_operations);
}