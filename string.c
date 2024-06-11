#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// #define MAX_LINES 1000
#define MAX_NAME_LENGTH 50//最大名前長
#define MAX_NAME_COUNT 10//最大名前数

typedef struct {
	char name[MAX_NAME_LENGTH];
	int count;
	double total;
} NameValue;
// 比較関数
int compare_name(const void *a, const void *b) {
    NameValue *nameValueA = (NameValue *)a;
    NameValue *nameValueB = (NameValue *)b;
    return strcmp(nameValueA->name, nameValueB->name);
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
	char line[1024] = {};//一行の長さ

	// Skip the header line
	fgets(line, sizeof(line), inputFile);

	while (fgets(line, sizeof(line), inputFile)) {
		char name[MAX_NAME_LENGTH] = {};
		double value = 0.0;

		// Parse the line to extract name and value
		sscanf(line, "%*d,%49[^,],%lf,%*d", name, &value);//%*dは読み飛ばす.配列長が50なので49まで取得する

		// Check if the name is already in the array
		bool found = false;
		for (int i = 0; i < nameCount; i++) {
			if (strcmp(nameValues[i].name, name) == 0) {
				nameValues[i].total += value;
				nameValues[i].count++;
				found = true;
				break;
			}
		}

		// If the name was not found, add a new entry
		if (found == false && nameCount < MAX_NAME_COUNT) {
			strcpy(nameValues[nameCount].name, name);
			nameValues[nameCount].total = value;
			nameValues[nameCount].count = 1;
			nameCount++;
		}
	}

	// Write the results to the output file
	fprintf(outputFile, "name,average_value\n");
	for (int i = 0; i < nameCount; i++) {
		double average = nameValues[i].total / nameValues[i].count;
		fprintf(outputFile, "%s,%.2f\n", nameValues[i].name, average);
	}
	

	fclose(inputFile);
	fclose(outputFile);

	return 0;
}