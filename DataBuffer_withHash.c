#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define MAX_DATA_LENGTH 50
#define TABLE_SIZE 100

typedef struct Record {
	char name[MAX_NAME_LENGTH];
	char data1[MAX_DATA_LENGTH];
	char data2[MAX_DATA_LENGTH];
	char data3[MAX_DATA_LENGTH];
	struct Record* next;
} Record;

Record* hashTable[TABLE_SIZE];
unsigned int hash(const char* str) {
	unsigned int hashValue = 0;
	while (*str) {
		hashValue += (unsigned int)(*str);
		str++;
	}
	return hashValue % TABLE_SIZE;
}
void insertRecord(Record* record) {
	unsigned int index = hash(record->name);
	record->next = hashTable[index];
	hashTable[index] = record;
}

void loadCSV(const char* filename) {
	FILE* file = fopen(filename, "r");
	if (!file) {
		perror("Failed to open file");
		exit(EXIT_FAILURE);
	}

	char line[256];
	int checkline = 1;
	while (fgets(line, sizeof(line), file)) {
		Record* newRecord = (Record*)malloc(sizeof(Record));
		if (sscanf(line, "%49[^,],%49[^,],%49[^,],",
			newRecord->name, newRecord->data1, newRecord->data2) == 3) {
			insertRecord(newRecord);
			// printf("insert!");
		}
		else {
			free(newRecord);
			printf("Failed to parse line: [line:%d]-> %s", checkline, line);
		}

		checkline++;
	}

	fclose(file);
}
Record* getRecordByName(const char* name) {
	unsigned int index = hash(name);
	Record* current = hashTable[index];
	while (current != NULL) {
		if (strcmp(current->name, name) == 0) {
			return current;
		}
		current = current->next;
	}
	return NULL;
}
void printHashTable() {
	for (int i = 0; i < TABLE_SIZE; i++) {
		Record* current = hashTable[i];
		if (current != NULL) {
			printf("Index %d:\n", i);
			while (current != NULL) {
				printf("  Name: %s, Data1: %s, Data2: %s, Data3: %s\n",
					current->name, current->data1, current->data2, current->data3);
				current = current->next;
			}
		}
	}
}
void printRecord(Record* record) {
	if(record == NULL) {
		printf("Record not found.\n");
		return;
	}
	printf("Name: %s, Data1: %s, Data2: %s, Data3: %s\n",
		record->name, record->data1, record->data2, record->data3);
}
char* getData1byName(const char* name) {
	unsigned int index = hash(name);
	Record* current = hashTable[index];
	while (current != NULL) {
		if (strcmp(current->name, name) == 0) {
			return current->data1;
		}
		current = current->next;
	}
	return NULL;
}
int main() {
	// ハッシュテーブルの初期化
	for (int i = 0; i < TABLE_SIZE; i++) {
		hashTable[i] = NULL;
	}

	// CSVファイルを読み込む
	loadCSV("data.csv");

	// 名前を指定してレコードを検索する
	const char* nameToFind = "Bob";
	printf("\n");
	Record* record = getRecordByName(nameToFind);
	printRecord(record);
	record = getRecordByName("john");
	printRecord(record);

	// 名前を指定してデータを取得する
	const char* nameToGet = "Bob";
	printf("\n");
	char* data1 = getData1byName(nameToGet);
	printf("%s's Data1: %s\n", nameToFind, data1);


	// ハッシュテーブルの内容を出力する
	printHashTable();

	return 0;
}
