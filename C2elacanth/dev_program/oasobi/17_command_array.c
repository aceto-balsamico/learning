#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_COMMANDS 256
#define MAX_PROHIBITS 256
#define MAX_CONFLICTS 256

typedef struct
{
	unsigned char *commands;
	size_t used_size;
	size_t capacity;
} CommandManager;

typedef struct
{
	unsigned char *prohibited;
	size_t used_size;
	size_t capacity;
} ProhibitManager;

typedef struct
{
	unsigned char (*conflicts)[2];
	size_t used_size;
	size_t capacity;
} ConflictManager;

void init_command_manager(CommandManager *cm, size_t capacity)
{
	cm->commands = malloc(capacity);
	cm->used_size = 0;
	cm->capacity = capacity;
}

void append_command(CommandManager *cm, unsigned char command)
{
	if (cm->used_size < cm->capacity)
	{
		cm->commands[cm->used_size++] = command;
	}
}

void init_prohibit_manager(ProhibitManager *pm, size_t capacity)
{
	pm->prohibited = malloc(capacity);
	pm->used_size = 0;
	pm->capacity = capacity;
}

void add_prohibit(ProhibitManager *pm, unsigned char command)
{
	if (pm->used_size < pm->capacity)
	{
		pm->prohibited[pm->used_size++] = command;
	}
}

void init_conflict_manager(ConflictManager *cm, size_t capacity)
{
	cm->conflicts = malloc(capacity * 2);
	cm->used_size = 0;
	cm->capacity = capacity;
}

void add_conflict(ConflictManager *cm, unsigned char cmd1, unsigned char cmd2)
{
	if (cm->used_size < cm->capacity)
	{
		cm->conflicts[cm->used_size][0] = cmd1;
		cm->conflicts[cm->used_size][1] = cmd2;
		cm->used_size++;
	}
}

void shuffle_commands(CommandManager *cm)
{
	for (size_t i = cm->used_size - 1; i > 0; --i)
	{
		size_t j = rand() % (i + 1);
		unsigned char temp = cm->commands[i];
		cm->commands[i] = cm->commands[j];
		cm->commands[j] = temp;
	}
}

void resolve_conflicts(CommandManager *cm, ProhibitManager *pm, ConflictManager *cfm)
{
	for (size_t i = 0; i < cm->used_size; ++i)
	{
		// Prohibited command check
		for (size_t j = 0; j < pm->used_size; ++j)
		{
			if (cm->commands[i] == pm->prohibited[j])
			{
				cm->commands[i] = cm->commands[--cm->used_size];
				--i;
				break;
			}
		}

		// Conflict command check
		for (size_t j = 0; j < cfm->used_size; ++j)
		{
			for (size_t k = i + 1; k < cm->used_size; ++k)
			{
				if ((cm->commands[i] == cfm->conflicts[j][0] && cm->commands[k] == cfm->conflicts[j][1]) ||
					(cm->commands[i] == cfm->conflicts[j][1] && cm->commands[k] == cfm->conflicts[j][0]))
				{
					cm->commands[k] = cm->commands[--cm->used_size];
					--k;
				}
			}
		}
	}
}

void print_commands(CommandManager *cm)
{
	for (size_t i = 0; i < cm->used_size; ++i)
	{
		printf("%02X ", cm->commands[i]);
	}
	printf("\n");
}
//@@@function
void command_array()
{
	srand((unsigned int)time(NULL));

	CommandManager cm;
	ProhibitManager pm;
	ConflictManager cfm;

	init_command_manager(&cm, MAX_COMMANDS);
	init_prohibit_manager(&pm, MAX_PROHIBITS);
	init_conflict_manager(&cfm, MAX_CONFLICTS);

	// コマンド追加
	for (unsigned char i = 0x01; i <= 0x20; ++i)
	{
		append_command(&cm, i);
	}
	print_commands(&cm);

	// 禁止コマンド追加
	add_prohibit(&pm, 0x02);
	add_prohibit(&pm, 0x15);
	add_prohibit(&pm, 0x2A);

	printf("after resolve\n");
	resolve_conflicts(&cm, &pm, &cfm);
	print_commands(&cm);

	// 組み合わせ禁止コマンド追加
	add_conflict(&cfm, 0x04, 0x1F);
	add_conflict(&cfm, 0x14, 0x09);
	add_conflict(&cfm, 0x19, 0x12);

	// コマンドシャッフルと競合解決
	printf("shuffle\n");
	shuffle_commands(&cm);
	print_commands(&cm);

	printf("after resolve\n");
	resolve_conflicts(&cm, &pm, &cfm);
	print_commands(&cm);

	printf("after size\n");
	cm.used_size = 4;
	print_commands(&cm);
	shuffle_commands(&cm);
	print_commands(&cm);

	// メモリ解放
	free(cm.commands);
	free(pm.prohibited);
	free(cfm.conflicts);

	return;
}
