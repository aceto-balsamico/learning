#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct BinaryTree {
    Node* root;
} BinaryTree;

Node* createNode(int key) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    newNode->left = newNode->right = NULL;
    return newNode;
}

Node* minValueNode(Node* node) {
    Node* current = node;

    while (current->left != NULL) {
        current = current->left;
    }

    return current;
}
void insert(Node* root, int key) {
    if (root->key == -1) {
        root->key = key;
    } else if (key < root->key) {
        if (root->left == NULL) {
            root->left = createNode(key);
        } else {
            insert(root->left, key);
        }
    } else if (key > root->key) {
        if (root->right == NULL) {
            root->right = createNode(key);
        } else {
            insert(root->right, key);
        }
    }
}
Node* deleteNode(Node* root, int key) {
    if (root == NULL) {
        return root;
    }

    if (key < root->key) {
        root->left = deleteNode(root->left, key);
    } else if (key > root->key) {
        root->right = deleteNode(root->right, key);
    } else {
        if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        }

        Node* temp = minValueNode(root->right);

        root->key = temp->key;

        root->right = deleteNode(root->right, temp->key);
    }

    return root;
}


void preorderTraversal(Node* root) {
    if (root != NULL) {
        printf("%d ", root->key);
        preorderTraversal(root->left);
        preorderTraversal(root->right);
    }
}
void inorderTraversal(Node* root) {
    if (root != NULL) {
        inorderTraversal(root->left);
        printf("%d ", root->key);
        inorderTraversal(root->right);
    }
}
void postorderTraversal(Node* root) {
    if (root != NULL) {
        postorderTraversal(root->left);
        postorderTraversal(root->right);
        printf("%d ", root->key);
    }
}
void freeTree(Node* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

void initBinaryTree(BinaryTree* tree) {
    tree->root = createNode(-1);
}
// FNV-1aハッシュ関数
uint32_t fnv1aHash(uint32_t a, uint32_t b, uint32_t c) {
    uint32_t hash = 2166136261u;  // FNV offset basis

    hash ^= a;
    hash *= 16777619u;  // FNV prime
    hash ^= b;
    hash *= 16777619u;
    hash ^= c;
    hash *= 16777619u;

    return hash;
}
int main() {
    BinaryTree tree;
    initBinaryTree(&tree);

    insert(tree.root, 5);
    insert(tree.root, 3);
    insert(tree.root, 7);
    insert(tree.root, 2);
    insert(tree.root, 4);
	// for(int i = 0; i < 3; i++)
	// {
	// 	for(int j = 0; j < 3; j++)
	// 	{
	// 		for(int k = 0; k < 3; k++)
	// 		{
	// 			insert(tree.root, (fnv1aHash(i, j, k))%100);
	// 		}
	// 	}
	// }

    printf("Original Tree: ");
    preorderTraversal(tree.root);
    printf("\n");

	int delete_element = 30;
    deleteNode(tree.root, delete_element);
    printf("Tree after deleting node with key : %d, ", delete_element);
    preorderTraversal(tree.root);
    printf("\n");

    freeTree(tree.root);
    tree.root = NULL;
    printf("Tree after freeing all nodes: ");
    inorderTraversal(tree.root);
    printf("\n");

    return 0;
}
