#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

// ラッパー構造体
typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    bool is_paused; // スレッドの停止状態を管理
} ThreadControl;

// ThreadControlの初期化
void thread_control_init(ThreadControl *ctrl) {
    pthread_mutex_init(&ctrl->mutex, NULL);
    pthread_cond_init(&ctrl->cond, NULL);
    ctrl->is_paused = false;
}

// ThreadControlの破棄
void thread_control_destroy(ThreadControl *ctrl) {
    pthread_mutex_destroy(&ctrl->mutex);
    pthread_cond_destroy(&ctrl->cond);
}

// スレッドを停止状態に設定
void thread_pause(ThreadControl *ctrl) {
    pthread_mutex_lock(&ctrl->mutex);
    ctrl->is_paused = true;
    pthread_mutex_unlock(&ctrl->mutex);
}

// スレッドを再開状態に設定
void thread_resume(ThreadControl *ctrl) {
    pthread_mutex_lock(&ctrl->mutex);
    ctrl->is_paused = false;
    pthread_cond_signal(&ctrl->cond);
    pthread_mutex_unlock(&ctrl->mutex);
}

// スレッドが再開されるまで待機
void thread_wait(ThreadControl *ctrl) {
    pthread_mutex_lock(&ctrl->mutex);
    while (ctrl->is_paused) {
        pthread_cond_wait(&ctrl->cond, &ctrl->mutex);
    }
    pthread_mutex_unlock(&ctrl->mutex);
}

// Machine構造体
typedef struct {
    char name[20];
    ThreadControl ctrl; // 制御用構造体
} Machine;

// Machine操作関数
void *operation(void *arg) {
    Machine *machine = (Machine *)arg;

	struct timespec timespec = {0, 50000000};
	int i = 0;
    // for (int i = 0; i < 10; i++) 
	while(1)
	{
        thread_wait(&machine->ctrl); // 停止中なら再開を待機
        printf("%s: Performing operation %d\n", machine->name, i + 1);
        // sleep(1); // 処理の模擬
		nanosleep(&timespec, NULL);
		i++;

		if(i > 20)
		{
			break;
		}
    }

    return NULL;
}

//@@@function
void wrap_pthread() {
    pthread_t threadA, threadB;
    Machine machineA = { "MachineA" }, machineB = { "MachineB" };

    // 制御構造体の初期化
    thread_control_init(&machineA.ctrl);
    thread_control_init(&machineB.ctrl);

    // スレッドを作成
    pthread_create(&threadA, NULL, operation, &machineA);
    pthread_create(&threadB, NULL, operation, &machineB);

	thread_pause(&machineA.ctrl);
    thread_pause(&machineB.ctrl);

    // MachineAを再開
    thread_resume(&machineA.ctrl);
    sleep(2); // 少し実行させる

    // MachineAを停止し、MachineBを再開
    thread_pause(&machineA.ctrl);
    thread_resume(&machineB.ctrl);
    sleep(2);

    // 再度MachineAを再開
    thread_pause(&machineB.ctrl);
    thread_resume(&machineA.ctrl);
    sleep(2);

    // 最後に両方停止
    thread_pause(&machineA.ctrl);
    thread_pause(&machineB.ctrl);

    // スレッドの終了待ち
    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);

    // 制御構造体の破棄
    thread_control_destroy(&machineA.ctrl);
    thread_control_destroy(&machineB.ctrl);

    return;
}