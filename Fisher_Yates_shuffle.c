#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Fisher-Yatesシャッフルアルゴリズムを使って配列をランダムに並べ替える関数

typedef struct Node {
    int value;

    struct Node* next;
} Node;

typedef struct List {
    int size;      // リストのサイズ（ノードの数）
    Node* head;    // リストの先頭
    Node** nodes;  // ノードのアドレスを保持する配列
} List;

// リストの初期化
List* initList() {
    List* New_List = (List*)malloc(sizeof(List));
	New_List->size = 0;
    New_List->head = NULL;
    New_List->nodes = NULL;

	return New_List;
}

// 新しいノードをリストに追加
void addNode(List* list, int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->value = value;

    newNode->next = list->head;
    list->head = newNode;
    list->size++;
}

// リストを固定し、ノードのアドレスを配列に格納
void freezeList(List* list) {
    if (list->size == 0) {
        return;
    }

    // ノードアドレス配列を動的に確保
    list->nodes = (Node**)malloc(list->size * sizeof(Node*));
    Node* current = list->head;

    // リストの先頭から順にノードのアドレスを配列に格納
    for (int i = 0; i < list->size; i++) {
        list->nodes[i] = current;
        current = current->next;
    }
}

// 配列をランダムにシャッフル
void shuffleArray(Node** array, int size) {
    srand((unsigned int)time(NULL));

    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        Node* temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

// 配列アクセスの時間を計測
void measureAccessTime(List* list) {
    clock_t start, end;
    double arrayTime;

    // 配列を使用したアクセスの時間を計測
    start = clock();
    for (int i = 0; i < list->size; i++) {
        int value = list->nodes[i]->value;
        // 処理内容を追加する場合はここに書く
		printf("[%d]value:%d\n", i, value);
    }
    end = clock();
    arrayTime = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Array Access Time: %f seconds\n", arrayTime);
}

// リストおよびメモリの解放
void freeList(List* list) {
    if (list->nodes) {
        for (int i = 0; i < list->size; i++) {
            free(list->nodes[i]);
        }
        free(list->nodes);
    }
	free(list);
}
void shuffleAndPrintList(List* list) {
    if (list->size == 0) {
        return;
    }

    // ノードアドレス配列のコピーを作成
    Node** tempArray = (Node**)malloc(list->size * sizeof(Node*));
    for (int i = 0; i < list->size; i++) {
        tempArray[i] = list->nodes[i];
    }

    // コピーをシャッフル
    shuffleArray(tempArray, list->size);

    // シャッフル後の配列を表示
    for (int i = 0; i < list->size; i++) {
        printf("Node %d: Value = %d\n", i, tempArray[i]->value);
    }

    // コピー配列のメモリを解放
    free(tempArray);
}
List* createShuffledList(List* original) {
    if (original->size == 0) {
        return NULL;
    }

    // 新しいリストを動的に確保
    List* shuffledList = (List*)malloc(sizeof(List));
    shuffledList->size = original->size;
    shuffledList->head = NULL;  // シャッフルリストのheadは使用しない
    shuffledList->nodes = (Node**)malloc(shuffledList->size * sizeof(Node*));

    // オリジナルの配列をコピーしてシャッフル
    for (int i = 0; i < original->size; i++) {
        shuffledList->nodes[i] = original->nodes[i];
    }
    shuffleArray(shuffledList->nodes, shuffledList->size);

    return shuffledList;
}
// 新しく作成したシャッフルリストのメモリの解放
void freeShuffledList(List* list) {
    if (list->nodes) {
        free(list->nodes);
    }
    free(list);
}
int main() {
    List* list = initList();

    // リストにノードを追加
    for (int i = 0; i < 5; i++) {
        addNode(list, i);
    }

    // リストを固定し、配列にノードのアドレスを格納
    freezeList(list);
	printf(" original\n");measureAccessTime(list);

    // 配列をランダムにシャッフル
	printf(" only value shuffle\n");shuffleAndPrintList(list);

    // 配列アクセスの時間を計測
    printf(" original again\n");measureAccessTime(list);

	List* NewShuffleList = createShuffledList(list);
	printf(" copy and shuffle\n");measureAccessTime(NewShuffleList);
	freeShuffledList(NewShuffleList);

	printf(" original again\n");measureAccessTime(list);
	

    // リストおよびメモリの解放
    freeList(list);

    return 0;
}
