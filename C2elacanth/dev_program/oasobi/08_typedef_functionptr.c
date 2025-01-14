#include "custom_common.h"

typedef void (*FuncVoidNoArgs)(void);
typedef int (*FuncIntNoArgs)(void);
typedef void (*FuncVoidWithArgs)(int, float);
typedef double (*FuncDoubleWithArgs)(int, double);
typedef void (*FuncVoidWithPtrArg)(int *);
typedef int (*FuncWithVarArgs)(const char*, ...);

// 使用例
void exampleFunction(void)
{
	printf("Hello, World!\n");
}
int getValue(void)
{
	return 42;
}
void printValues(int a, float b)
{
	printf("a = %d, b = %f\n", a, b);
}
double multiply(int a, double b)
{
	return a * b;
}

int customPrintf(const char* format, ...) 
{
    va_list args;
    va_start(args, format);
    int result = vprintf(format, args);
    va_end(args);
    return result;
}

void increment(int *value)
{
	(*value)++;
}

//@@@function
void FunctionPtr()
{
	FuncVoidNoArgs func_v_n = exampleFunction;
	FuncIntNoArgs func_i_n = getValue;
	FuncVoidWithArgs func_v_if = printValues;
	FuncDoubleWithArgs func_d_id = multiply;
	FuncWithVarArgs func_i_cp__ = customPrintf;
	FuncVoidWithPtrArg func_v_cp = increment;

	func_v_n();						   // "Hello, World!" と出力
	printf("%d\n", func_i_n());		   // 42 と出力
	func_v_if(5, 3.14);				   // "a = 5, b = 3.140000" と出力
	printf("%f\n", func_d_id(3, 4.5)); // 13.500000 と出力
	func_i_cp__("Value: %d, %s\n", 42, "example"); // "Value: 42, example" と出力
	int x = 10;
	func_v_cp(&x);
	printf("%d\n", x); // 11 と出力
}
