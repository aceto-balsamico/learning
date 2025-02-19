#include "custom_common.h"

#define MENU_CAPACITY 64
#define EXPLANATION_LENGTH 32


typedef struct
{
	int keys[MENU_CAPACITY];
	int rates[MENU_CAPACITY];
	char explanations[MENU_CAPACITY][EXPLANATION_LENGTH];
	int cumsum[MENU_CAPACITY];
	int count;
	int total_rate;
	int remaining;
} F_Menu;

// 初期化
void F_initMenu(F_Menu *menu)
{
	menu->count = 0;
	menu->total_rate = 0;
	menu->remaining = 0;
}

// 要素追加
void F_append(F_Menu *menu, int key, int rate, const char *explanation)
{
	if (menu->count >= MENU_CAPACITY)
		return;
	menu->keys[menu->count] = key;
	menu->rates[menu->count] = rate;
	strncpy(menu->explanations[menu->count], explanation, EXPLANATION_LENGTH - 1);
	menu->explanations[menu->count][EXPLANATION_LENGTH - 1] = '\0';

	menu->total_rate += rate;
	menu->cumsum[menu->count] = (menu->count == 0) ? rate : menu->cumsum[menu->count - 1] + rate;

	menu->count++;
	menu->remaining++;
}

// Fisher-Yates シャッフルを用いた選択
int F_choice(F_Menu *menu)
{
	if (menu->remaining == 0)
		return -1;

	int rnd = rand() % menu->remaining;
	int idx = rnd;

	// 選択した要素をリストの最後とスワップして選択を確定
	int selected_key = menu->keys[idx];
	menu->keys[idx] = menu->keys[menu->remaining - 1];
	menu->keys[menu->remaining - 1] = selected_key;

	menu->remaining--;
	return selected_key;
}

//@@@function
void Shuffle_Rate_Menu()
{
	srand(time(NULL));

	F_Menu menu;
	F_initMenu(&menu);

	// F_append(&menu, 1, 10, "Option 1");
	// F_append(&menu, 2, 30, "Option 2");
	// F_append(&menu, 3, 20, "Option 3");
	// F_append(&menu, 4, 40, "Option 4");

	for(int i = 0; i < 64; i++)
	{
		F_append(&menu, i + 5, i + 1, "Option 5");
	}

	// choiceを連続で呼ぶ（重複なし）
	for (int i = 0; i < 64; i++)
	{
		int result = F_choice(&menu);
		if (result == -1)
		{
			printf("No more choices available.\n");
			break;
		}
		printf("Selected key: %d\n", result);
	}

	// printf("Resetting menu...\n");
	// F_initMenu(&menu);

	// F_append(&menu, 5, 50, "Option 5");
	// F_append(&menu, 6, 50, "Option 6");

	// printf("Selected key after reset: %d\n", F_choice(&menu));
	// printf("Selected key after reset: %d\n", F_choice(&menu));
	// printf("Selected key after reset: %d\n", F_choice(&menu));
}
