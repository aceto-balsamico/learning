#include "custom_common.h"

#define INPUT_BUFFER_SIZE 256

//改行かEOFまで読み取って捨てる
void clear_stdin() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        // 標準入力の残りを消去
    }
}

int fscanf_safe(const char *format, ...) {
    FILE* stream = stdin;
	
	char buffer[INPUT_BUFFER_SIZE];
    
    // 入力を取得
    if (fgets(buffer, sizeof(buffer), stream) == NULL) {
        return EOF; // 入力エラーまたはEOF
    }
	else
	{
		if (buffer[strlen(buffer) - 1] != '\n') {
			// 入力がオーバーフローして改行が入らなかった場合
			printf("Input is too long. Flush stdin! ->Buffer limit:%d\n", INPUT_BUFFER_SIZE);
			clear_stdin();
		
		}
	}

    // fgetsで読み込んだ文字列の改行を削除
    buffer[strcspn(buffer, "\n")] = '\0';

    // 可変引数リストを処理
    va_list args;
    va_start(args, format);
    int result = vsscanf(buffer, format, args);
    va_end(args);

    return result;
}
//@@@function
int custom_scanf() {
    int age;
    double height;
    char name[50];

    printf("Enter your name: ");
    if (fscanf_safe("%49s", name) == 1) {  // 名前を入力
        printf("Hello, %s!\n", name);
    } else {
        fprintf(stderr, "Invalid input for name.\n");
        return 1;
    }

    printf("Enter your age: ");
    if (fscanf_safe("%d", &age) == 1) {  // 年齢を入力
        printf("Your age: %d\n", age);
    } else {
        fprintf(stderr, "Invalid input for age.\n");
        return 1;
    }

    printf("Enter your height (in cm): ");
    if (fscanf_safe("%lf", &height) == 1) {  // 身長を入力
        printf("Your height: %.2f cm\n", height);
    } else {
        fprintf(stderr, "Invalid input for height.\n");
        return 1;
    }

    return 0;
}