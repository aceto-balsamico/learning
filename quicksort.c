#include <stdio.h>
#include <stdlib.h>
#include "common.h"

void swap(int* x, int* y)
{
	int buffer = 0;
	buffer = *x;
	*x = *y;
	*y = buffer;
}

int partition(int X[], int left, int right)
{
	int pivot = X[left];
	int low = left + 1;
	int high = right;
	while (low <= high)
	{
		while (X[low] <= pivot)
		{
			low++;
		}
		while (X[high] > pivot)
		{
			high--;
		}
		if (low <= high)
		{
			swap(&X[low], &X[high]);
		}
	}
	swap(&X[left], &X[high]);
	return high;
}
int partition2(int X[], int left, int right)
{
	int pivot = X[left];
	int i = left + 1;
	int j = right;

	while(i < j)
	{
		while(i <= right && X[i] < pivot)i++;

		while(X[j]>pivot) j++;

		if(i <= j)
		{
			swap(X+i, X+j);
			i++;
			j--;
		}
	}
	swap(X+left, X+j);

	return j;
}

void quickSort(int X[], int left, int right)
{
	if (left < right)
	{
		int pivot = partition(X, left, right);
		quickSort(X, left, pivot - 1);
		quickSort(X, pivot + 1, right);
	}
}

void quickSort2(int X[], int left, int right)
{
	int j = 0;
	if (left < right)
	{
		j=partition(X, left, right);
		quickSort2(X, left, j - 1);
		quickSort2(X, j + 1, right);
	}
}

int compare(const void* a, const void* b)
{
	return (*(int*)a - *(int*)b);
}
#define ARR_SIZE 100000
int main()
{
	int arr[ARR_SIZE] = {};
	int arr2[ARR_SIZE] = {};
	int arr3[ARR_SIZE] = {};

	for (size_t i = 0; i < ARR_SIZE; i++)
	{
		arr[i] = rand() % ARR_SIZE;
		arr2[i] = arr[i];
		arr3[i] = arr[i];
	}
	begin_time(1);
	quickSort2(arr2, 0, ARR_SIZE - 1);
	end_time(1);
	
	begin_time(1);
	quickSort(arr, 0, ARR_SIZE - 1);
	end_time(1);

	begin_time(1);
	qsort(arr3, ARR_SIZE, sizeof(int), compare);
	end_time(1);


	return 0;
}