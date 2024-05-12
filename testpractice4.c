#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
	int value;

	struct Node* prev;
	struct Node* next;
} Node;

typedef struct List {
	int size;       // リストのサイズ（ノードの数）

	Node* head;
	Node* tail;
	Node** nodes;   // 各ノードのアドレスを保持する配列
} List;

// 空のリストを作成する関数
List* makelist(int num_nodes) 
{
	List* list = (List*)malloc(sizeof(List));
	if (list == NULL) 
	{
		fprintf(stderr, "Memory allocation failed.\n");
		exit(EXIT_FAILURE);
	}

	list->head = NULL;
	list->tail = NULL;
	list->size = num_nodes;
	list->nodes = (Node**)malloc(num_nodes * sizeof(Node*));
	if (list->nodes == NULL) 
	{
		fprintf(stderr, "Memory allocation failed.\n");
		exit(EXIT_FAILURE);
	}

	// 初期化: 各ノードのアドレスを NULL で初期化
	for (int i = 0; i < num_nodes; i++) 
	{
		list->nodes[i] = NULL;
	}

	return list;
}

// リストの末尾に新しいノードを追加する関数
void appendlist(List* list, int value) 
{
	Node* new_node = (Node*)malloc(sizeof(Node));
	if (new_node == NULL) 
	{
		fprintf(stderr, "Memory allocation failed.\n");
		exit(EXIT_FAILURE);
	}

	new_node->value = value;
	new_node->prev = list->tail;
	new_node->next = NULL;

	if (list->tail != NULL) 
	{
		list->tail->next = new_node;
	}

	list->tail = new_node;

	if (list->head == NULL) 
	{
		list->head = new_node;
	}

	// 新しいノードのアドレスを配列に格納
	int index = 0;
	while (list->nodes[index] != NULL && index < list->size) 
	{
		index++;
	}
	if (index < list->size) 
	{
		list->nodes[index] = new_node;
	}
	else 
	{
		fprintf(stderr, "List size exceeded.\n");
		exit(EXIT_FAILURE);
	}
}

// リストを解放する関数。freelist_nodeもfreelist_listもどちらもそこまで時間は変わらない。
void freelist_node(List* list) 
{
	Node* current = list->head;
	while (current != NULL) 
	{
		Node* next = current->next;
		free(current);
		current = next;
	}
	free(list->nodes);  // ノードのアドレス配列を解放
	free(list);         // リスト構造体を解放
}
void freelist_list(List* list) 
{
    // リスト内の各ノードのアドレスを解放
    for (int i = 0; i < list->size; i++) 
	{
        if (list->nodes[i] != NULL) 
		{
            free(list->nodes[i]);
        }
    }

    // ノードのアドレス配列を解放
    free(list->nodes);

    // リスト構造体を解放
    free(list);
}
Node* choiceNode(List* list, int num)
{
	Node* target;
	target = list->nodes[num];

	return target;
}
void printNodeInfo(Node *node) 
{
    if (node != NULL) 
	{
        printf("Node value: %d, ", node->value);
        printf("Previous node value: %d, ", (node->prev != NULL) ? node->prev->value : -1);
        printf("Next node value: %d\n", (node->next != NULL) ? node->next->value : -1);
    } 
	else 
	{
        printf("Node not found.\n");
    }
}
void printNodeValue(Node* node) 
{
    if (node != NULL) 
	{
        printf("%d ", node->value);
    }
}
void printList(List* list) 
{
    Node* current = list->head;
    while (current != NULL) 
	{
		printNodeValue(current);
        current = current->next;
    }
    printf("\n");
}

List* makefilterList(List* originalList, int (*condition)(int), int max_nodes) 
{
    List* filteredList = makelist(max_nodes);
    Node* current = originalList->head;

    while (current != NULL) 
	{
        if (condition(current->value)) 
		{
            appendlist(filteredList, current->value);
        }
        current = current->next;
    }

    return filteredList;
}
List* makefilterList_ByMinValue(List* originalList, int max_nodes) 
{
    List* filteredList = makelist(max_nodes);

    int minValue = 0;
	bool findValue = false;

	while(!findValue)
	{
		Node* current = originalList->head;
		while (current != NULL) 
		{
			if (current->value == minValue) 
			{
				appendlist(filteredList, current->value);
				findValue = true;
			}
			current = current->next;
		}
		if(findValue == false)
		{
			minValue++; // 最小値を増やして再度フィルタリング
		}
	}

    return filteredList;
}
int isValue100(int value) 
{
    return (value == 100);
}

// 条件関数: valueが偶数であるかどうかを判定する
int isEven(int value) 
{
    return (value % 2 == 0);
}
int isAll(int value)
{
	return (true);
}

int main() 
{
	// リストを作成し、ノードを追加する例
	int appendcont = 100;
	List* mylist = makelist(appendcont);  // ノード数が5個であるリストを作成

	for(int i = 0; i < appendcont; i++)
	{
		// appendlist(mylist, i);
		appendlist(mylist, i);
	}

	// 各ノードのアドレスを使用して、リスト内のノードにアクセスできます
	int nodecount = 0;
	for (int i = 0; i < mylist->size; i++) //nodeが1500を超えるならこっちのほうが早い
	{
		nodecount++;
	}
	printf("\nnode count:%d\n", nodecount);
	nodecount = 0;
	for (Node* current = mylist->head; current != NULL; current = current->next) //nodeが1500以下ならこっちのほうが早い
	{	
		nodecount++;
	}
	printf("\nnode count:%d, listsize:%d\n", nodecount, mylist->size);
	
	printNodeInfo(choiceNode(mylist, 0));//指定したNodeの情報を表示
	printNodeInfo(choiceNode(mylist, 1));
	printNodeInfo(choiceNode(mylist, 2));
	printNodeInfo(choiceNode(mylist, 3));
	printNodeInfo(choiceNode(mylist, 4));
	// printList(mylist);
	
	
	List* EvenList = makefilterList(mylist, isEven, mylist->size);
	printList(EvenList);
	printNodeInfo(choiceNode(EvenList, 1));

	List* minValueList = makefilterList_ByMinValue(mylist, mylist->size);
	printList(minValueList);



	freelist_list(EvenList);
	freelist_list(minValueList);
	freelist_list(mylist);
	
	return 0;
}
