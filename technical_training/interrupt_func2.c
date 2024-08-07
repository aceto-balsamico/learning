#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

volatile bool interrupt_occurred = false;  // 割り込みが発生したことを示すフラグ
volatile bool g_flag_execute = true;  // 割り込みを発生させるためのフラグ


// 割り込みハンドラ関数
void timer_handler(int signum) {
// void timer_handler(int signum, siginfo_t *info, void *context) {
    // 割り込みが発生したことをフラグで示す

	// 割り込み実行前にグローバル変数を確認
	if (g_flag_execute == false) {
		/* 割り込み処理は行わない */
	} else {
		printf("\tInterrupt executed\n");
    	interrupt_occurred = true;
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
    // sa.sa_flags = SA_SIGINFO;
    // sa.sa_sigaction = &timer_handler;
    // sigemptyset(&sa.sa_mask);
    // sigaction(SIGALRM, &sa, NULL);
	sa.sa_handler = &timer_handler;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGALRM, &sa, NULL);

    // 初回のタイマーをランダムな時間に設定
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
	double elapsed_time = 0.0;
    while (1) {
        usleep(1000000);  // 1秒スリープ
		printf("Elapsed time: %.1f seconds\n", elapsed_time);
		elapsed_time += 1.0;

		if(elapsed_time == 10.0) g_flag_execute = false;
		if(elapsed_time == 20.0) g_flag_execute = true;

        // 割り込み処理が行われたかどうかを確認
        if (interrupt_occurred) {
            printf("Interrupt occurred\n");
            interrupt_occurred = false;  // フラグをリセット
        }

        // 経過時間の表示など、他の処理を行う
    }

    return 0;
}
/**
 *	setitimer関数
	1.	which:
		タイマーの種類を指定する整数値です。次のいずれかの定数を指定します：
		ITIMER_REAL: システムのリアルタイムに基づくタイマー。タイマーが発火すると SIGALRM シグナルが送信されます。
		ITIMER_VIRTUAL: プロセスの仮想時間に基づくタイマー。プロセスが実行している間にだけカウントされ、タイマーが発火すると SIGVTALRM シグナルが送信されます。
		ITIMER_PROF: プロセスのプロファイル時間に基づくタイマー。プロセスのユーザ時間とシステム時間の合計に基づいてカウントされ、タイマーが発火すると SIGPROF シグナルが送信されます。
	2.	new_value:
		itimerval 構造体へのポインタです。新しいタイマーの設定を指定します。構造体のフィールドは以下の通りです：
		it_value: タイマーの初回発火時間を指定します。struct timeval 型で、秒とマイクロ秒を指定します。
		it_interval: タイマーの繰り返し発火間隔を指定します。struct timeval 型で、秒とマイクロ秒を指定します。it_interval がゼロの場合、タイマーは一度だけ発火します。
	3.	old_value:
		タイマーの以前の設定を保存するための itimerval 構造体へのポインタです。この引数が NULL の場合、以前の設定を取得することはありません。以前のタイマーの設定を取得したい場合は、ここに有効なポインタを渡します。

 *	sigaction関数
	sigaction 関数は、指定したシグナルに対する処理を設定するために使用します。シグナルは、特定のイベントが発生した際にプロセスに通知するための手段です。シグナルハンドラを設定することで、シグナルを受け取ったときに特定の関数が呼び出されるようにできます。
	•	signum: 設定したいシグナル番号。例えば、SIGALRM はアラームタイマーが発火したときに送られるシグナルです。
	•	act: 新しいシグナルハンドラの設定を指定する sigaction 構造体へのポインタ。
	•	oldact: 現在のシグナルハンドラの設定を保存するための sigaction 構造体へのポインタ（NULL を指定すると保存しない）。
	
	•	sa.sa_handler: シグナルが発生したときに呼び出される関数のポインタです。この例では timer_handler という関数が指定されています。timer_handler はシグナルが発生したときに実行される処理を定義しています。
	•	sa.sa_flags: シグナルハンドラの動作を制御するフラグです。SA_RESTART は、シグナルハンドラが実行中にブロックされたシステムコールを再起動するように指示します。つまり、シグナルハンドラが実行された後、シグナルによって中断されたシステムコールが再実行されることを意味します。
 */