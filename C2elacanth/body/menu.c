#include "custom_common.h"
#include "library_functions.h"

void DisplayMenu(s_FunctionPtr *copyFunctionsPtr, int N_displayFunctions, const char *searchString);
void coelacanth_menu(char* searchString);
void clearQueue(int *queue, int *queueSize);
void displayQueue(int *queue, int queueSize, int previousQueueSize);

void userfunc(int argc, char* argv[])
{
	if(argc > 1)
	{
		coelacanth_menu(argv[1]);
	}
	else
	{
		coelacanth_menu("All");
	}
}

void coelacanth_menu(char* searchString)
{	
	bool found = false;
	s_FunctionPtr **copyFunctionsPtr = (s_FunctionPtr **)malloc(g_numFunctionsPtr * sizeof(s_FunctionPtr *));
	int N_displayFunctions = 0;
	if (copyFunctionsPtr == NULL)
	{
		printf("malloc error\n");
		exit(1);
	}
	for (int i = 0; i < g_numFunctionsPtr; i++)
	{
		// printf("%s\n", g_FunctionsPtr[i].subdir_name);
		// printf("%p\n", g_FunctionsPtr[i].func);
		// printf("%s\n", copyFunctionsPtr[N_displayFunctions].func_name);
		if (strstr(g_FunctionsPtr[i].func_name, searchString) || strstr(g_FunctionsPtr[i].subdir_name, searchString) || strcmp(searchString, "All") == 0)
		{
			copyFunctionsPtr[N_displayFunctions] = &g_FunctionsPtr[i];
			N_displayFunctions++;
			found = true;
		}
	}
	if (found == false)
	{
		printf("No matching functions found for '%s'.\n", searchString);
		return;
	}

	DisplayMenu(*copyFunctionsPtr, N_displayFunctions, searchString);

	int queue[MAX_QUEUE_SIZE] = {};
	int queueSize = 0;

	while (1)
	{
		printf("\n-> ");

		char input[100] = {};

		if (fgets(input, sizeof(input), stdin) == NULL)
		{
			printf("fgets return EOF(ctrl+d)\n");
			exit(1);
		}

		if (input[0] == 'q')
		{
			break;
		}
		else if (input[0] == 'r')
		{
			displayQueue(queue, 0, queueSize); // 表示されているメニューをクリア
			clearQueue(queue, &queueSize);
		}
		else if (input[0] == 's')
		{
			// キューを実行
			begin_time();
			for (int i = 0; i < queueSize; i++)
			{
				void *func = copyFunctionsPtr[queue[i] - 1]->func;
				if (func != NULL)
				{
					((void (*)())func)();
				}
				else
				{
					printf("Function not found\n");
				}
			}
			printf("\nTest Elapsed Time: %8.3lf ms\n", end_time_ms());
			// キューをクリア
			clearQueue(queue, &queueSize);
			DisplayMenu(*copyFunctionsPtr, N_displayFunctions, searchString);
		}
		else
		{
			// 数値が入力された場合、キューに追加
			int previousQueueSize = queueSize;
			char *token = strtok(input, " ,"); // スペースで区切ってトークンを取得
			while (token != NULL)
			{
				// ハイフンが含まれているか確認
				if (strchr(token, '-') != NULL)
				{
					// ハイフン区切りの範囲を解析
					int start, end;
					if (sscanf(token, "%d-%d", &start, &end) == 2)
					{
						// 範囲が正しい順番であるかチェック
						if (start > 0 && end >= start && end <= N_displayFunctions && queueSize < MAX_QUEUE_SIZE)
						{
							for (int i = start; i <= end && queueSize < MAX_QUEUE_SIZE; i++)
							{
								queue[queueSize++] = i;
							}
						}
					}
				}
				else
				{
					// 通常の数値トークンの場合
					int selectedFileIndex = atoi(token);
					if ((selectedFileIndex > 0) && (selectedFileIndex <= N_displayFunctions) && (queueSize < MAX_QUEUE_SIZE))
					{
						queue[queueSize++] = selectedFileIndex;
					}
				}
				token = strtok(NULL, " ,"); // 次のトークンを取得
			}

			displayQueue(queue, queueSize, previousQueueSize);
		}
	}

	free(copyFunctionsPtr);
}
// サブディレクトリ名の部分一致で検索し、ヒットした関数を一覧表示する
void DisplayMenu(s_FunctionPtr *copyFunctionsPtr, int N_displayFunctions, const char *searchString)
{

	printf("=== Functions matching '%s' ===\n", searchString);

	int vertical = 10;
	int horizontal = N_displayFunctions / vertical;
	int amari = (N_displayFunctions % vertical == 0) ? 0 : 1;
	for (int i = 1; i <= vertical; i++)
	{
		for (int j = 0; j < horizontal + amari; j++)
		{
			if (i + j * vertical <= N_displayFunctions)
				printf("%2d. %-20s|", i + (vertical * j), copyFunctionsPtr[i + (vertical * j) - 1].func_name);
			else
			{
				for (int tmp = 0; tmp < 24; tmp++)
				{
					printf(" ");
				}
				printf("|");
			}
		}
		printf("\n");
	}
}

void clearQueue(int *queue, int *queueSize)
{
	// printf("%d is clear\n", *queue);

	*queueSize = 0;
}
void displayQueue(int *queue, int queueSize, int previousQueueSize)
{

	for (int back = 0; back < previousQueueSize + 2; back++)
	{
		printf("\033[F"); // カーソルを一行上に移動
	}
	printf("\033[0J"); // カーソルから右下までクリア

	for (int i = 0; i < queueSize; i++)
	{
		if (i != queueSize - 1)
			printf(" | %s\n", g_FunctionsPtr[queue[i] - 1].func_name);
		else
			printf(" v %s\n", g_FunctionsPtr[queue[i] - 1].func_name);
	}

	fflush(stdout); // 出力をフラッシュして即座に表示
}