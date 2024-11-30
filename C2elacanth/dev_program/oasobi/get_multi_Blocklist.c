#include "custom_common.h"
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
        free(line_copy);
        return -1; // 数値ではない場合
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
void GetMultiBlocklist() {
    char input[256];
    int array[MAX_ELEMENTS];
    int expected_base = 0;  // 連番の基準

    printf("行ごとに入力してください (例: 0:4,0x100,0x101,0x102,0x103)。終了するには空行を入力。\n");

    while (1) {
        // 入力を取得
        if (!fgets(input, sizeof(input), stdin)) {
            perror("Error reading input");
            return;
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
            return;
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

}