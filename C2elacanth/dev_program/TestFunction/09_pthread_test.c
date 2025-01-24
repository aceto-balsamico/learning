#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

// フラグと条件変数
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condA = PTHREAD_COND_INITIALIZER;
pthread_cond_t condB = PTHREAD_COND_INITIALIZER;
bool suspendA = false;
bool suspendB = true; // Bは初め一時停止

// 機械の構造体
typedef struct
{
	const char *name;
} Machine;

Machine MachineA = {"MachineA"};
Machine MachineB = {"MachineB"};

void *OperationA(void *arg)
{
	Machine *machine = (Machine *)arg;

	while (1)
	{
		pthread_mutex_lock(&mutex);	//mutex=同期メカニズム。スレッドがmutexをロックするとそのスレッドがリソースを占有する。
		while (suspendA)
		{
			pthread_cond_wait(&condA, &mutex);
		}
		pthread_mutex_unlock(&mutex);//mutexを解放あんロックすると他のスレッドがリソースを使用できるようになる

		// 処理A
		printf("%s: OperationA is running\n", machine->name);
		sleep(1);

		pthread_mutex_lock(&mutex);
		suspendA = true;
		suspendB = false;
		pthread_cond_signal(&condB);	//待機中のスレッドを起こす(再開する)
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

void *OperationB(void *arg)
{
	Machine *machine = (Machine *)arg;

	while (1)
	{
		pthread_mutex_lock(&mutex);
		while (suspendB)
		{
			pthread_cond_wait(&condB, &mutex); // BはsuspendAがfalseになるまで待機。条件が満たされるまで待機
		}
		pthread_mutex_unlock(&mutex);

		// 処理B
		printf("%s: OperationB is running\n", machine->name);
		sleep(1);

		pthread_mutex_lock(&mutex);
		suspendB = true;
		suspendA = false;
		pthread_cond_signal(&condA);	// AはsuspendBがfalseになるまで待機. 条件変数をすべての待機スレッドに通知する
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}
//@@@function
void pthread_test()
{
	pthread_t threadA, threadB;

	// スレッド作成
	// OperationAとBは別々のスレッドで実行される。
	// 両方のスレッドは一時停止と再開を管理するため、条件変数とミューテックスを使用
	// suspendA と suspendB フラグを使って、各スレッドの実行を制御する。
	//	条件変数を用いてスレッド間で適切に信号を送る。
	// 1秒ごとに互いに停止再開する。

	pthread_create(&threadA, NULL, OperationA, &MachineA);
	pthread_create(&threadB, NULL, OperationB, &MachineB);

	// スレッドの終了待機
	pthread_join(threadA, NULL);	//ほかスレッドの終了を待機する
	pthread_join(threadB, NULL);

	return;
	
}
