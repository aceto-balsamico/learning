// Automatically generated header file

void sort_and_shuffle();
void machine_sim_2();
void machine_sim3();
void Parallel_Operation();
void setjmp_longjmp();
void stack_function();
void B_tree();
void MemoryPool_benchmark();
void union_example();
int change_func_purpose();
void conways_life_game();
int custom_scanf();
void Test_System_hello();
void sort_and_shuffle();
void GetMultiBlocklist();
void mandelbrot();
int WaferMapping();
void FunctionPtr();

typedef struct
{
    void* func;
    const char *func_name;
    const char *subdir_name;
} s_FunctionPtr;

s_FunctionPtr g_FunctionsPtr[] =
{
    {(void*)sort_and_shuffle, "sort_and_shuffle", "TestFunction"},
    {(void*)machine_sim_2, "machine_sim_2", "TestFunction"},
    {(void*)machine_sim3, "machine_sim3", "TestFunction"},
    {(void*)Parallel_Operation, "Parallel_Operation", "TestFunction"},
    {(void*)setjmp_longjmp, "setjmp_longjmp", "TestFunction"},
    {(void*)stack_function, "stack_function", "TestFunction"},
    {(void*)B_tree, "B_tree", "TestFunction"},
    {(void*)MemoryPool_benchmark, "MemoryPool_benchmark", "TestFunction"},
    {(void*)union_example, "union_example", "TestFunction"},
    {(void*)change_func_purpose, "change_func_purpose", "oasobi"},
    {(void*)conways_life_game, "conways_life_game", "oasobi"},
    {(void*)custom_scanf, "custom_scanf", "oasobi"},
    {(void*)Test_System_hello, "Test_System_hello", "oasobi"},
    {(void*)sort_and_shuffle, "sort_and_shuffle", "oasobi"},
    {(void*)GetMultiBlocklist, "GetMultiBlocklist", "oasobi"},
    {(void*)mandelbrot, "mandelbrot", "oasobi"},
    {(void*)WaferMapping, "WaferMapping", "oasobi"},
    {(void*)FunctionPtr, "FunctionPtr", "oasobi"},
};

int g_numFunctionsPtr = sizeof(g_FunctionsPtr) / sizeof(g_FunctionsPtr[0]);
