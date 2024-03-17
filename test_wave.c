#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define MAX_LINE_LENGTH 1000 // CSVファイルの1行の最大長
#define THRESHOLD 0.5 // 値の閾値

// CSVファイルから周期性を持つ波形の1周期の長さを取得する関数
int get_period_length(FILE *input_file, int start_line) {
    char line[MAX_LINE_LENGTH];
    double previous_value = 0.0;
    int line_count = start_line;

    // start_lineまで読み飛ばす
    for (int i = 0; i < start_line; i++) {
        if (!fgets(line, MAX_LINE_LENGTH, input_file)) {
            printf("Error: Unable to read from input file\n");
            return -1;
        }
    }

    // CSVファイルからデータを読み取り、周期の長さを取得
    while (fgets(line, MAX_LINE_LENGTH, input_file)) {
        double value;
        sscanf(line, "%*lf,%lf", &value);

        // 閾値を基準に値が大と小を判断
        bool is_large = (value >= THRESHOLD);

        // 前回の値と異なる場合、1周期の終了を検出
        if (line_count > start_line && is_large != (previous_value >= THRESHOLD)) {
            return line_count - start_line;
        }

        previous_value = value;
        line_count++;
    }

    // 1周期の長さが見つからない場合
    printf("Error: Unable to find period length!\n");
    return -1;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s input.csv output.csv\n", argv[0]);
        return 1;
    }

    char *input_filename = argv[1];
    char *output_filename = argv[2];

    FILE *input_file = fopen(input_filename, "r");
    if (!input_file) {
        printf("Error: Unable to open input file %s\n", input_filename);
        return 1;
    }

    FILE *output_file = fopen(output_filename, "w");
    if (!output_file) {
        printf("Error: Unable to create output file %s\n", output_filename);
        fclose(input_file);
        return 1;
    }

    char line[MAX_LINE_LENGTH];
	int i = 0;
	int linenum = 0;
    // 元のCSVファイルから1行ずつ読み取り、新しいCSVファイルに書き込む
    while (fgets(line, MAX_LINE_LENGTH, input_file)) {
        // fputs(line, output_file);

        // 各行をカンマで分割
        char *token = strtok(line, ",");
        while (token != NULL) {
            // 数値を文字列から整数に変換して、新しいCSVファイルに追記
            int value = atoi(token);
            fprintf(output_file, "%d,", value);

            // 次のトークンに進む
            token = strtok(NULL, ",");
        }

		fprintf(output_file, "%d,", i);
		i++;
		if(i == get_period_length(input_file, linenum)){i = 0;}
        // // 1波長ごとにリセットされる数値を3列目に追記
        // for (int i = 0; i < 8; i++) {
        //     fprintf(output_file, ",%d", i);
        // }

        fprintf(output_file, "\n"); // 行末に改行を追加
    }

    fclose(input_file);
    fclose(output_file);
    printf("Output written to %s\n", output_filename);
    return 0;
}
