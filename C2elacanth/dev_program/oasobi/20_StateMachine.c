#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LOOP_COUNT 10000000
#define STATE_COUNT 4

int sum = 0;

double get_time_sec(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

// -------------------- 1. switch-case --------------------
void run_switch_case(int *states) {
    sum = 0;
    for (int i = 0; i < LOOP_COUNT; i++) {
        switch (states[i]) {
            case 0: sum += 1; break;
            case 1: sum += 2; break;
            case 2: sum += 3; break;
            case 3: sum += 4; break;
        }
    }
}

// -------------------- 2. function pointer --------------------
void f0(void) { sum += 1; }
void f1(void) { sum += 2; }
void f2(void) { sum += 3; }
void f3(void) { sum += 4; }

void run_func_pointer(int *states) {
    sum = 0;
    void (*funcs[])(void) = { f0, f1, f2, f3 };
    for (int i = 0; i < LOOP_COUNT; i++) {
        funcs[states[i]]();
    }
}

// -------------------- 3. goto table (GNU拡張) --------------------
void run_goto_table(int *states) {
    sum = 0;
    static void *labels[] = { &&state0, &&state1, &&state2, &&state3 };

    for (int i = 0; i < LOOP_COUNT; i++) {
        goto *labels[states[i]];
state0: sum += 1; continue;
state1: sum += 2; continue;
state2: sum += 3; continue;
state3: sum += 4; continue;
    }
}

// -------------------- 4. LUT table --------------------
void run_lut_table(int *states) {
    sum = 0;
    int lut[] = { 1, 2, 3, 4 };
    for (int i = 0; i < LOOP_COUNT; i++) {
        sum += lut[states[i]];
    }
}

// -------------------- Main --------------------
//@@@function
int speed_test_StateMachine(void) 
{
    int *states = malloc(sizeof(int) * LOOP_COUNT);
    if (!states) return 1;

    // 状態をランダムに初期化
    for (int i = 0; i < LOOP_COUNT; i++) {
        states[i] = rand() % STATE_COUNT;
    }

    double t1, t2;

    t1 = get_time_sec();
    run_switch_case(states);
    t2 = get_time_sec();
    printf("switch-case   : %.6f sec (sum=%d)\n", t2 - t1, sum);

    t1 = get_time_sec();
    run_func_pointer(states);
    t2 = get_time_sec();
    printf("func-pointer  : %.6f sec (sum=%d)\n", t2 - t1, sum);

    t1 = get_time_sec();
    run_goto_table(states);
    t2 = get_time_sec();
    printf("goto-table    : %.6f sec (sum=%d)\n", t2 - t1, sum);

    t1 = get_time_sec();
    run_lut_table(states);
    t2 = get_time_sec();
    printf("lut-table     : %.6f sec (sum=%d)\n", t2 - t1, sum);

    free(states);
    return 0;
}