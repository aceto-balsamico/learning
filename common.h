#pragma once

typedef struct _List
{
	int val1;
	int val2;
	int val3;
	
	struct _List* top;
	struct _List* next;
	struct _List* prev;
}LIST;