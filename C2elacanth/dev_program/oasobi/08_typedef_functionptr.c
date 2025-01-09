#include "custom_common.h"

typedef void (*FuncVoidNoArgs)(void);
typedef int (*FuncIntNoArgs)(void);
typedef void (*FuncVoidWithArgs)(int, float);
typedef double (*FuncDoubleWithArgs)(int, double);
typedef void (*FuncVoidWithPtrArg)(int*);

// 使用例
void exampleFunction(void) {
    printf("Hello, World!\n");
}
int getValue(void) {
    return 42;
}
void printValues(int a, float b) {
    printf("a = %d, b = %f\n", a, b);
}
double multiply(int a, double b) {
    return a * b;
}
void increment(int* value) {
    (*value)++;
}

//@@@function
void FunctionPtr()
{
	FuncVoidNoArgs func_v_n = exampleFunction;
	FuncIntNoArgs func_i_n = getValue;
	FuncVoidWithArgs func_v_if = printValues;
	FuncDoubleWithArgs func_d_id = multiply;
	FuncVoidWithPtrArg func_v_p = increment;

	func_v_n(); // "Hello, World!" と出力
	printf("%d\n", func_i_n()); // 42 と出力
	func_v_if(5, 3.14); // "a = 5, b = 3.140000" と出力
	printf("%f\n", func_d_id(3, 4.5)); // 13.500000 と出力
	int x = 10;
	func_v_p(&x);
	printf("%d\n", x); // 11 と出力
}

