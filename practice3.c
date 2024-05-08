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
} Node;

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
		*(list->end) = list; // リスト全体の末尾を更新

		// デバッグ出力
		printf("Node Value: %d,", list->value);
		printf("Top: %p, ", list->top);
		printf("                   Node   : %p, \n", list);
	}
	else
	{
		// 新しいノードを作成
		new_node = (Node *)malloc(sizeof(Node));
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

		printf("list->end:%16p, ", *(list->end));
		printf("list->end:%16p, ", *(list->end));
		printf("list->end:%16p, ", *(list->end));
		// printf("list->end->next:%16p, ", (*(list->end))->next);//ok(nil)
		// printf("new_node:%16p, ", new_node);//ok(end struct add)
		//
		(*(list->end))->next = new_node; // 現在の末尾ノードのnextを新しいノードに設定
		*(list->end) = new_node;		 // リスト全体の末尾を更新

		// デバッグ出力
		printf("Node Value: %5d, ", new_node->value);
		printf("Top: %16p, ", new_node->top);
		printf("Prev: %16p, ", new_node->prev);
		printf("NodeAdd: %16p, ", new_node);
		printf("Next: %16p, ", new_node->next);
		printf("End: %16p, ", *(new_node->end));
		// printf("&((*(list->end))->next):%p, &(*(list->end)):%p", &((*(new_node->end))->next), &(*(new_node->end)));
		printf("\n");
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
Node *check_endstruct(Node *list)
{
	// Node check_node;
	// *(check_node.end) = *(list->end);
	// printf("check_node:%p,", *(list->end));

	return *(list->end);
}
int main()
{
	Node *my_list = makelist(); // 空のリストを作成

	for (int i = 0; i < 10; i++)
	{
		printf("my_list:%p,", my_list);
		appendlist(my_list, i); // リストにノードを追加
								// printf("(debug)endvalue:%d\n", (*(my_list->end))->value);
	}
	// printf("List values:\n");
	printf("my_list:%p,", my_list);
	printf("chekcend:%p, \n", check_endstruct(my_list));
	printf("my_list:%p,", my_list);
	printf("chekcend:%p, \n", check_endstruct(my_list));
	printf("my_list:%p,", my_list);
	printf("chekcend:%p, \n", check_endstruct(my_list));
	// printf("mylist->end:%p\n", *(my_list->end));//これはNG

	// printf("mylist->top->end->value:%d\n", (*(my_list->top->end))->value);
	Node new_node;
	// new_node.value = value;
	new_node.top = my_list->top;
	// new_node.next = NULL;
	// new_node.prev = *(my_list->end); // 現在の末尾ノードを前のノードとして設定
	// (*(my_list->end))->next = &new_node; // 現在の末尾ノードのnextを新しいノードに設定
	new_node.end = my_list->end;
	printf("newnode.top:%p, newnode.end:%p, my_list->end:%p\n", (new_node.top), *(new_node.end), *(my_list->end));
	printf("newnode.top:%p, newnode.end:%p, my_list->end:%p\n", (new_node.top), *(new_node.end), *(my_list->end));
	printf("newnode.top:%p, newnode.end:%p, my_list->end:%p\n", (new_node.top), *(new_node.end), *(my_list->end));
	Node *current = my_list;
	while (current != NULL)
	{
		// current->end = my_list->end;
		printf("value:%5d, prev:%16p, address:%16p, next:%16p, end:%16p, top:%16p\n",
			   current->value, current->prev, current, current->next, *(current->end), current->top);

		current = current->next;
	}
	/* Node *current = my_list;
	while (current != NULL)
	{
		// current->value = value;
		// current->top = my_list->top;
		// current->next = NULL;
		// current->prev = *(my_list->end); // 現在の末尾ノードを前のノードとして設定
		*(current->end) = *(my_list->end);

		printf("value:%5d, prev:%16p, address:%16p, next:%16p, end:%16p, top:%16p\n",
			   current->value, current->prev, current, current->next, *(current->end), current->top);

		current = current->next;
	} */
	// リスト全体を解放
	freelist(my_list);

	return 0;
}
