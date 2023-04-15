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

double time_spent, begin, end;
struct timespec ts1, ts2;
void begin_time(int select_way)
{
	if(select_way == 0)	begin = clock();
	else if(select_way == 1)	clock_gettime(CLOCK_MONOTONIC, &ts1);
}
//0=CLOCKS_PER_SEC(CPU Execution time), 1=CLOCK_MONOTONIC(elapsed time)
void end_time(int select_way)
{
	end = clock();
	if(select_way == 0)
	{
		time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
		printf(" CPU Execution time is %f [ms]\n", time_spent * 1000);
	}
	else if(select_way == 1)
	{
		clock_gettime(CLOCK_MONOTONIC, &ts2);
		printf(" elapsed time is %f [ms]\n", (ts2.tv_sec - ts1.tv_sec) * 1000.0 + (ts2.tv_nsec - ts1.tv_nsec) / 1000000.0);	//milisec order
		// printf(" elapsed time is %ld.%ld [s]\n", (ts2.tv_sec - ts1.tv_sec) , (ts2.tv_nsec - ts1.tv_nsec));	//sec order
	}
} 






