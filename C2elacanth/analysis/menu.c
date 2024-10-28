#include "custom_common.h"
#include "library_functions.h"

void DisplayMenu(const char *searchString);
void analysis_menu()
{
	No6_Program();
	No8_Erase();
	common_B8func();

	// DisplayMenu("No6");
	DisplayMenu("No8");
	DisplayMenu("Program");
}

// サブディレクトリ名の部分一致で検索し、ヒットした関数を一覧表示する
void DisplayMenu(const char *searchString)
{
	printf("=== Functions matching '%s' ===\n", searchString);

	bool found = false;
	void *funcPtr;
	for (int i = 0; i < g_numFunctionsPtr; i++)
	{
		// 関数名またはカテゴリ名に部分一致する場合
		if (strstr(g_FunctionsPtr[i].name, searchString) || strstr(g_FunctionsPtr[i].subdir_name, searchString))
		{
			found = true;
			printf(" - %s (Category: %s)\n", g_FunctionsPtr[i].name, g_FunctionsPtr[i].subdir_name);

			funcPtr = g_FunctionsPtr[i].func;
			printf("//EXECUTE//\n");
			((void (*)())funcPtr)();
			printf("\n//FINISH//");
		}
	}

	if (found == false)
	{
		printf("No matching functions found for '%s'.\n", searchString);
	}
}