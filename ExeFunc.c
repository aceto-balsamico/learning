#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

#define MAX_FILES 100
#define MAX_FILENAME_LENGTH 256

// 関数のプロトタイプ宣言
void listExecutableFiles(const char *folderPath);
void executeFile(const char *filename);
void getFileAtIndex(char *selectedFile, const char *folderPath, int index);
void clearQueue(int *queue, int *queueSize);
void displayQueue(int *queue, int queueSize);

int main() {
    // 実行ファイルが格納されているフォルダのパス
    const char *folderPath = ".";

    // キューとキューのサイズ
    int queue[MAX_FILES];
    int queueSize = 0;

	// 実行ファイルのリストを表示
	listExecutableFiles(folderPath);

	// メニューを表示
	printf("\nSelect a file to queue (enter the corresponding number, 's' to run queue, 'q' to quit, 'r' to clear.)\n");
	printf("Queue: ");

    while(1){
		printf("\n\033[2K-> ");	
		char input[100] = {};
        // scanf("%s", input);
		fgets(input, sizeof(input), stdin);

        if (input[0] == 'q') {
            break;
        } else if (input[0] == 'r') {
            // キューをクリア
            clearQueue(queue, &queueSize);
            // キューを表示
			// printf("\n");
            displayQueue(queue, queueSize);
			printf("\033[2K\rQueue: ");	
		}else if (input[0] == 's') {
            // キューを実行
            for (int i = 0; i < queueSize; ++i) {
                char selectedFile[MAX_FILENAME_LENGTH];
                getFileAtIndex(selectedFile, folderPath, queue[i]);
                executeFile(selectedFile);
            }
            // キューをクリア
            clearQueue(queue, &queueSize);
        } else {
            // 数値が入力された場合、キューに追加
			char *token = strtok(input, " "); // スペースで区切ってトークンを取得
            while (token != NULL) {
				int selectedFileIndex = atoi(token);
				if (selectedFileIndex > 0) {
					queue[queueSize] = selectedFileIndex;
					queueSize++;
				}
				token = strtok(NULL, " "); // 次のトークンを取得
			}
            // キューを表示
            displayQueue(queue, queueSize);
        }
    }

    return 0;
}
// 指定されたフォルダ内の実行ファイルをリストアップする関数
void listExecutableFiles(const char *folderPath) {
    DIR *dir;
    struct dirent *entry;
    char *files[MAX_FILES];
    int count = 0;

    dir = opendir(folderPath);
    if (dir == NULL) {
        perror("Unable to open directory");
        exit(EXIT_FAILURE);
    }

    printf("Executable files in %s:\n", folderPath);

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {  // 通常のファイル
            char filePath[MAX_FILENAME_LENGTH];
            sprintf(filePath, "%s/%s", folderPath, entry->d_name);
            if (access(filePath, X_OK) == 0) {  // 実行可能なファイル
                printf("%3d. %s\n", count + 1, entry->d_name);
                files[count] = strdup(entry->d_name);
                count++;
            }
        }
    }

    closedir(dir);
}

// 指定されたインデックスの実行ファイルを取得する関数
void getFileAtIndex(char *selectedFile, const char *folderPath, int index) {
    DIR *dir;
    struct dirent *entry;
    int count = 0;

    dir = opendir(folderPath);
    if (dir == NULL) {
        perror("Unable to open directory");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {  // 通常のファイル
            char filePath[MAX_FILENAME_LENGTH];
            sprintf(filePath, "%s/%s", folderPath, entry->d_name);
            if (access(filePath, X_OK) == 0) {  // 実行可能なファイル
                count++;
                if (count == index) {
                    strcpy(selectedFile, filePath);
                    break;
                }
            }
        }
    }

    closedir(dir);
}

// 指定されたファイルを実行する関数
void executeFile(const char *filename) {
    printf("Executing %s\n", filename);
    system(filename);
}
void clearQueue(int *queue, int *queueSize) {
    *queueSize = 0;
}
void displayQueue(int *queue, int queueSize) {
	printf("\033[F");// カーソルを一行上に移動
    printf("\033[F");// カーソルを一行上に移動
	
    printf("Queue: ");
    for (int i = 0; i < queueSize; i++) {
        printf("%d ", queue[i]);
    }

    fflush(stdout); // 出力をフラッシュして即座に表示
}
