#include "custom_common.h"
#include "library_functions.h"


void DisplayMenu(const char *searchString);
void analysis_menu()
{
	// DisplayMenu("No_6");
	// DisplayMenu("No_8");
	DisplayMenu("Program");
}

//関数名もしくはサブディレクトリ名の部分一致で検索し、ヒットした関数を一覧表示する
void DisplayMenu(const char *searchString) 
{
	printf("=== Functions matching '%s' ===\n", searchString);

	bool found = false;
	for (int i = 0; i < g_numFunctionsPtr; i++) 
	{
		char c = 'u';

		if (strstr(g_FunctionsPtr[i].name, searchString) || strstr(g_FunctionsPtr[i].subdir_name, searchString)) 
		{
			found = true;
			printf(" - %s (Category: %s)\n", g_FunctionsPtr[i].name, g_FunctionsPtr[i].subdir_name);
		}
	}

	if (found == false) 
	{
		printf("No matching functions found for '%s'.\n", searchString);
	}

}