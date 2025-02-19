#include "custom_common.h"

#define MENU_CAPACITY 64
#define EXPLANATION_LENGTH 32

typedef struct
{
	int keys[MENU_CAPACITY];
	int rates[MENU_CAPACITY];
	char explanations[MENU_CAPACITY][EXPLANATION_LENGTH];
	int selected[MENU_CAPACITY]; // 選択済みフラグ
	int cumsum[MENU_CAPACITY];	 // 累積和
	int count;
	int total_rate;
	int remaining; // 選択可能数
} I_Menu;

// 二分探索でランダム選択
int binarySearch(I_Menu *menu, int value)
{
	int left = 0, right = menu->count - 1;
	while (left < right)
	{
		int mid = left + (right - left) / 2;
		if (menu->cumsum[mid] > value)
		{
			right = mid;
		}
		else
		{
			left = mid + 1;
		}
	}
	return left;
}

// 初期化
void I_initMenu(I_Menu *menu)
{
	menu->count = 0;
	menu->total_rate = 0;
	menu->remaining = 0;
	memset(menu->selected, 0, sizeof(menu->selected));
}

// 要素追加
void I_append(I_Menu *menu, int key, int rate, const char *explanation)
{
	if (menu->count >= MENU_CAPACITY)
		return;
	menu->keys[menu->count] = key;
	menu->rates[menu->count] = rate;
	strncpy(menu->explanations[menu->count], explanation, EXPLANATION_LENGTH - 1);
	menu->explanations[menu->count][EXPLANATION_LENGTH - 1] = '\0';

	// 累積和を計算
	menu->total_rate += rate;
	menu->cumsum[menu->count] = (menu->count == 0) ? rate : menu->cumsum[menu->count - 1] + rate;

	menu->count++;
	menu->remaining++;
}

// 1つ選択（重複なし）
int I_choice(I_Menu *menu)
{
	if (menu->remaining == 0)
		return -1; // すべて選ばれたら終了

	int rnd, idx;

	// 選択済みでないものが出るまで繰り返す
	do
	{
		rnd = rand() % menu->total_rate;
		idx = binarySearch(menu, rnd);
	} while (menu->selected[idx]);

	// 選択済みにする
	menu->selected[idx] = 1;
	menu->remaining--;

	return menu->keys[idx];
}

//@@@function
void improve_rate_menu()
{
	srand(time(NULL));

	I_Menu menu;
	I_initMenu(&menu);

	// I_append(&menu, 1, 10, "Option 1");
	// I_append(&menu, 2, 30, "Option 2");
	// I_append(&menu, 3, 20, "Option 3");
	// I_append(&menu, 4, 40, "Option 4");

	for(int i = 0; i < 64; i++)
	{
		I_append(&menu, i + 5, i + 1, "Option 5");
	}

	// choiceを連続で呼ぶ（重複なし）
	for (int i = 0; i < 64; i++)
	{
		int result = I_choice(&menu);
		if (result == -1)
		{
			printf("No more choices available.\n");
			break;
		}
		printf("Selected key: %d\n", result);
	}

	// // 再利用
	// printf("Resetting menu...\n");
	// I_initMenu(&menu);

	// I_append(&menu, 5, 50, "Option 5");
	// I_append(&menu, 6, 50, "Option 6");

	// printf("Selected key after reset: %d\n", I_choice(&menu));
	// printf("Selected key after reset: %d\n", I_choice(&menu));
	// printf("Selected key after reset: %d\n", I_choice(&menu));

	return;
}


