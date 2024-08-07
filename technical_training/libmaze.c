// libmaze.c
#include <stdio.h>
#include <stdlib.h>
#include "libmaze.h"

struct Node* create_maze() {
    struct Node *nodes[NODE_COUNT];
    
    // ノードの作成
    for (int i = 0; i < NODE_COUNT; i++) {
        nodes[i] = (struct Node*)malloc(sizeof(struct Node));
        nodes[i]->value = rand() % 100; // ランダムな値を設定
        nodes[i]->next1 = NULL;
        nodes[i]->next2 = NULL;
    }
    
    // ランダムにポインタを設定
    for (int i = 0; i < NODE_COUNT; i++) {
        nodes[i]->next1 = nodes[rand() % NODE_COUNT];
        nodes[i]->next2 = nodes[rand() % NODE_COUNT];
    }

    // 最後のノードにメッセージを設定
    nodes[NODE_COUNT - 1]->value = 999; // 特殊な値を設定して最後のノードを示す
    nodes[NODE_COUNT - 1]->next1 = NULL;
    nodes[NODE_COUNT - 1]->next2 = NULL;

    // 最初のノードを返す
    return nodes[0];
}

void free_maze(struct Node *start) {
    struct Node *nodes[NODE_COUNT];
    for (int i = 0; i < NODE_COUNT; i++) {
        nodes[i] = start->next1; // すべてのノードのポインタを取得（次のノードが次のループで参照される）
    }
    for (int i = 0; i < NODE_COUNT; i++) {
        free(nodes[i]); // ノードの解放
    }
}