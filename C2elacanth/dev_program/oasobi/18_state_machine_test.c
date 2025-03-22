#include "custom_common.h"

#define NUM_MACHINES 1

typedef enum e_operation_stage
{
	OperationSelect,
	TargetSelect,
	Prefix,
	TargetLevel,
	COM,
	ADD,
	DATA,
	LevelEXE,
	MultiEXE,
	EXECUTE,
	FINISH,
	END,
} OperationStage;
typedef struct machine_info
{
	int BLK;
	int type;
	int multi;
}MachineInfo;

typedef struct
{
	int machine_id;
	OperationStage stage;
	int key_start, key_read1, key_read2;
} Machine;

//@@@function
void state_machine_test(void)
{
	int count = 0;
	srand(time(NULL));
	Machine machines[NUM_MACHINES];
	int finish_page = 0;
	int current_page = 0;
	int current_level = 0;

	for (int i = 0; i < NUM_MACHINES; i++)
	{
		machines[i].machine_id = i;
		machines[i].stage = OperationSelect;
	}
	MachineInfo machine_info = {0, 0, 0}; 

	int COM_Plane = 0;
	while (1)
	{
		printf("[%d]\t", count);
		count++;
		int machine_index = rand() % NUM_MACHINES;
		Machine *current_machine = &machines[machine_index];

		switch (current_machine->stage)
		{
			case OperationSelect:

				printf("OperationSelect");

				current_machine->stage = TargetSelect;
				break;
			case TargetSelect:

				printf("TargetSelect");

				machine_info.BLK = rand() % 100;
				machine_info.type = 3;
				machine_info.multi = 4;
				
				current_page = 97;
				finish_page = 100;

				current_level = 1;

				current_machine->stage = Prefix;
				break;

			case Prefix:

				printf("Prefix");
				current_machine->stage = TargetLevel;
				break;

			case TargetLevel:

				printf("Level %d", current_level);
				current_machine->stage = COM;
				break;

			case COM:

				printf("COM");
				current_machine->stage = ADD;
				COM_Plane++;
				break;

			case ADD:

				printf("ADD:%d", machine_info.BLK);

				current_machine->stage = DATA;
				break;

			case DATA:

				printf("DATA");

				if (COM_Plane == machine_info.multi*machine_info.type)
				{
					current_machine->stage = EXECUTE;
				}
				else if(COM_Plane % machine_info.multi == 0)
				{
					current_machine->stage = LevelEXE;
				}
				else
				{
					current_machine->stage = MultiEXE;
				}
				break;

			case MultiEXE:

				printf("MultiEXE");
				current_machine->stage = Prefix;

				break;
			case LevelEXE:

				printf("LevelEXE");
				current_level++;
				current_machine->stage = Prefix;

				break;
			case EXECUTE:

				printf("EXECUTE");
				current_page++;

				current_machine->stage = FINISH;

				break;

			case FINISH:
				printf("finish\n");
				COM_Plane = 0;
				current_level = 1;

				if (current_page == finish_page)
				{
					current_machine->stage = END;
				}
				else
				{
					current_machine->stage = Prefix;
				}
				break;

			default:
				break;
		}
		printf("\n");

		if (current_machine->stage == END)
		{
			break;
		}
	}
}