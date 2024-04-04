// Automatically generated header file

#include <stdio.h>

void hello_world();
int Test_Test();
int* addressptr();
void simple();

typedef struct {
    const char *name;
    void* func;
} s_FunctionPtr;

s_FunctionPtr g_FunctionsPtr[] = {
    {"hello_world", (void*)hello_world},
    {"Test_Test", (void*)Test_Test},
    {"addressptr", (void*)addressptr},
    {"simple", (void*)simple},
};

int g_numFunctionsPtr = sizeof(g_FunctionsPtr) / sizeof(g_FunctionsPtr[0]);
