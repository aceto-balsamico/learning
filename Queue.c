#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 100

// サンプルとして簡略化された構造体を使用
typedef struct {
    int id;
    char data[1024];  // 仮の大きなサイズのデータ
} MyStruct;

typedef struct Node {
    MyStruct* data;
    struct Node* next;
} Node;

typedef struct Queue {
    Node* front;
    Node* rear;
    int size;
} Queue;

Node* createNode(MyStruct* data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->front = queue->rear = NULL;
    queue->size = 0;
    return queue;
}

void enqueue(Queue* queue, int id, const char* data) {
    MyStruct* newData = (MyStruct*)malloc(sizeof(MyStruct));
    newData->id = id;
    snprintf(newData->data, sizeof(newData->data), "%s", data);

    Node* newNode = createNode(newData);

    if (queue->rear == NULL) {
        queue->front = queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }

    queue->size++;
}

void processA_and_free_queue(Queue* queue) {
    while (queue->front != NULL) {
        Node* temp = queue->front;
        MyStruct* data = temp->data;

        // 処理Aをここに追加
        printf("Processing data with id %d\n", data->id);

        queue->front = temp->next;
        free(temp);

        if (queue->front == NULL) {
            queue->rear = NULL;
        }

        queue->size--;

        // メモリを解放
        free(data);
    }

    // キュー自体の解放
    free(queue);
    // queue = NULL;  // この行は不要

}

int main() {
    Queue* queue = createQueue();

    // 構造体をキューに追加
    for (int i = 0; i < 100; ++i) {
        enqueue(queue, i, "SampleData");

        // キューのサイズとして戻り値を取得
        int currentSize = queue->size;
        printf("Current queue size: %d\n", currentSize);
    }

    // キューから構造体を取り出し、処理Aを実行
    processA_and_free_queue(queue);

    // キューのサイズが減っていることを確認
    printf("Queue size after processing: %d\n", queue->size);



    return 0;
}
