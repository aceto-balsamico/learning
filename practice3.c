#include <stdio.h>
#include <stdlib.h>

// ノードの構造体定義
typedef struct _Node 
{
	int value;
	struct _Node *top;
	struct _Node *next;
	struct _Node *prev;
	struct _Node **end;
}Node;

// リストを作成する関数
Node *makelist() 
{
	Node *list = malloc(sizeof(Node));
	if (list == NULL) 
	{
		fprintf(stderr, "Memory allocation failed.\n");
		exit(EXIT_FAILURE);
	}
	list->value = 0;
	list->top = NULL;
	list->next = NULL;
	list->prev = NULL;
	list->end = &list; // 初めはリスト自身の末尾を指す

	return list;
}

// リストにノードを追加する関数
void appendlist(Node *list, int value) 
{
	// 新しいノードを作成
	Node *new_node;

	if (list->top == NULL) 
	{
		// リストが空の場合、makelistで作成したノードを使用
		list->value = value;
		list->top = list;
		(*(list->end))->next = list; // 現在の末尾ノードのnextを新しいノードに設定
		*(list->end) = list; // リスト全体の末尾を更新

		// デバッグ出力
		printf("Node Value: %d,", list->value);
		printf("Top: %p, \n", (void *)list->top);

	} 
	else 
	{
		// 新しいノードを作成
		// printf("top is not null\n");
		new_node = malloc(sizeof(Node));
		if (new_node == NULL) 
		{
			fprintf(stderr, "Memory allocation failed.\n");
			exit(EXIT_FAILURE);
		}
		
		new_node->value = value;
		new_node->top = list->top;
		new_node->next = NULL;
		new_node->prev = *(list->end); // 現在の末尾ノードを前のノードとして設定
		new_node->end = list->end;

		(*(list->end))->next = new_node; // 現在の末尾ノードのnextを新しいノードに設定
		*(list->end) = new_node; // リスト全体の末尾を更新

		// デバッグ出力
		printf("Node Value: %d,", new_node->value);
		printf("Top: %p, ", (void *)new_node->top);
		printf("Prev: %p, ", (void *)new_node->prev);
		printf("NodeAdd: %p, ", (void *)new_node);
		printf("Next: %p, ", (void *)new_node->next);
		printf("End: %p\n", (void *)*(new_node->end));
	}

}

// リストを解放する関数
void freelist(Node *list) 
{
	Node *current = list;
	Node *next;

	while (current != NULL) 
	{
		next = current->next;
		free(current);
		current = next;
	}
}

int main() 
{
	Node *my_list = makelist(); // 空のリストを作成

	for (int i = 0; i < 10; i++) 
	{
		appendlist(my_list, i); // リストにノードを追加
	}
	printf("List values:\n");
	Node *current = my_list;
	while (current != NULL) 
	{
		printf("value:%d, prev:%12p, address:%12p, next:%12p, end:%12p, top:%12p\n", 
		current->value, current->prev, current, current->next, *(current->end), current->top);
		current = current->next;
	}
	// リスト全体を解放
	freelist(my_list);

	return 0;
}
