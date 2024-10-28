// Automatically generated header file

#include <stdio.h>

void common_B8func();
void No8_Program();
void No8_Erase();
void No6_Erase();
void No6_Program();

typedef struct
{
    const char *name;
    void* func;
    const char *subdir_name;
} s_FunctionPtr;

s_FunctionPtr g_FunctionsPtr[] =
{
    {"common_B8func", (void*)common_B8func, "No8"},
    {"No8_Program", (void*)No8_Program, "No8"},
    {"No8_Erase", (void*)No8_Erase, "No8"},
    {"No6_Erase", (void*)No6_Erase, "No6"},
    {"No6_Program", (void*)No6_Program, "No6"},
};

int g_numFunctionsPtr = sizeof(g_FunctionsPtr) / sizeof(g_FunctionsPtr[0]);
