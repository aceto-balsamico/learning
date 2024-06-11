#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NAME_COUNT 100
#define MAX_NAME_LENGTH 50

typedef struct {
	char name[MAX_NAME_LENGTH];
	double total;
	int count;
} NameValue;

// 名前と値の構造体を降順に並べるための比較関数
int compare_value(const void* a, const void* b) {
	NameValue* nameValueA = (NameValue*)a;
	NameValue* nameValueB = (NameValue*)b;
	double averageA = nameValueA->total / nameValueA->count;
	double averageB = nameValueB->total / nameValueB->count;
	return (averageB > averageA) - (averageB < averageA); // 降順に並べるための比較
}

int main(int argc, char* argv[]) {
	FILE* inputFile = NULL;
	if (argc > 1) {
		inputFile = fopen(argv[1], "r");
		if (inputFile == NULL) {
			fprintf(stderr, "Error opening file.\n");
			return 1;
		}
	}
	FILE* outputFile = fopen("output.csv", "w");
	if (inputFile == NULL || outputFile == NULL) {
		fprintf(stderr, "Error opening file.\n");
		return 1;
	}

	NameValue nameValues[MAX_NAME_COUNT] = {};
	int nameCount = 0;
	char line[1024] = {}; // 一行の長さ

	// ヘッダー行をスキップ
	fgets(line, sizeof(line), inputFile);

	while (fgets(line, sizeof(line), inputFile)) {
		char name[MAX_NAME_LENGTH] = {};
		double value = 0.0;

		// 行を解析して名前と値を抽出
		sscanf(line, "%*d,%49[^,],%lf,%*d", name, &value); // %*dは読み飛ばす.配列長が50なので49まで取得する

		// 名前がすでに配列に存在するか確認
		bool found = false;
		for (int i = 0; i < nameCount; i++) {
			if (strcmp(nameValues[i].name, name) == 0) {
				nameValues[i].total += value;
				nameValues[i].count++;
				found = true;
				break;
			}
		}

		// 名前が見つからなかった場合、新しいエントリを追加
		if (found == false && nameCount < MAX_NAME_COUNT) {
			strcpy(nameValues[nameCount].name, name);
			nameValues[nameCount].total = value;
			nameValues[nameCount].count = 1;
			nameCount++;
		}
	}

	// 平均値を降順に並べ替える
	qsort(nameValues, nameCount, sizeof(NameValue), compare_value);

	// 結果を出力ファイルに書き込む
	fprintf(outputFile, "name,average_value\n");
	for (int i = 0; i < nameCount; i++) {
		double average = nameValues[i].total / nameValues[i].count;
		fprintf(outputFile, "%s,%.2f\n", nameValues[i].name, average);
	}

	fclose(inputFile);
	fclose(outputFile);

	return 0;
}
