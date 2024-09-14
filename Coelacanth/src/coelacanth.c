#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <dlfcn.h>

// #include "common_func.h"
#include "library_functions.h"
#define MAX_QUEUE_SIZE 100

void clearQueue(int *queue, int *queueSize) {
	// printf("%d is clear\n", *queue);

	*queueSize = 0;
}
void displayQueue(int *queue, int queueSize, int previousQueueSize) {

	for(int back = 0; back < previousQueueSize+2; back++)
	{
		printf("\033[F");// カーソルを一行上に移動
	}
	printf("\033[0J"); // カーソルから右下までクリア

	// printf("\033[2K");
	// printf("\033[F");// カーソルを一行上に移動
	// printf(" |\n");

	for (int i = 0; i < queueSize; i++) {
		if(i != queueSize-1)	printf(" | %s\n", g_FunctionsPtr[queue[i]-1].name);
		else					printf(" v %s\n", g_FunctionsPtr[queue[i]-1].name);

	}

	fflush(stdout); // 出力をフラッシュして即座に表示
}
void displayMenu()
{
	int vertical = 3;
	// メニューを表示
	printf("\nSelect a function to execute:\n");
	// for (int i = 0; i < numFunctions; i++) 
	// {
	// 	printf("\n%d. %s", i+1, functions[i].name);
	// }

    // int n = 20; // 画面に表示する数値の上限
    int i;
	int horizontal = g_numFunctionsPtr/vertical;
	int amari = (g_numFunctionsPtr%vertical==0)?0:1;
    for (i = 1; i <= vertical; i++) 
	{
		for(int j = 0; j < horizontal+amari; j++)
		{
			if(i+j*vertical <= g_numFunctionsPtr)printf("%2d. %-20s|", i+(vertical*j), g_FunctionsPtr[i+(vertical*j)-1].name);
			else{ for(int tmp = 0; tmp < 24; tmp++){printf(" ");}printf("|");}
			
		}
		printf("\n");
	}
	printf("Select a function to execute:\n\n");

}
int main() 
{
	displayMenu();

	int queue[MAX_QUEUE_SIZE] = {};
	int queueSize = 0;

	while(1)
	{
		printf("\n-> ");	
		
		char input[100] = {};
		
		fgets(input, sizeof(input), stdin);

		if (input[0] == 'q') 
		{
			break;
		} 
		else if (input[0] == 'r') 
		{
			displayQueue(queue, 0, queueSize);	//表示されているメニューをクリア
			// for(int back = 0; back < queueSize-2; back++)
			// {
			// 	printf("\033[F");// カーソルを一行上に移動
			// }
			clearQueue(queue, &queueSize);
		}
		else if (input[0] == 's') 
		{
			// キューを実行
			for (int i = 0; i < queueSize; i++) 
			{
				void* func = g_FunctionsPtr[queue[i]-1].func;
				if (func != NULL) 
				{
					((void (*)())func)();
				} 
				else 
				{
					printf("Function not found\n");
				}
			}
			// キューをクリア
			clearQueue(queue, &queueSize);
		} 
		else 
		{
			// 数値が入力された場合、キューに追加
			int previousQueueSize = queueSize;
			char *token = strtok(input, " "); // スペースで区切ってトークンを取得
			while (token != NULL) 
			{
				int selectedFileIndex = atoi(token);
				if ((selectedFileIndex > 0) && (selectedFileIndex <= g_numFunctionsPtr)){
					queue[queueSize] = selectedFileIndex;
					queueSize++;
				}
				token = strtok(NULL, " "); // 次のトークンを取得
			}

			displayQueue(queue, queueSize, previousQueueSize);
		}
	}

	return 0;
}
