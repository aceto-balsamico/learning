#include "custom_common.h"

#define MAX_COMMANDS 256
#define MAX_PROHIBITED 256
#define MAX_CONFLICTS 256

// SoA構造体定義
struct CommandManager
{
	unsigned char *commands; // コマンド配列
	size_t used_size;		 // 現在使用中のコマンド数
	size_t max_size;		 // コマンド配列の最大容量
};

// 禁止コマンド管理
unsigned char prohibited[MAX_PROHIBITED];
size_t prohibited_count = 0;

// 組み合わせ禁止管理
unsigned char conflicts[MAX_CONFLICTS][2];
size_t conflict_count = 0;

// コマンドマネージャーの初期化
void init_command_manager(struct CommandManager *cm, size_t max_size)
{
	cm->commands = (unsigned char *)malloc(max_size * sizeof(unsigned char));
	cm->used_size = 0;
	cm->max_size = max_size;
}

// コマンドマネージャーの解放
void free_command_manager(struct CommandManager *cm)
{
	free(cm->commands);
	cm->commands = NULL;
	cm->used_size = 0;
	cm->max_size = 0;
}

// コマンド追加関数
void add_command(struct CommandManager *cm, unsigned char command)
{
	if (cm->used_size < cm->max_size)
	{
		cm->commands[cm->used_size++] = command;
	}
}

// 禁止コマンド追加関数
void add_prohibited_command(unsigned char command)
{
	if (prohibited_count < MAX_PROHIBITED)
	{
		prohibited[prohibited_count++] = command;
	}
}

// 組み合わせ禁止追加関数
void add_conflict_pair(unsigned char a, unsigned char b)
{
	if (conflict_count < MAX_CONFLICTS)
	{
		conflicts[conflict_count][0] = a;
		conflicts[conflict_count][1] = b;
		conflict_count++;
	}
}

// 配列のシャッフル関数
void shuffle_commands(struct CommandManager *cm)
{
	for (size_t i = cm->used_size - 1; i > 0; --i)
	{
		size_t j = rand() % (i + 1);
		unsigned char temp = cm->commands[i];
		cm->commands[i] = cm->commands[j];
		cm->commands[j] = temp;
	}
}

// 禁止コマンドチェック関数
int is_prohibited(unsigned char command)
{
	for (size_t i = 0; i < prohibited_count; ++i)
	{
		if (command == prohibited[i])
			return 1;
	}
	return 0;
}

// 組み合わせ禁止チェック関数
int check_conflict(unsigned char a, unsigned char b)
{
	for (size_t i = 0; i < conflict_count; ++i)
	{
		if ((a == conflicts[i][0] && b == conflicts[i][1]) ||
			(a == conflicts[i][1] && b == conflicts[i][0]))
		{
			return 1;
		}
	}
	return 0;
}

// 禁止コマンドと組み合わせ禁止コマンドのフィルタリング
void filter_commands(struct CommandManager *cm)
{
	for (size_t i = 0; i < cm->used_size;)
	{
		if (is_prohibited(cm->commands[i]))
		{
			cm->commands[i] = cm->commands[--cm->used_size];
			continue;
		}

		for (size_t j = i + 1; j < cm->used_size; ++j)
		{
			if (check_conflict(cm->commands[i], cm->commands[j]))
			{
				cm->commands[j] = cm->commands[--cm->used_size];
				break;
			}
		}
		++i;
	}
}

// コマンドの表示関数
void print_commands(struct CommandManager *cm)
{
	for (size_t i = 0; i < cm->used_size; ++i)
	{
		printf("%02X ", cm->commands[i]);
	}
	printf("\n");
}

//@@@function
void command_array(void)
{
	srand((unsigned int)time(NULL));

	struct CommandManager cm;
	init_command_manager(&cm, MAX_COMMANDS);

	for (unsigned char i = 0; i < 16; ++i)
	{
		add_command(&cm, i);
	}

	// 禁止コマンドの追加
	add_prohibited_command(0x0C);
	add_prohibited_command(0x25);
	add_prohibited_command(0x2A);

	// 組み合わせ禁止コマンドの追加
	add_conflict_pair(0x06, 0x04); // 0x06があれば0x04は禁止
	add_conflict_pair(0x0F, 0x0D); // 0x0Fがあれば0x0Dは禁止

	printf("Original Commands:\n");
	print_commands(&cm);

	shuffle_commands(&cm);
	printf("\nShuffled Commands:\n");
	print_commands(&cm);

	filter_commands(&cm);
	printf("\nFiltered Commands:\n");
	print_commands(&cm);
	cm.used_size = 4;
	print_commands(&cm);
	shuffle_commands(&cm);
	print_commands(&cm);

	free_command_manager(&cm);
	return;
}
