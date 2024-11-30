#include "custom_common.h"

// #include <stdio.h>

#define WIDTH 80	  // 横幅
#define HEIGHT 40	  // 縦幅
#define MAX_ITER 1000 // 最大反復回数

// マンデルブロ集合の描画

void draw_mandelbrot()
{
	// 描画領域の範囲
	double x_min = -2.0, x_max = 1.0;
	double y_min = -1.0, y_max = 1.0;

	// 1ドットあたりのスケール
	double x_scale = (x_max - x_min) / WIDTH;
	double y_scale = (y_max - y_min) / HEIGHT;

	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			double c_re = x_min + x * x_scale; // 実部
			double c_im = y_max - y * y_scale; // 虚部
			double z_re = 0, z_im = 0;		   // zの初期値
			int iter = 0;

			// マンデルブロ集合の反復計算
			while (z_re * z_re + z_im * z_im <= 4.0 && iter < MAX_ITER)
			{
				double z_re_tmp = z_re * z_re - z_im * z_im + c_re;
				z_im = 2.0 * z_re * z_im + c_im;
				z_re = z_re_tmp;
				iter++;
			}

			// 描画文字の選択
			char c = (iter == MAX_ITER) ? '#' : ' ';
			putchar(c);
		}
		putchar('\n');
	}
}
//@@@function
void mandelbrot()
{
	draw_mandelbrot();
}