#include "common.h"

#define TABLE_SIZE 100

typedef struct node 
{
	// char* key;
	uint32_t key;
	int value;
	struct node* next;
} Node;

//衝突をチェックするために各hash値の示す構造体アドレスを配列で保存する。
Node* table[TABLE_SIZE];

int hash(char* key) 
{
	//keyの文字のASCIIを足し合わせてTABLE_SIZEで剰余を取る
	int hash_value = 0;
	for (int i = 0; i < strlen(key); i++) {
		hash_value += key[i];
	}
	return hash_value % TABLE_SIZE;
}
uint32_t hash_int(uint32_t key)
{
	//murmur hash
	uint32_t x = key;
	x ^= x >> 16;
	x *= 0x7feb352d;
	x ^= x >> 15;
	x *= 0x846ca68b;
	x ^= x >> 16;
	return x % TABLE_SIZE;
}

// void put(char* key, int value) 
void put(uint32_t key, int value) 
{
	//new_nodeに値を代入し、衝突が起こらなかったら直接追加、起こったらchainして追加する
	int index = hash_int(key);
	Node* new_node = (Node*)malloc(sizeof(Node));
	new_node->key = key;
	new_node->value = value;
	new_node->next = NULL;

	if (table[index] == NULL) 
	{
		table[index] = new_node;
	}
	else 
	{
		Node* current_node = table[index];
		while (current_node->next != NULL) 
		{
			current_node = current_node->next;
		}
		current_node->next = new_node;
	}
}

// int get(char* key) 
int get(uint32_t key) 
{
	int index = hash_int(key);
	Node* current_node = table[index];
	while (current_node != NULL) 
	{
		// if (strcmp(current_node->key, key) == 0) 
		if (current_node->key == 0) 
		{
			return current_node->value;
		}
		current_node = current_node->next;
	}
	return -1;
}

// void remove_item(char* key) 
void remove_item(uint32_t key) 
{
	// int index = hash(key);
	int index = hash_int(key);
	Node* current_node = table[index];
	Node* prev_node = NULL;
	while (current_node != NULL) 
	{
		// if (strcmp(current_node->key, key) == 0) 
		if (current_node->key == 0) 
		{
			if (prev_node == NULL) 
			{
				table[index] = current_node->next;
			}
			else 
			{
				prev_node->next = current_node->next;
			}
			free(current_node);
			return;
		}
		prev_node = current_node;
		current_node = current_node->next;
	}
}

void print_table() 
{
	for (int i = 0; i < TABLE_SIZE; i++) 
	{
		Node* current_node = table[i];
		printf("Index %d: ", i);
		while (current_node != NULL) 
		{
			// printf("(%s, %d) ", current_node->key, current_node->value);
			printf("(%u, %d) ", current_node->key, current_node->value);
			current_node = current_node->next;
		}
		printf("\n");
	}
}
void clear_table() 
{
	for (int i = 0; i < TABLE_SIZE; i++) 
	{
		Node* current_node = table[i];
		while (current_node != NULL) 
		{
			Node* temp_node = current_node;
			current_node = current_node->next;
			free(temp_node);
		}
		table[i] = NULL;
	}
}

int main() 
{
	begin_time();
	for(char x = 0; x < 50; x++)
	{
		put(x, x + 50);
	}
	// put("apple", 10);
	// put("banana", 20);
	// put("cherry", 30);
	// put("durian", 40);

	// printf("Value for key 'apple': %d\n", get("apple"));
	// printf("Value for key 'banana': %d\n", get("banana"));
	// printf("Value for key 'cherry': %d\n", get("cherry"));
	// printf("Value for key 'durian': %d\n", get("durian"));

	// remove_item("banana");

	// printf("After removing key 'banana':\n");
	print_table();
	end_time();
	
	clear_table();

	return 0;
}
