#include "custom_common.h"


#define NUM_MACHINES 4
#define MAX_STEPS 5
#define MAX_PARALLEL_OPERATION 4
#define MAX_EXECUTION_COUNT 2
#define INTERLEAVED_RATE 5

typedef enum 
{
	READY,
	BUSY,
}Condition;

// 機械の状態を管理する構造体
typedef struct 
{
	char name;
	int execution_count;
	Condition condition;
} Machine;

void Interleaved_Operation(Machine machines[], int current_machine_index);
void Operation_Function(Machine machines[], int current_machine_index);

// 使用可能な機械のリストを作成し、その中からランダムに選択
int select_random_machine(Machine machines[], int current_machine_index) 
{
	int available_machines[NUM_MACHINES - 1]; // 現在の機械以外を格納
	int N_availble_machines = 0;
	int selected_machine = 0; 

	// 使用可能な機械をリストアップ
	for (int i = 0; i < NUM_MACHINES; i++) 
	{
		if((i != current_machine_index) && (machines[i].condition == READY))
		{
			available_machines[N_availble_machines] = i;
			N_availble_machines++;
		}
	}

	// 使用中の機械がMAX_PARALLEL_OPERATIONを超える場合は-1を返す
	if(NUM_MACHINES - N_availble_machines >= MAX_PARALLEL_OPERATION) 
	{
		printf("---MAX_PARALLEL_OPERATION is over\n"); 
		for (int i = 0; i < NUM_MACHINES; i++) 
		{
			if(i != current_machine_index)
			{
				available_machines[N_availble_machines] = i;
				N_availble_machines++;
			}
		}
		selected_machine = available_machines[rand() % N_availble_machines];
		printf("Wait Finish Busy [%d]\n", selected_machine);
		// return -1;
		return selected_machine;
	}
	// 使用中の機械がMAX_EXECUTION_COUNTを超える場合は-1を返す
	selected_machine = available_machines[rand() % N_availble_machines];
	if(machines[selected_machine].execution_count >= MAX_EXECUTION_COUNT) {printf("---[%c] is over MAX_EXECUTION_COUNT\n", machines[selected_machine].name);return -1;}

	// ランダムに1つの機械を選択
	return selected_machine;
}
void Interleaved_Operation(Machine machines[], int current_machine_index) 
{
	// ランダムに別の機械を選択し、動作中でなければ再帰的に呼び出す
	int next_machine_index = select_random_machine(machines, current_machine_index);
	if(next_machine_index == -1) 
	{
		return;
	}

	machines[next_machine_index].condition = BUSY;
	Operation_Function(machines, next_machine_index);
	machines[next_machine_index].condition = READY;
}

// 機械のOperation関数
void Operation_Function(Machine machines[], int current_machine_index) 
{
	Machine *current_machine = &machines[current_machine_index];
	bool flag_reset = false;
	current_machine->execution_count++;

	for(int i = 0; i < MAX_STEPS; i++)
	{
		if(current_machine->name == 'A'){/*  */}
		else if(current_machine->name == 'B'){printf("\t");}
		else if(current_machine->name == 'C'){printf("\t\t");}
		else printf("\t\t\t");
		printf("Machine %c: Operation step %d\n", current_machine->name, i);
		
		if (rand() % INTERLEAVED_RATE == 0) 
		{
			// OperationFunctionを再帰的に呼び出す
			Interleaved_Operation(machines, current_machine_index);
		}

		if(rand()%30 == 0)
		{
			if(current_machine->name == 'A'){/*  */}
			else if(current_machine->name == 'B'){printf("\t");}
			else if(current_machine->name == 'C'){printf("\t\t");}
			else printf("\t\t\t");
			printf("------RESET!!---------\n");
			//Operationを途中で中断してreset:に飛んで関数を終了する
			flag_reset = true;
			goto reset;
		}
	}

	reset:
	{
		if(flag_reset == true)
		{		
			return;
		}
	}
}
void Init_Machine(Machine machines[])
{
	for(int i = 0; i < NUM_MACHINES; i++)
	{
		machines[i].name = 'A' + i;
		machines[i].execution_count = 0;
		machines[i].condition = READY;
	}
}
//@@@function
void Parallel_Operation() 
{
	// 機械の初期化
	Machine machines[NUM_MACHINES] = {};

	srand(time(NULL)); // 乱数の初期化

	// 最初の機械をランダムに選択して動作開始
	for(int i = 0; i < 2; i++)
	{
		Init_Machine(machines);
		printf("\n-----------------------START %d----------------------------\n", i);

		int start_machine_index = rand() % NUM_MACHINES;
		machines[start_machine_index].condition = BUSY;
		Operation_Function(machines, start_machine_index);
		machines[start_machine_index].condition = READY;
	}
}
