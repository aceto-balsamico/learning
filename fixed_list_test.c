#include <stdio.h>
#include <stdlib.h>
#include "common.h"

typedef struct sNode
{
	int data;
	struct sNode* next;
	struct sNode* prev;
} Node;

typedef struct sList
{
	int list_size;
	Node** address_array;

	Node* top;
	Node* end;
} List;

List* MakeList()
{
	List* list = (List*)malloc(sizeof(List));

	list->list_size = 0;
	list->address_array = NULL;
	list->top = NULL;
	list->end = NULL;

	return list;
}

void AppendNode(List* list, int data)
{
	Node* node = (Node*)malloc(sizeof(Node));
	list->list_size++;
	node->data = data;
	node->next = NULL;
	node->prev = list->end;

	if (list->top == NULL)
	{
		list->top = node;
		list->end = node;
	}
	else
	{
		list->end->next = node;
		list->end = node;
	}
}
void LockList(List* list)
{
    if (list->address_array != NULL)
    {
        free(list->address_array);
    }
    list->address_array = (Node**)malloc(sizeof(Node*) * list->list_size);

    int index = 0;
    for (Node* current = list->top; current != NULL; current = current->next)
    {
        list->address_array[index] = current;
        index++;
    }
    if (index == list->list_size)
        printf("index:%d:Lock Success\n", index);
}

void FreeList(List* list)
{
	if(list->address_array != NULL)free(list->address_array);
	for(Node* Current = list->top; Current != NULL; Current = Current->next)
	{
		Node* next = Current->next;
		free(Current);
	}
	free(list);
}
int Random(int min, int max)
{
    return rand() % (max - min + 1) + min;
}
void ShuffleList(List* list)
{
    if (list->list_size <= 1)
        return;

    // Fisher-Yatesアルゴリズムでaddress_arrayをシャッフル
    for (int i = list->list_size - 1; i > 0; i--)
    {
        int j = Random(0, i);
        Node* temp = list->address_array[i];
        list->address_array[i] = list->address_array[j];
        list->address_array[j] = temp;
    }

    // シャッフルされたaddress_arrayを基にリストを再構築
    list->top = list->address_array[0];
    list->top->prev = NULL;
    Node* current = list->top;
    for (int i = 1; i < list->list_size; i++)
    {
        current->next = list->address_array[i];
        list->address_array[i]->prev = current;
        current = current->next;
    }
    list->end = current;
    list->end->next = NULL;
}
void ShuffleListDirect_lock(List* list)
{
    if (list->list_size <= 1)
        return;

    // 配列を動的に割り当て
    Node** nodes = (Node**)malloc(sizeof(Node*) * list->list_size);

    // リストのノードを配列に格納
    Node* current = list->top;
    for (int i = 0; i < list->list_size; i++)
    {
        nodes[i] = current;
        current = current->next;
    }

    // Fisher-Yatesアルゴリズムで配列をシャッフル
    srand((unsigned int)time(NULL));
    for (int i = list->list_size - 1; i > 0; i--)
    {
        int j = Random(0, i);
        Node* temp = nodes[i];
        nodes[i] = nodes[j];
        nodes[j] = temp;
    }

    // シャッフルされた配列を基にリストを再構築
    list->top = nodes[0];
    list->top->prev = NULL;
    current = list->top;
    for (int i = 1; i < list->list_size; i++)
    {
        current->next = nodes[i];
        nodes[i]->prev = current;
        current = current->next;
    }
    list->end = current;
    list->end->next = NULL;

    // address_arrayを更新
    if (list->address_array != NULL)
    {
        free(list->address_array);
    }
    list->address_array = nodes;
}

//愚直に線形探索してシャッフルする。遅い。
void ShuffleList_Direct2(List* list) 
{
    if (list->list_size <= 1)
        return;

    // ノードのポインタを格納する配列を用意
    Node* current = list->top;
    for (int i = 0; i < list->list_size; i++) {
        int j = Random(0, i);
        // currentノードとlistのj番目のノードを入れ替える
        Node* temp = current;
        if (i != j) {
            Node* swap = list->top;
            for (int k = 0; k < j; k++) {
                swap = swap->next;
            }
            // Swap the nodes in the list
            if (current->prev) current->prev->next = swap;
            if (current->next) current->next->prev = swap;
            if (swap->prev) swap->prev->next = current;
            if (swap->next) swap->next->prev = current;

            Node* tempPrev = current->prev;
            Node* tempNext = current->next;
            current->prev = swap->prev;
            current->next = swap->next;
            swap->prev = tempPrev;
            swap->next = tempNext;

            if (i == 0) list->top = swap;
            if (j == 0) list->top = current;
            if (i == list->list_size - 1) list->end = swap;
            if (j == list->list_size - 1) list->end = current;
        }
        current = current->next;
	}
}

int PrintList(List* list, int mode)
{
	int count = 0;
    Node* current = list->top;
    while (current != NULL)
    {
        if(mode == 0)
		{
			printf("[%d] prev:%12p, current:%12p, next:%12p\n", current->data, current->prev, current, current->next);
		}
		else if(mode == 1)
		{
			count += current->data;
		}
        current = current->next;
    }
	return count;
}
int PrintList_index(List* list, int mode)
{
	int count = 0;
	for(int i = 0; i < list->list_size; i++)
	{
		if(mode == 0)
		{
			printf("[%d] prev:%12p, current:%12p, next:%12p\n", list->address_array[i]->data, list->address_array[i]->prev, list->address_array[i], list->address_array[i]->next);
		}
		else if(mode == 1)
		{
			count += list->address_array[i]->data;
		}
		
	}
	return count;
}
List* MakeCopyListSegment(List* list, int start, int end) {
    if (start < 0 || end >= list->list_size || start > end) {
        return NULL; // 無効な範囲の場合
    }

    List* new_list = MakeList();
    int segment_size = end - start + 1;

    // 新しいリストのaddress_arrayを確保
    new_list->address_array = (Node**)malloc(sizeof(Node*) * segment_size);
    
    for (int i = start; i <= end; i++) {
        // ノードをコピー
        Node* node = (Node*)malloc(sizeof(Node));
        node->data = list->address_array[i]->data;
        node->prev = (i == start) ? NULL : new_list->address_array[i - start - 1];
        node->next = NULL;

        // 新しいリストにノードを追加
        if (i == start) {
            new_list->top = node;
        } else {
            new_list->address_array[i - start - 1]->next = node;
        }

        new_list->address_array[i - start] = node;
    }

    new_list->end = new_list->address_array[segment_size - 1];
    new_list->list_size = segment_size;

    return new_list;
}
int main()
{
    List* list = MakeList();
	
	int count = 500;
	for (int i = 0; i < count; i++)
	{
		AppendNode(list, i);
	}
	
	begin_time(1);
	ShuffleListDirect_lock(list);	//Lockされてないリストに対して直接シャッフルする。その家庭で作った配列を使用してLockもする。
	end_time(1);

	begin_time(1);
    LockList(list);
	ShuffleList(list);	//一番早い.ただしLockListが必要なのでトータルでは遅い。
	end_time(1);

	begin_time(1);
    // printf("resutl:%d\n", PrintList(list, 1));
	printf("resutl:%d\n", PrintList_index(list, 1));//こっちのほうが早い。
	end_time(1);

	printf("\n");
	List* Copy = MakeCopyListSegment(list, 10, 20);
	// PrintList(Copy, 0);

	Node* current = NULL;//ループの外で宣言したほうが早い。
	for (int i = 0; i < Copy->list_size; i++) 
	{
   		current = Copy->address_array[i];
		printf("[%d] prev:%12p, current:%12p, next:%12p\n", current->data, current->prev, current, current->next);
    	
	}


	
	FreeList(Copy);


    // メモリ解放
	FreeList(list);
    
    return 0;
}