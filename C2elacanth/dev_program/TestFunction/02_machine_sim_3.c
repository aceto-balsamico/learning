#include "custom_common.h"

#define NUM_MACHINES 1
#define MAX_OPERATIONS 30

typedef enum {
    START,
    READ1,
    READ2,
    OUT1,
    OUT2,
    FINISH,
	END,
} OperationStage;

typedef struct {
    int machine_id;
    OperationStage stage;
    int key_start, key_read1, key_read2;
} Machine;

void perform_start(int machine_id, int *key_start) {
    *key_start = rand() % 100;
    printf("Machine %d: START (Key: %d)\n", machine_id, *key_start);
}

void perform_read1(int machine_id, int *key_read1) {
    *key_read1 = rand() % 100;
    printf("Machine %d: READ1 (Key: %d)\n", machine_id, *key_read1);
}

void perform_read2(int machine_id, int *key_read2) {
    *key_read2 = rand() % 100;
    printf("Machine %d: READ2 (Key: %d)\n", machine_id, *key_read2);
}

void perform_out1(int machine_id, int key) {
    printf("Machine %d: OUT1 (Key: %d)\n", machine_id, key);
}

void perform_out2(int machine_id, int key) {
    printf("Machine %d: OUT2 (Key: %d)\n", machine_id, key);
}

void perform_finish(int machine_id) {
    printf("Machine %d: FINISH\n", machine_id);
}
//@@@function
void machine_sim3() {
    srand(time(NULL));
    Machine machines[NUM_MACHINES];
    int total_operations = 0;
    // int last_out_machine = -1;  // 最後にOutを実行した機械を記録
	OperationStage prev_stage = END;
	int finish = 0;
    // 各機械の初期化
    for (int i = 0; i < NUM_MACHINES; i++) {
        machines[i].machine_id = i;
        machines[i].stage = START;
    }

	int count = 0;
	while(1)
	{
		printf("[%d]", count);
		count++;
        int machine_index = rand() % NUM_MACHINES;
        Machine *current_machine = &machines[machine_index];
		

        switch (current_machine->stage) {
            case START:
                perform_start(current_machine->machine_id, &current_machine->key_read1);
				prev_stage = current_machine->stage;
                current_machine->stage = READ2;
                break;

            case READ1:
                perform_read1(current_machine->machine_id, &current_machine->key_read1);
				prev_stage = current_machine->stage;
                current_machine->stage = OUT2;
                break;

            case READ2:
                perform_read2(current_machine->machine_id, &current_machine->key_read2);
				prev_stage = current_machine->stage;
                current_machine->stage = OUT1;
                break;

            case OUT1:
                perform_out1(current_machine->machine_id, current_machine->key_read1);
				prev_stage = current_machine->stage;
                // last_out_machine = current_machine->machine_id;
				if(rand() % 5 == 0 || total_operations > MAX_OPERATIONS) 				{current_machine->stage = FINISH;}
				else 									{current_machine->stage = READ1;}
                break;

            case OUT2:
                perform_out2(current_machine->machine_id, current_machine->key_read2);
				prev_stage = current_machine->stage;
                // last_out_machine = current_machine->machine_id;
				if(rand() % 5 == 0 || total_operations > MAX_OPERATIONS) 				{current_machine->stage = FINISH;}
				else 									{current_machine->stage = READ2;}
                break;

            case FINISH:
				printf("finish:");
				if(prev_stage == OUT1 || prev_stage == READ1) {perform_out2(current_machine->machine_id, current_machine->key_read2);}
				else if(prev_stage == OUT2 || prev_stage == READ2) {perform_out1(current_machine->machine_id, current_machine->key_read1);}
				else printf("error %d\n", current_machine->stage);
                

				if(total_operations > MAX_OPERATIONS) {current_machine->stage = END;}
				else 	current_machine->stage = START;  // 新しいサイクルの開始
                // last_out_machine = -1;  // FINISH後は制約をリセットshuffle
                break;

			default:
				break;
        }
        total_operations++;

		finish = 0;
		for(int i = 0; i < NUM_MACHINES; i++)
		{
			if(machines[i].stage == END)
			{
				finish++;
			}
		}
		if(finish == NUM_MACHINES) {break;}
		
		
    }

    printf("シミュレーション終了: 合計 %d 回の操作が行われました。\n", total_operations);
}