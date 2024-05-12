#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
	int array_size;
	unsigned char* array;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct List {
	int size;
    Node *head;
    Node *tail;
} List;

// 空のリストを作成する関数
List *makelist() {
    List *list = (List *)malloc(sizeof(List));
    if (list == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
	list->size = 0;
    list->head = NULL;
    list->tail = NULL;
    return list;
}

// リストの末尾に新しいノードを追加する関数
void appendlist(List *list, int value) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    new_node->value = value;
    new_node->array_size = 0;
    new_node->array = NULL;
    new_node->prev = list->tail; // 新しいノードの前のノードを末尾ノードに設定
    new_node->next = NULL;

	list->size++;
    if (list->tail != NULL) {
        list->tail->next = new_node; // 末尾ノードの次のノードを新しいノードに設定
    }

    list->tail = new_node; // リストの末尾を更新
    if (list->head == NULL) {
        list->head = new_node; // リストが空の場合、ヘッドも新しいノードに設定
    }
}
void appendlist_witharray(List* list, int value, uint8_t* array, size_t array_size) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    list->size++;
    new_node->value = value;
    new_node->array_size = array_size;
    new_node->array = (uint8_t*)malloc(array_size * sizeof(uint8_t));
    if (new_node->array == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(new_node);  // ノードのメモリ解放
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < array_size; i++) {
        new_node->array[i] = array[i];
    }
    new_node->prev = list->tail;
    new_node->next = NULL;


    if (list->tail != NULL) {
        list->tail->next = new_node;
    }
    list->tail = new_node;

    if (list->head == NULL) {
        list->head = new_node;
    }

}
// リストの末尾のノードを取得する関数
Node *get_end_node(List *list) {
    if (list == NULL || list->tail == NULL) {
        return NULL; // 空のリストまたは末尾ノードがない場合は NULL を返す
    }
    return list->tail;
}

// リストの内容を表示する関数
void display_list(List *list) {
    printf("List values:\n");
    Node *current = list->head;
    while (current != NULL) {
        printf("value: %d, ", current->value);
		printf("prev:%p, ", current->prev);
		printf("current:%p, ", current);
		printf("next:%p, ", current->next);

		for(int arrayindex = 0; arrayindex < current->array_size; arrayindex++)
		{
			printf("%d,", current->array[arrayindex]);
		}
		
		printf("\n");
        current = current->next;
    }
}

// リスト全体を解放する関数
void freelist(List *list) {
    if (list == NULL) {
        return;
    }
    Node *current = list->head;
    while (current != NULL) {
        Node *next = current->next;
		if (current->array != NULL) {
            free(current->array);  // arrayがNULLでない場合、メモリを解放
        }
        free(current);
        current = next;
    }
    free(list);
}

int main() {
    List *my_list = makelist(); // 空のリストを作成

    for (int i = 0; i < 5; i++) {
        appendlist(my_list, i); // リストにノードを追加
    }

    display_list(my_list); // リストの内容を表示

    Node *end_node = get_end_node(my_list);
    if (end_node != NULL) {
        printf("NodeSize:%d, End Node Value: %d (%p)\n", my_list->size, end_node->value, end_node);
    }

	appendlist(my_list, 100); // リストにノードを追加
	display_list(my_list); // リストの内容を表示
	printf("NodeSize:%d, End Node Value: %d (%p)\n", my_list->size, get_end_node(my_list)->value, get_end_node(my_list));

	unsigned char array_data[10] = {1, 2, 3, 4, 5, 6, 7, 8 ,9 ,10};
	appendlist_witharray(my_list, 200, array_data, 10);
	display_list(my_list);

    freelist(my_list); // リスト全体を解放

    return 0;
}
