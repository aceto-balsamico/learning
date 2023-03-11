#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <math.h>

// Linear Linked List
typedef struct _List
{
	int val1;
	int val2;
	int val3;
	int data[5000];
	
	struct _List* top;
	struct _List* next;
	struct _List* prev;
}LIST;

// AVL Tree
typedef struct _Node 
{
	int data;
	int A[5002];

	struct _Node* left;
	struct _Node* right;
	int height;
}NODE;

typedef struct _box
{
	int data1;
	int data2[1000];
	int data3[1000];
}DataBOX;


#define begin_time double time_spent = 0.0;\
clock_t begin = clock();

#define end_time clock_t end = clock();\
time_spent += (double)(end - begin) / CLOCKS_PER_SEC;\
printf("\n CPU Execution time is %f [ms]\n", time_spent * 1000);





