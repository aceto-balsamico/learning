#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 10  // 元の配列のサイズ
#define MAX_RANDOM_ELEMENTS 5  // ランダムに選択する最大要素数
#define MEMORY_POOL_SIZE_A 100  // メモリプールAのサイズ（構造体用）
#define MEMORY_POOL_SIZE_B 100  // メモリプールBのサイズ（配列用）

// 構造体の定義
typedef struct {
    int* array;
    int N_array;
} Node;

// 配列をシャッフルする
void shuffleArray(int* array, int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

int main() {
    srand(100);  // 乱数の種を設定

    // 元の配列を作成
    int originalArray[ARRAY_SIZE];
    for (int i = 0; i < ARRAY_SIZE; i++) {
        originalArray[i] = i + 1;
    }

    // メモリプールの確保
    char* memoryPoolA = (char*)malloc(MEMORY_POOL_SIZE_A);
    char* memoryPoolB = (char*)malloc(MEMORY_POOL_SIZE_B);
    char* currentPtrA = memoryPoolA;
    char* currentPtrB = memoryPoolB;
    size_t remainingMemoryA = MEMORY_POOL_SIZE_A;
    size_t remainingMemoryB = MEMORY_POOL_SIZE_B;

    // 構造体配列のポインタ
    Node* nodes = (Node*)memoryPoolA;
    int nodeCount = 0;
	int elementCount = 0;

    while (remainingMemoryA > sizeof(Node)) {
        // ランダムに要素数を決定
        int numElements = rand() % MAX_RANDOM_ELEMENTS + 1;
		elementCount += numElements;
        size_t requiredMemoryA = sizeof(Node);
        size_t requiredMemoryB = numElements * sizeof(int);

        // 十分なメモリが残っていない場合は終了
        if (remainingMemoryA < requiredMemoryA || remainingMemoryB < requiredMemoryB) {
            break;
        }

        // ノードを配置
        Node* node = (Node*)currentPtrA;
        currentPtrA += sizeof(Node);
        remainingMemoryA -= sizeof(Node);

        // 配列を配置
        node->array = (int*)currentPtrB;
        currentPtrB += numElements * sizeof(int);
        remainingMemoryB -= numElements * sizeof(int);

        // 配列をシャッフルしてランダムに要素を選択
        shuffleArray(originalArray, ARRAY_SIZE);
        for (int i = 0; i < numElements; i++) {
            node->array[i] = originalArray[i];
        }

        node->N_array = numElements;

        // ノードを構造体配列に追加
        nodes[nodeCount++] = *node;
    }

    // 結果を表示
    for (int i = 0; i < nodeCount; i++) {
        printf("Node %d with %d elements: ", i, nodes[i].N_array);
        for (int j = 0; j < nodes[i].N_array; j++) {
            printf("%d ", nodes[i].array[j]);
        }
        printf("\n");
    }
	for(int element = 0; element < elementCount; element++) {
		printf("%d,", ((int*)memoryPoolB)[element]);
	}
    // メモリを解放
    free(memoryPoolA);
    free(memoryPoolB);

    return 0;
}
