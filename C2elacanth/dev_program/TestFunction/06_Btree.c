#include "custom_common.h"

#define T 3  // B木の順序 (最小度)
#define MAX_KEYS (2 * T - 1)
#define MAX_CHILDREN (2 * T)

// B木のノード
typedef struct BTreeNode {
    int keys[MAX_KEYS];            // キー配列
    struct BTreeNode *children[MAX_CHILDREN]; // 子ノードへのポインタ
    int num_keys;                  // 現在のキー数
    bool is_leaf;                  // 葉ノードかどうか
} BTreeNode;

// B木の定義
typedef struct BTree {
    BTreeNode *root;
} BTree;

// ノードの作成
BTreeNode* create_node(bool is_leaf) {
    BTreeNode *node = (BTreeNode*)malloc(sizeof(BTreeNode));
    node->is_leaf = is_leaf;
    node->num_keys = 0;
    for (int i = 0; i < MAX_CHILDREN; i++) {
        node->children[i] = NULL;
    }
    return node;
}

// B木の作成
BTree* create_tree() {
    BTree *tree = (BTree*)malloc(sizeof(BTree));
    tree->root = create_node(true); // 初期状態では葉ノードのみ
    return tree;
}

// キーを線形検索
int search_key(BTreeNode *node, int key) {
    int idx = 0;
    while (idx < node->num_keys && key > node->keys[idx]) {
        idx++;
    }
    return idx;
}

// キーの検索
bool search(BTreeNode *node, int key) {
    if (node == NULL) return false;

    int idx = search_key(node, key);

    if (idx < node->num_keys && node->keys[idx] == key) {
        return true; // キーが見つかった
    }

    if (node->is_leaf) {
        return false; // 葉まで到達したが見つからない
    }

    return search(node->children[idx], key); // 子ノードで再帰検索
}

// ノードの分割
void split_child(BTreeNode *parent, int idx, BTreeNode *child) {
    BTreeNode *new_node = create_node(child->is_leaf);
    new_node->num_keys = T - 1;

    // 子ノードの後半のキーを新ノードにコピー
    for (int j = 0; j < T - 1; j++) {
        new_node->keys[j] = child->keys[j + T];
    }

    if (!child->is_leaf) {
        for (int j = 0; j < T; j++) {
            new_node->children[j] = child->children[j + T];
        }
    }

    child->num_keys = T - 1;

    // 親ノードの子を1つ後ろにずらす
    for (int j = parent->num_keys; j > idx; j--) {
        parent->children[j + 1] = parent->children[j];
    }

    parent->children[idx + 1] = new_node;

    // 親ノードにキーを追加
    for (int j = parent->num_keys - 1; j >= idx; j--) {
        parent->keys[j + 1] = parent->keys[j];
    }
    parent->keys[idx] = child->keys[T - 1];
    parent->num_keys++;
}

// キーの挿入（再帰）
void insert_non_full(BTreeNode *node, int key) {
    int idx = node->num_keys - 1;

    if (node->is_leaf) {
        // 葉ノードの場合、キーを正しい位置に挿入
        while (idx >= 0 && key < node->keys[idx]) {
            node->keys[idx + 1] = node->keys[idx];
            idx--;
        }
        node->keys[idx + 1] = key;
        node->num_keys++;
    } else {
        // 内部ノードの場合、正しい子ノードを探す
        while (idx >= 0 && key < node->keys[idx]) {
            idx--;
        }
        idx++;

        // 子ノードが満杯なら分割
        if (node->children[idx]->num_keys == MAX_KEYS) {
            split_child(node, idx, node->children[idx]);
            if (key > node->keys[idx]) {
                idx++;
            }
        }
        insert_non_full(node->children[idx], key);
    }
}

// B木へのキーの挿入
void insert(BTree *tree, int key) {
    BTreeNode *root = tree->root;

    if (root->num_keys == MAX_KEYS) {
        BTreeNode *new_root = create_node(false);
        new_root->children[0] = root;
        split_child(new_root, 0, root);
        insert_non_full(new_root, key);
        tree->root = new_root;
    } else {
        insert_non_full(root, key);
    }
}

// キーの検索関数
void search_key_in_btree(BTree *tree, int key) {
    if (search(tree->root, key)) {
        printf("キー %d はB木に存在します。\n", key);
    } else {
        printf("キー %d はB木に存在しません。\n", key);
    }
}

// B木の中身を表示（中序巡回）
void display(BTreeNode *node, int level) {
    if (node == NULL) return;

    int i;
    for (i = 0; i < node->num_keys; i++) {
        if (!node->is_leaf) display(node->children[i], level + 1);
        printf("Level %d: %d\n", level, node->keys[i]);
    }
    if (!node->is_leaf) display(node->children[i], level + 1);
}
// B木のノードを解放する
void free_btree_node(BTreeNode *node) {
    if (node == NULL) return;

    // 子ノードを再帰的に解放
    if (!node->is_leaf) {
        for (int i = 0; i <= node->num_keys; i++) {
            free_btree_node(node->children[i]);
        }
    }
    free(node);
}

// B木全体を解放する
void free_btree(BTree *tree) {
    if (tree == NULL) return;
    free_btree_node(tree->root);
    free(tree);
}
//@@@function
void B_tree() 
{
    BTree *tree = create_tree();

    insert(tree, 10);
    insert(tree, 20);
    insert(tree, 5);
    insert(tree, 6);
    insert(tree, 12);
    insert(tree, 30);
    insert(tree, 7);
    insert(tree, 17);

    printf("B木の内容:\n");
    display(tree->root, 0);

	// begin_time();
    search_key_in_btree(tree, 6);

	// printf("\nTest Elapsed Time: %8.3lf ms\n", end_time_ms());
	// begin_time();
    search_key_in_btree(tree, 15);

	// printf("\nTest Elapsed Time: %8.3lf ms\n", end_time_ms());

	//削除はむずかしい。

	free_btree(tree);



	int array[8] = {10, 20, 5, 6, 12, 30, 7, 17};
	// begin_time();
	for(int i = 0; i < 8; i++)
	{
		if(array[i] == 6)
		{
			printf("%d is found\n", array[i]);
			break;
		}
	}
	// printf("\nTest Elapsed Time: %8.3lf ms\n", end_time_ms());

	// begin_time();
	for(int i = 0; i < 8; i++)
	{
		if(array[i] == 15)
		{
			printf("%d is found\n", array[i]);
		}
	}
	printf("Not found %d\n", 15);

	// printf("\nTest Elapsed Time: %8.3lf ms\n", end_time_ms());

}