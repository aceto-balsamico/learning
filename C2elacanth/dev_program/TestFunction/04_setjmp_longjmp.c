#include "custom_common.h"

#include <setjmp.h>

jmp_buf buf;

void error_handler() 
{
    printf("エラーが発生しました！\n");
    longjmp(buf, 1);  // setjmpにジャンプ
}

//@@@function
void setjmp_longjmp() 
{
    if (setjmp(buf) == 0) 
	{
        // setjmpが初めて呼び出された場合
        printf("通常の処理を開始します。\n");
        
        for (int i = 0; i < 5; i++) 
		{
            printf("処理中: %d\n", i);
            if (i == 2) 
			{  
				// エラー発生をシミュレート
                error_handler();  
            }
        }
        
        printf("ループが正常に終了しました。\n");
    }
	else 
	{
        // longjmpが呼び出された場合
        printf("例外処理を実行中...\n");
    }

    printf("プログラムを終了します。\n");
}