#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

/*
 * ROWS x COLS のグリッドを用意する。
 * 初期状態をランダムに設定する。
 * 各世代で以下のルールに基づいてセルの状態を更新する。
 * 生存: 生きているセルが隣接する生きたセルを2つまたは3つ持つ場合、生存する。
 * 過疎: 生きているセルが隣接する生きたセルを1つ以下しか持たない場合、死滅する。
 * 過密: 生きているセルが隣接する生きたセルを4つ以上持つ場合、死滅する。
 * 誕生: 死んでいるセルが隣接する生きたセルをちょうど3つ持つ場合、誕生する。
 */

#define ROWS 15
#define COLS 60
int generate_random_range(int min, int max) {
    if (min > max) {
        int temp = min;
        min = max;
        max = temp;
    }

    return rand() % (max - min + 1) + min;
}
// セルの状態を更新する
void update_grid(int grid[ROWS][COLS], int new_grid[ROWS][COLS]) {
	//8方向を表すベクトル
	int dx[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
	int dy[] = { -1, 0, 1, -1, 1, -1, 0, 1 };


	for (int row = 0; row < ROWS; row++) {
		for (int col = 0; col < COLS; col++) {
			int live_neighbors = 0;

			for (int k = 0; k < 8; k++) {
				int new_row = row + dx[k];
				int new_col = col + dy[k];
				if (new_row >= 0 && new_row < ROWS && new_col >= 0 && new_col < COLS) {
					live_neighbors += grid[new_row][new_col];
				}
			}

			if (grid[row][col] == 1) {
				if (live_neighbors < 2 || live_neighbors > 3) {
					new_grid[row][col] = 0;
				}
				else {
					new_grid[row][col] = 1;
				}
			}
			else {
				if (live_neighbors == 3) {
					new_grid[row][col] = 1;
				}
				else {
					new_grid[row][col] = 0;
				}
			}
		}
	}
}

// グリッドの表示
void print_grid(int grid[ROWS][COLS]) {
	for (int row = 0; row < ROWS; row++) {
		for (int col = 0; col < COLS; col++) {
			if (grid[row][col] == 1) {
				printf("O");
			}
			else {
				printf(".");
			}
		}
		printf("\n");
	}
}
bool check_grid(int grid[ROWS][COLS], int new_grid[ROWS][COLS])
{
	int equal = 0;
	for(int row = 0; row < ROWS; row++)
	{
		for(int col = 0; col < COLS; col++)
		{
			if(grid[row][col] == new_grid[row][col])equal++;
		}
	}
	if(equal == ROWS*COLS)	return true;
	else					return false;
}
// グリッドをランダムに初期化
void initialize_grid(int grid[ROWS][COLS]) {
	for (int row = 0; row < ROWS; row++) {
		for (int col = 0; col < COLS; col++) {
			grid[row][col] = rand() % 2;
		}
	}
}
int has_time_elapsed(time_t start_time, int duration_seconds) {
    time_t current_time;
    time(&current_time);
    return difftime(current_time, start_time) >= duration_seconds;
}
int main() {
	int grid[ROWS][COLS];
	int new_grid[ROWS][COLS];
	

	srand(time(NULL));
	initialize_grid(grid);

	int duration_seconds = 60; // 無限ループを抜けるまでの秒数
    time_t start_time;
    time(&start_time); // 開始時間を記録

	while (!has_time_elapsed(start_time, duration_seconds)) {
		printf("\033[H\033[J"); // コンソールをクリア
		print_grid(grid);
		update_grid(grid, new_grid);
		if(check_grid(grid, new_grid) == true)break;
		
		

		// 新しいグリッドの内容を古いグリッドにコピー
		for (int row = 0; row < ROWS; row++) {
			for (int col = 0; col < COLS; col++) {
				grid[row][col] = new_grid[row][col];
			}
		}

		usleep(200000); // 0.2秒待機
	}

	return 0;
}
