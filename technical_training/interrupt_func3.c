#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
bool interrupt_enabled = true; // 割り込み処理を許可するかどうかを制御するグローバル変数
bool flag_main_finish;

// 疑似ランダム割り込みが発生したときに呼び出される関数
void interrupt_handler() {
    printf("Interrupt occurred, executing handler function.\n");
    // 複雑な処理をここに記述
}

// 疑似ランダムに割り込みを発生させるスレッド
void* interrupt_thread(void* arg) {
    while (1) {
        int sleep_time = rand() % 5 + 1;
        sleep(sleep_time);

        // 割り込みが許可されているかどうかをチェック
        pthread_mutex_lock(&mutex);
        if (interrupt_enabled) {
            interrupt_handler();
        } else {
            // printf("Interrupt skipped due to global control variable.\n");
        }
        pthread_mutex_unlock(&mutex);

		// メインスレッドが終了しているかどうかをチェック
		if (flag_main_finish) {
			break;
		}
    }
    return NULL;
}


int main() {
    pthread_t thread;

    // 乱数生成の初期化
    srand(time(NULL));

    // 割り込みスレッドを作成
    if (pthread_create(&thread, NULL, interrupt_thread, NULL) != 0) {
        perror("pthread_create");
        return EXIT_FAILURE;
    }

    // メインスレッドの処理
	int counter = 0;
    for (int i = 0; i < 30; i++) 
	// while(1)
	{
        printf("Main thread running...[%02d]\n", counter);
        sleep(1);

        // 一定の条件で割り込みを無効化
        if (counter > 10 && interrupt_enabled == true) {
            pthread_mutex_lock(&mutex);
            interrupt_enabled = false; // 割り込みを無効化
            pthread_mutex_unlock(&mutex);
            // printf("Interrupts disabled.\n");
        }


        // 割り込みを再度有効化
        if (counter > 20 && interrupt_enabled == false) {
            pthread_mutex_lock(&mutex);
            interrupt_enabled = true; // 割り込みを有効化
            pthread_mutex_unlock(&mutex);
            // printf("Interrupts enabled.\n");
        }
		counter++;
    }

    // スレッドの終了を待機
	flag_main_finish = true;
    pthread_join(thread, NULL);

    return EXIT_SUCCESS;
}