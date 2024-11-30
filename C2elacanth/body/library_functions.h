// Automatically generated header file

void sort_and_shuffle();
void machine_sim_2();
void machine_sim3();
void Test_System_hello();
int WaferMapping();
int change_func_purpose();
void conways_life_game();
int custom_scanf();
void sort_and_shuffle();
void GetMultiBlocklist();
void mandelbrot();

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
    {(void*)Test_System_hello, "Test_System_hello", "oasobi"},
    {(void*)WaferMapping, "WaferMapping", "oasobi"},
    {(void*)change_func_purpose, "change_func_purpose", "oasobi"},
    {(void*)conways_life_game, "conways_life_game", "oasobi"},
    {(void*)custom_scanf, "custom_scanf", "oasobi"},
    {(void*)sort_and_shuffle, "sort_and_shuffle", "oasobi"},
    {(void*)GetMultiBlocklist, "GetMultiBlocklist", "oasobi"},
    {(void*)mandelbrot, "mandelbrot", "oasobi"},
};

int g_numFunctionsPtr = sizeof(g_FunctionsPtr) / sizeof(g_FunctionsPtr[0]);
