#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <ctime>

typedef struct {
	int* data;
	size_t size;
	size_t capacity;
} DynamicArray;

void initArray(DynamicArray* a, size_t initialSize) {
	a->data = (int*)malloc(initialSize * sizeof(int));
	a->size = 0;
	a->capacity = initialSize;
}

void insertArray(DynamicArray* a, int element) {
	if (a->size == a->capacity) {
		a->capacity *= 2;
		a->data = (int*)realloc(a->data, a->capacity * sizeof(int));
	}
	a->data[a->size++] = element;
}

void freeArray(DynamicArray* a) {
	free(a->data);
	a->data = NULL;
	a->size = 0;
	a->capacity = 0;
}
#define d_test_size 100000000
int cpp_main();
int c_main();
int main() 
{
	c_main();
	cpp_main();
	return 0;
}	

int cpp_main() {
    clock_t start, end;
    double cpu_time_used;
    std::vector<int> vec;
    size_t n = d_test_size; // 1000万要素を追加

    vec.reserve(5); // 初期容量を設定

    start = clock();
    for (size_t i = 0; i < n; i++) {
        vec.push_back(i);
    }
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

	int sum = 0;
	for(int i = 0; i < vec.size(); i++) {
		sum += vec[i];
	}
    std::cout << "C++: Time taken: " << cpu_time_used << " seconds, Sum:" << sum << std::endl;

    return 0;
}

int c_main() {
	clock_t start, end;
	double cpu_time_used;
	DynamicArray a;
	size_t n = d_test_size; // 1000万要素を追加

	initArray(&a, 5);

	start = clock();
	for (size_t i = 0; i < n; i++) {
		insertArray(&a, i);
	}
	end = clock();

	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

	int sum = 0;
	for (int i = 0; i < a.size; i++) {
		sum += a.data[i];
	}
	printf("C: Time taken: %f seconds, Sum: %d\n", cpu_time_used, sum);

	freeArray(&a);

	return 0;
}
