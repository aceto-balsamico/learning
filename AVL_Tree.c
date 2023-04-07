#include "common.h"

// Calculate height
int height(NODE* N) 
{
	if(N == NULL)	return 0;
	return N->height;
}

int max(int a, int b) 
{
 	return (a > b) ? a : b;
}

// Create a node
NODE* newNode(int data) 
{
	NODE* node = (NODE*)malloc(sizeof(NODE));
	node->data = data;
	node->left = NULL;
	node->right = NULL;
	node->height = 1;
	return (node);
}

// Right rotate
NODE* rightRotate(NODE* y) 
{
	NODE* x = y->left;
	NODE* T2 = x->right;

	x->right = y;
	y->left = T2;

	y->height = max(height(y->left), height(y->right)) + 1;
	x->height = max(height(x->left), height(x->right)) + 1;

	return x;
}

// Left rotate
NODE* leftRotate(NODE* x) 
{
	NODE* y = x->right;
	NODE* T2 = y->left;

	y->left = x;
	x->right = T2;

	x->height = max(height(x->left), height(x->right)) + 1;
	y->height = max(height(y->left), height(y->right)) + 1;

	return y;
}

// Get the balance factor
int getBalance(NODE* N) 
{
	if(N == NULL)	return 0;
	return height(N->left) - height(N->right);
}

// Insert node
NODE* insertNode(NODE* node, int data) 
{
	// Find the correct position to insertNode the node and insertNode it
	if (node == NULL)
		return (newNode(data));

	if (data < node->data)
		node->left = insertNode(node->left, data);
	else if (data > node->data)
		node->right = insertNode(node->right, data);
	else
		return node;

	// Update the balance factor of each node and
	// Balance the tree
	node->height = 1 + max(height(node->left), height(node->right));

	int balance = getBalance(node);
	if (balance > 1 && data < node->left->data)
		return rightRotate(node);

	if (balance < -1 && data > node->right->data)
		return leftRotate(node);

	if (balance > 1 && data > node->left->data) 
	{
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	if (balance < -1 && data < node->right->data) 
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	return node;
}

NODE* minValueNode(NODE* node) 
{
	NODE* current = node;

	while (current->left != NULL)
		current = current->left;

	return current;
}

// Delete a nodes
NODE* deleteNode(NODE* root, int data) 
{
	// Find the node and delete it
	if (root == NULL)
		return root;

	if (data < root->data)
		root->left = deleteNode(root->left, data);

	else if (data > root->data)
		root->right = deleteNode(root->right, data);

	else 
	{
		if ((root->left == NULL) || (root->right == NULL)) 
		{
			NODE* temp = root->left ? root->left : root->right;

			if (temp == NULL) 
			{
				temp = root;
				root = NULL;
			} 
			else
				*root = *temp;
			free(temp);
		} else 
		{
			NODE* temp = minValueNode(root->right);

			root->data = temp->data;

			root->right = deleteNode(root->right, temp->data);
		}
	}

	if(root == NULL)
		return root;

	// Update the balance factor of each node and
	// balance the tree
	root->height = 1 + max(height(root->left), height(root->right));

	int balance = getBalance(root);
	if (balance > 1 && getBalance(root->left) >= 0)
		return rightRotate(root);

	if (balance > 1 && getBalance(root->left) < 0) 
	{
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}

	if (balance < -1 && getBalance(root->right) <= 0)
		return leftRotate(root);

	if (balance < -1 && getBalance(root->right) > 0) 
	{
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}

	return root;
}

// Print the tree
void printPreOrder(NODE* root) 
{
	if (root != NULL) 
	{
		printf("%d ", root->data);
		printPreOrder(root->left);
		printPreOrder(root->right);
	}
}
void printInOrder(NODE* root)
{
	if (root != NULL) 
	{
		printInOrder(root->left);
		printf("%d ", root->data);
		printInOrder(root->right);
	}
}
void printPostOrder(NODE* root)
{
	if (root != NULL) 
	{
		printPostOrder(root->left);
		printPostOrder(root->right);
		printf("%d ", root->data);
	}
}
int g_free_count = 0;
void FreeAllTree(NODE* root)
{
	if (root != NULL) 
	{
		FreeAllTree(root->left);
		FreeAllTree(root->right);
		free(root);
		g_free_count++;
	}
}

int main() 
{	
	//AVL Treeを利用して100までの数字で1〜1000を表示する
	begin_time();

	NODE* root = NULL;
	for(int vartical = 0; vartical < 100; vartical++)
	{
		for(int horizontal = 1; horizontal <= 10; horizontal++)
		{
			root = insertNode(root, vartical * 10 + horizontal);
		}
	}
	printInOrder(root);	//InOrderでノードを全表示
	FreeAllTree(root);
	end_time();

	printf("\n Free Node Count:%d\n", g_free_count);	//freeしたノードの数

	// root = deleteNode(root, 3);	//ノードを削除

  return 0;
}