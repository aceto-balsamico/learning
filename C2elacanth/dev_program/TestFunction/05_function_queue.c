#include "custom_common.h"


#define MAX_STACK_SIZE 128
#define NUM_STACKS 2

typedef void (*TaskFunction)(void *); // 関数ポインタ型定義

// タスク情報を保存する構造体
typedef struct {
    TaskFunction func;  // 実行する関数
    void *arg;          // 関数の引数
    bool is_paused;     // タスクが一時停止状態か
} Task;

// スタック構造体
typedef struct {
    Task tasks[MAX_STACK_SIZE]; // タスク用スタック
    int top;                    // スタックの先頭位置
} TaskStack;

// スタックを初期化
void init_stack(TaskStack *stack) {
    stack->top = -1;
}

// タスクをスタックにプッシュ
bool push_task(TaskStack *stack, TaskFunction func, void *arg) {
    if (stack->top >= MAX_STACK_SIZE - 1) {
        printf("Stack overflow!\n");
        return false;
    }
    stack->top++;
    stack->tasks[stack->top].func = func;
    stack->tasks[stack->top].arg = arg;
    stack->tasks[stack->top].is_paused = false;
    return true;
}

// タスクをポップ
bool pop_task(TaskStack *stack, Task *task) {
    if (stack->top < 0) {
        return false; // スタックが空
    }
    *task = stack->tasks[stack->top];
    stack->top--;
    return true;
}

// スタックのタスクを実行
void execute_stack(TaskStack *stack) {
    Task current_task;
    while (pop_task(stack, &current_task)) {
        if (!current_task.is_paused) {
            current_task.func(current_task.arg);
        }
    }
}

// グローバルでスタック管理（複数スタック）
TaskStack stacks[NUM_STACKS];

// ---- サンプル関数 ---- //
void task1(void *arg) {
    printf("Task 1: Step %d\n", *(int *)arg);
    *(int *)arg += 1;
    if (*(int *)arg < 3) { // 3ステップ実行したら終了
        push_task(&stacks[0], task1, arg); // Task1を再度Pushして継続
    } else {
        printf("Task 1: Completed\n");
    }
}

void task2(void *arg) {
    printf("Task 2: Step %d\n", *(int *)arg);
    *(int *)arg += 1;
    if (*(int *)arg < 2) {
        push_task(&stacks[1], task2, arg); // Task2を別のスタックにPush
    } else {
        printf("Task 2: Completed\n");
    }
}

//@@@function
void stack_function() {
    // 複数のスタックを初期化
    for (int i = 0; i < NUM_STACKS; i++) {
        init_stack(&stacks[i]);
    }

    int task1_counter = 0, task2_counter = 0;

    // タスクをスタックにプッシュ
    push_task(&stacks[0], task1, &task1_counter);
    push_task(&stacks[1], task2, &task2_counter);

    // スタック0とスタック1を交互に実行
    while (stacks[0].top >= 0 || stacks[1].top >= 0) {
        printf("\n--- Executing Stack 0 ---\n");
        execute_stack(&stacks[0]);
        printf("\n--- Executing Stack 1 ---\n");
        execute_stack(&stacks[1]);
    }

    printf("\nAll tasks completed!\n");
}