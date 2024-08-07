#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

/**
 * sigaction 構造体: シグナル処理の設定を行うために使用し、どの関数がシグナル発生時に呼び出されるか、シグナルハンドラの実行中にどのシグナルをブロックするか、シグナル処理のオプションを設定するためのフラグを管理します。
 * itimerval 構造体: タイマーの初回発火時間と、その後の繰り返し発火間隔を設定します。
 */
	

volatile bool g_flag_execute = true;  // グローバル変数
volatile double elapsed_time = 0;     // 経過時間（秒）

// 割り込みハンドラ関数
void timer_handler(int signum) {
    // 割り込み実行前にグローバル変数を確認
    if (g_flag_execute == false) {
        printf("\tInterrupt ignored\n");
    } else {
        printf("\tInterrupt executed\n");
    }

    // 次の割り込みまでのランダムな時間間隔を設定
    int random_time = rand() % 5 + 1; // 1秒から5秒の間のランダムな時間
    struct itimerval timer;
    timer.it_value.tv_sec = random_time;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;

    // タイマーを再設定
    if (setitimer(ITIMER_REAL, &timer, NULL) == -1) {
        perror("setitimer");
        exit(1);
    }
}

int main() {
    srand(time(NULL));  // 乱数の初期化

    struct sigaction sa;
    struct itimerval timer;

    // SIGALRMのシグナルハンドラを設定
    sa.sa_handler = &timer_handler;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGALRM, &sa, NULL);

    // 最初のタイマーをランダムな時間に設定
    int random_time = rand() % 5 + 1; // 1秒から5秒の間のランダムな時間
    timer.it_value.tv_sec = random_time;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;

    // タイマーを開始
    if (setitimer(ITIMER_REAL, &timer, NULL) == -1) {
        perror("setitimer");
        exit(1);
    }

    // メインループ
    while (1) {
        usleep(1000000);  // 0.5秒スリープ
        printf("Elapsed time: %.1f seconds\n", elapsed_time);
        elapsed_time += 1.0;

        if (elapsed_time >= 10 && elapsed_time < 20) {
            g_flag_execute = false; // 割り込みを無効化
        } else {
            g_flag_execute = true;  // 割り込みを有効化
        }
    }

    return 0;
}