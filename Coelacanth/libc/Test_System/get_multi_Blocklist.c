#include "common_func.h"

#define MAX_ELEMENTS 17  // 要素数16 + 1

// 1行の文字列を解析して配列に格納する関数
int parse_line(const char *line, int *array) {
    int base_num, count, i;
    unsigned int value;
    char *token, *endptr, *line_copy;

    // コピーを作成して解析
    line_copy = strdup(line);
    if (line_copy == NULL) {
        perror("Memory allocation failed");
        return -1;
    }

    // ベース番号と数値の個数を取得
    token = strtok(line_copy, ":");
    base_num = strtol(token, &endptr, 10);
    if (*endptr != '\0') {
		if(strcmp(token, "start") != 0 && strcmp(token, "stop") != 0){
			free(line_copy);
			return -1; // 数値ではない場合
		}
    }

    token = strtok(NULL, ", ");
    count = strtol(token, &endptr, 10);
    if (*endptr != '\0' || count > 16) {  // countが16を超える場合はエラー
        free(line_copy);
        return -1;
    }

    // ベース番号と個数を配列に格納
    array[0] = base_num;
    array[1] = count;

    // 16進数の値を解析して格納
    for (i = 0; i < count; i++) {
        token = strtok(NULL, ", ");
        if (token == NULL) {
            free(line_copy);
            return -1; // 必要な数値が不足している場合
        }
        value = strtoul(token, &endptr, 16);
        if (*endptr != '\0') {
            free(line_copy);
            return -1; // 16進数でない場合
        }
        array[2 + i] = value;
    }

    // 残りの配列を0で埋める
    for (; i < 16; i++) {
        array[2 + i] = 0;
    }

    free(line_copy);
    return 0;
}
//@@@function
int GetMultiBlocklist() {
    char input[256];
    int array[MAX_ELEMENTS];
    int expected_base = 0;  // 連番の基準

    printf("行ごとに入力してください (例: 0:4,0x100,0x101,0x102,0x103)。終了するには空行を入力。\n");

    while (1) {
        // 入力を取得
        if (!fgets(input, sizeof(input), stdin)) {
            perror("Error reading input");
            return 1;
        }

        // 改行を取り除く
        input[strcspn(input, "\n")] = '\0';

        // 空行なら終了
        if (strlen(input) == 0) {
            break;
        }

        // 配列に格納
        if (parse_line(input, array) != 0) {
            fprintf(stderr, "Error parsing line: %s\n", input);
            return 1;
        }

        // 連番が途切れた場合終了
        if (array[0] != expected_base) {
            break;
        }

        // 配列内容を表示
        printf("Base: %d, Count: %d, Values:", array[0], array[1]);
        for (int i = 0; i < array[1]; i++) {
            printf(" [0x%x]", array[2 + i]);
        }
        printf("\n");

        // 次の連番を期待値に設定
        expected_base++;
    }

    return 0;
}


// 指定された step に従って値を生成する関数
void generate_sequence(const int *start, const int *stop, int step, int result[][MAX_ELEMENTS], int *num_steps) {
    int count = start[1]; // 数値の個数
    *num_steps = 0;

    // 初期値をコピー
    int temp[MAX_ELEMENTS];
    memcpy(temp, start + 2, count * sizeof(int));

    while (1) {
        // 現在の値を結果に格納
        for (int i = 0; i < count; i++) {
            result[*num_steps][i] = temp[i];
        }
        (*num_steps)++;

        // 次のステップを計算
        int should_stop = 1;
        for (int i = 0; i < count; i++) {
            temp[i] += step;
            if (temp[i] <= stop[2 + i]) {
                should_stop = 0; // 継続
            }
        }

        if (should_stop || *num_steps >= MAX_ELEMENTS) {
            break; // 停止条件
        }
    }
}

//@@@function
int RangeMultiBlock() {
    char start_line[] = "start:5, 1, 2, 3, 4, 5";
    char stop_line[] = "stop:5, 21, 22, 23, 24, 25";
    int step = 4;

    int start[MAX_ELEMENTS + 2], stop[MAX_ELEMENTS + 2];
    int result[MAX_ELEMENTS][MAX_ELEMENTS];
    int num_steps;

    // 入力を解析
    if (parse_line(start_line, start) == -1 || parse_line(stop_line, stop) == -1) {
        fprintf(stderr, "Failed to parse input lines.\n");
        return EXIT_FAILURE;
    }

    // シーケンスを生成
    generate_sequence(start, stop, step, result, &num_steps);

    // 結果を表示
    printf("Generated sequence:\n");
    for (int i = 0; i < num_steps; i++) {
        printf("Step %d: ", i + 1);
        for (int j = 0; j < start[1]; j++) {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }

    return 0;
}