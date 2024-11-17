#include "common_func.h"

void functionA(int x) {
    printf("Executing Function A: x=%d\n", x);
}

void functionB(int x) {
    printf("Executing Function B: x=%d\n", x);
}
// 可変長引数の関数1
void function1(const char *format, ...) {
    va_list args;
    va_start(args, format);
    printf("[Function1] ");
    vprintf(format, args); // 可変長引数を処理
    va_end(args);
}

// 可変長引数の関数2
void function2(const char *format, ...) {
    va_list args;
    va_start(args, format);
    printf("[Function2] ");
    vprintf(format, args); // 可変長引数を処理
    va_end(args);
}

typedef void (*function_t)(int);
typedef void (*variadic_function_t)(const char *format, ...);

// void (*selected_function)(int) = NULL;
// void (*selected_function_format)(const char *format, ...);

function_t selected_function = NULL;
variadic_function_t selected_function_format = NULL;



void init_function(int condition) {
    selected_function = (condition) ? functionA : functionB;
}

void switch_function(int condition) {
    init_function(condition); // 関数を切り替え
}

void select_function(int choice) {
    if (choice == 1) {
        selected_function_format = function1;
    } else {
        selected_function_format = function2;
    }
}

//@@@function
int change_func_purpose() {
    init_function(1); // 初期設定
    for (int i = 0; i < 3; i++) {
        selected_function(i);
    }

    printf("Switching to Function B\n");
    switch_function(0); // 関数ポインタを動的に変更

    for (int i = 0; i < 3; i++) {
        selected_function(i);
    }

	select_function(1);
    selected_function_format("Hello, %s!\n", "World");

    // 切り替え
    select_function(2);
    selected_function_format("The value is %d\n", 42);


    return 0;
}