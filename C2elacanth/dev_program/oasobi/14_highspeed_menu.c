#include "custom_common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MENU_CAPACITY 64
#define EXPLANATION_LENGTH 32

typedef struct
{
	int keys[MENU_CAPACITY];
	int rates[MENU_CAPACITY];
	char explanations[MENU_CAPACITY][EXPLANATION_LENGTH];
	int selected[MENU_CAPACITY]; // 選択済みフラグ
	int count;
	int total_rate;
	int remaining; // 残りの選択可能数
} Menu;

// 初期化
void initMenu(Menu *menu)
{
	menu->count = 0;
	menu->total_rate = 0;
	menu->remaining = 0;
	memset(menu->selected, 0, sizeof(menu->selected));
}

// 要素追加
void append(Menu *menu, int key, int rate, const char *explanation)
{
	if (menu->count >= MENU_CAPACITY)
		return;
	menu->keys[menu->count] = key;
	menu->rates[menu->count] = rate;
	strncpy(menu->explanations[menu->count], explanation, EXPLANATION_LENGTH - 1);
	menu->explanations[menu->count][EXPLANATION_LENGTH - 1] = '\0';
	menu->total_rate += rate;
	menu->count++;
	menu->remaining++;
}

// 1つ選択（重複なし）
int choice(Menu *menu)
{
	if (menu->remaining == 0)
		return -1; // すべて選ばれたら終了

	int rnd = rand() % menu->total_rate;
	int sum = 0;

	for (int i = 0; i < menu->count; i++)
	{
		if (menu->selected[i])
			continue; // 既に選択済みならスキップ
		sum += menu->rates[i];
		if (rnd < sum)
		{
			menu->selected[i] = 1;				// 選択済みフラグを立てる
			menu->total_rate -= menu->rates[i]; // 残りの合計rateを減らす
			menu->remaining--;					// 選択可能数を減らす
			return menu->keys[i];
		}
	}
	return -1;
}
//@@@function
void rate_menu()
{
	srand(time(NULL));

	Menu menu;
	initMenu(&menu);

	append(&menu, 1, 10, "Option 1");
	append(&menu, 2, 30, "Option 2");
	append(&menu, 3, 20, "Option 3");
	append(&menu, 4, 40, "Option 4");

	// choiceを連続で呼ぶ（重複なし）
	for (int i = 0; i < 4; i++)
	{
		int result = choice(&menu);
		if (result == -1)
		{
			printf("No more choices available.\n");
			break;
		}
		printf("Selected key: %d\n", result);
	}

	// 再利用
	printf("Resetting menu...\n");
	initMenu(&menu);

	append(&menu, 5, 50, "Option 5");
	append(&menu, 6, 50, "Option 6");

	printf("Selected key after reset: %d\n", choice(&menu));
	printf("Selected key after reset: %d\n", choice(&menu));
	printf("Selected key after reset: %d\n", choice(&menu));

	return;
}