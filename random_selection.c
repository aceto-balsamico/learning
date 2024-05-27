#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef struct Node {
    int value;
    int weight;
	char* name;
} Node;

// ノードを管理するための構造体
typedef struct NodeList {
    Node* nodes;
    int size;
    int capacity;
} NodeList;

// NodeListの初期化
NodeList* init_node_list() {
    NodeList* node_list = (NodeList*)malloc(sizeof(NodeList));
    if (!node_list) {
        perror("Failed to allocate memory for node list");
        exit(EXIT_FAILURE);
    }
    node_list->size = 0;
    node_list->capacity = 4; // 初期容量
    node_list->nodes = (Node*)malloc(node_list->capacity * sizeof(Node));
    if (!node_list->nodes) {
        perror("Failed to allocate memory for nodes");
        free(node_list);
        exit(EXIT_FAILURE);
    }
    return node_list;
}

// ノードを作成してNodeListに追加する関数
void append_node(char* name, NodeList* node_list, int value, int weight) {
    if (node_list->size >= node_list->capacity) {
        node_list->capacity *= 2;
        node_list->nodes = (Node*)realloc(node_list->nodes, node_list->capacity * sizeof(Node));
        if (!node_list->nodes) {
            perror("Failed to reallocate memory for nodes");
            exit(EXIT_FAILURE);
        }
    }
    node_list->nodes[node_list->size].value = value;
    node_list->nodes[node_list->size].weight = weight;
	node_list->nodes[node_list->size].name = name;
    node_list->size++;
}

// 二分探索でノードを選択する関数
int binary_search(int* cumulative_weights, int n, int random_value) {
    int low = 0;
    int high = n - 1;
    while (low < high) {
        int mid = (low + high) / 2;
        if (random_value < cumulative_weights[mid]) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    return low;
}

// ノードをランダムに選択する関数
Node* RandomSelect(NodeList* node_list, bool flag_print) {
    int num_nodes = node_list->size;

    // 累積重みの計算
    int* cumulative_weights = (int*)malloc(num_nodes * sizeof(int));
    if (!cumulative_weights) {
        perror("Failed to allocate memory for cumulative weights");
        exit(EXIT_FAILURE);
    }
    
	//累積和の計算
	cumulative_weights[0] = node_list->nodes[0].weight;
    for (int i = 1; i < num_nodes; i++) {
        cumulative_weights[i] = cumulative_weights[i - 1] + node_list->nodes[i].weight;
    }

    // 乱数生成とノード選択
    int total_weight = cumulative_weights[num_nodes - 1];
    int random_value = rand() % total_weight;
    int selected_index = binary_search(cumulative_weights, num_nodes, random_value);
    Node* selected_node = &(node_list->nodes[selected_index]);

    // メモリ解放
    free(cumulative_weights);

	if(flag_print == true)printf("[Selected node]: %s\n", selected_node->name);

    return selected_node;
}

// NodeListの解放
void free_node_list(NodeList* node_list) {
    free(node_list->nodes);
    free(node_list);
}
#include "common.h"
int main() {
    srand(time(NULL));

    NodeList* node_list = init_node_list();

    // ノードの追加
    append_node("Hello", 			node_list, 1, 10);
    append_node("Goodbye", 			node_list, 2, 10);
    // append_node("Good Afternoon",	node_list, 3, 1);
    // 追加のノード
    // append_node("Good Morning", 	node_list, 4, 20);
    // append_node("Good Evening", 	node_list, 5, 15);

    // ランダムなノードを選択
	int one = 0, two = 0, three = 0, four = 0, five = 0;
	begin_time(1);
	for(int i = 0; i < 100000; i++)
	{
    	Node* selected_node = RandomSelect(node_list, false);
		if(selected_node->value == 1)one++;
		else if(selected_node->value == 2)two++;
		else if(selected_node->value == 3)three++;
		else if(selected_node->value == 4)four++;
		else if(selected_node->value == 5)five++;
	}
	end_time(1);
    // printf("Selected node value: %d\n", selected_node->value);
	printf("1: %d, 2: %d, 3: %d, ", one, two, three);
	printf("4: %d, 5: %d\n", four, five);

    // メモリ解放
    free_node_list(node_list);

    return 0;
}
