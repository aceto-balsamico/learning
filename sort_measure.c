#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ARRAY_SIZE 10000

// グローバル変数
struct timespec start_time, end_time;

// ソートアルゴリズムの宣言
void bubbleSort(int arr[], int n);				//O(n^2)
void quickSort(int arr[], int low, int high);	//O(nlogn), ソート済み配列に対してO(n^2)
void insertionSort(int arr[], int n);			//O(n^2), ソート済み配列に対しO(n)
void heapSort(int arr[], int n);				//O(nlogn) 安定
void selectionSort(int arr[], int n);			//O(n^2), メモリを使用しない
void mergeSort(int arr[], int left, int right);	//O(nlogn) 安定
void merge(int arr[], int left, int mid, int right);
void shuffleArray(int arr[], int n);

// ランダムな配列を生成
void generateRandomArray(int arr[], int size) {
	for (int i = 0; i < size; i++) {
		// arr[i] = rand() % 100;  // 0〜100のランダムな数
		arr[i] = i + (rand() % 100);  // 0〜100のランダムな数
	}
}

// 時間計測の開始
void measuretest_start() {
	clock_gettime(CLOCK_MONOTONIC, &start_time);
}

// 時間計測の終了
double measuretest_end() {
	clock_gettime(CLOCK_MONOTONIC, &end_time);
	return (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_nsec - start_time.tv_nsec) / 1e6;
}

// メイン関数
int main() {
	srand(time(NULL));

	int* originalArray = malloc(ARRAY_SIZE * sizeof(int));
	if (originalArray == NULL) {
		fprintf(stderr, "メモリの割り当てに失敗しました。\n");
		return 1;
	}

	// ランダムな配列を生成
	generateRandomArray(originalArray, ARRAY_SIZE);
	// ソートアルゴリズムの速度比較
	printf("ソートアルゴリズムの速度比較:\n");

	// バブルソート
	int* arrayForBubbleSort = malloc(ARRAY_SIZE * sizeof(int));
	memcpy(arrayForBubbleSort, originalArray, ARRAY_SIZE * sizeof(int));
	measuretest_start();
	bubbleSort(arrayForBubbleSort, ARRAY_SIZE);
	double bubbleTime = measuretest_end();
	printf("バブルソート: %.2f ms\n", bubbleTime);
	free(arrayForBubbleSort);

	// クイックソート
	int* arrayForQuickSort = malloc(ARRAY_SIZE * sizeof(int));
	memcpy(arrayForQuickSort, originalArray, ARRAY_SIZE * sizeof(int));
	measuretest_start();
	quickSort(arrayForQuickSort, 0, ARRAY_SIZE - 1);
	double quickTime = measuretest_end();
	printf("クイックソート: %.2f ms\n", quickTime);
	free(arrayForQuickSort);

	// 挿入ソート
	int* arrayForInsertionSort = malloc(ARRAY_SIZE * sizeof(int));
	memcpy(arrayForInsertionSort, originalArray, ARRAY_SIZE * sizeof(int));
	measuretest_start();
	insertionSort(arrayForInsertionSort, ARRAY_SIZE);
	double insertionTime = measuretest_end();
	printf("挿入ソート: %.2f ms\n", insertionTime);
	free(arrayForInsertionSort);

	// ヒープソート
	int* arrayForHeapSort = malloc(ARRAY_SIZE * sizeof(int));
	memcpy(arrayForHeapSort, originalArray, ARRAY_SIZE * sizeof(int));
	measuretest_start();
	heapSort(arrayForHeapSort, ARRAY_SIZE);
	double heapTime = measuretest_end();
	printf("ヒープソート: %.2f ms\n", heapTime);
	free(arrayForHeapSort);

	// 選択ソート
	int* arrayForSelectionSort = malloc(ARRAY_SIZE * sizeof(int));
	memcpy(arrayForSelectionSort, originalArray, ARRAY_SIZE * sizeof(int));
	measuretest_start();
	selectionSort(arrayForSelectionSort, ARRAY_SIZE);
	double selectionTime = measuretest_end();
	printf("選択ソート: %.2f ms\n", selectionTime);
	free(arrayForSelectionSort);

	// マージソート
	int* arrayForMergeSort = malloc(ARRAY_SIZE * sizeof(int));
	memcpy(arrayForMergeSort, originalArray, ARRAY_SIZE * sizeof(int));
	measuretest_start();
	mergeSort(arrayForMergeSort, 0, ARRAY_SIZE - 1);
	double mergeTime = measuretest_end();
	printf("マージソート: %.2f ms\n", mergeTime);
	free(arrayForMergeSort);

	// Fisher-Yates シャッフル
	measuretest_start();
	shuffleArray(originalArray, ARRAY_SIZE);
	double shuffleTime = measuretest_end();
	printf("Fisher-Yates シャッフル: %.2f ms\n", shuffleTime);


	// メモリ解放
	free(originalArray);

	return 0;
}

// 各ソートアルゴリズムの実装
void bubbleSort(int arr[], int n) {
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - i - 1; j++) {
			if (arr[j] > arr[j + 1]) {
				int temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}

void quickSort(int arr[], int low, int high) {
	if (low < high) {
		int pivot = arr[high];
		int i = (low - 1);
		for (int j = low; j < high; j++) {
			if (arr[j] < pivot) {
				i++;
				int temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
			}
		}
		int temp = arr[i + 1];
		arr[i + 1] = arr[high];
		arr[high] = temp;

		int pi = i + 1;
		quickSort(arr, low, pi - 1);
		quickSort(arr, pi + 1, high);
	}
}

void insertionSort(int arr[], int n) {
	for (int i = 1; i < n; i++) {
		int key = arr[i];
		int j = i - 1;
		while (j >= 0 && arr[j] > key) {
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = key;
	}
}

void heapify(int arr[], int n, int i) {
	int largest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;

	if (left < n && arr[left] > arr[largest])
		largest = left;

	if (right < n && arr[right] > arr[largest])
		largest = right;

	if (largest != i) {
		int temp = arr[i];
		arr[i] = arr[largest];
		arr[largest] = temp;

		heapify(arr, n, largest);
	}
}

void heapSort(int arr[], int n) {
	for (int i = n / 2 - 1; i >= 0; i--)
		heapify(arr, n, i);
	for (int i = n - 1; i >= 0; i--) {
		int temp = arr[0];
		arr[0] = arr[i];
		arr[i] = temp;

		heapify(arr, i, 0);
	}
}

void selectionSort(int arr[], int n) {
	for (int i = 0; i < n - 1; i++) {
		int minIndex = i;
		for (int j = i + 1; j < n; j++) {
			if (arr[j] < arr[minIndex])
				minIndex = j;
		}
		int temp = arr[minIndex];
		arr[minIndex] = arr[i];
		arr[i] = temp;
	}
}

void merge(int arr[], int left, int mid, int right) {
	int i, j, k;
	int n1 = mid - left + 1;
	int n2 = right - mid;

	int* L = malloc(n1 * sizeof(int));
	int* R = malloc(n2 * sizeof(int));

	for (i = 0; i < n1; i++)
		L[i] = arr[left + i];
	for (j = 0; j < n2; j++)
		R[j] = arr[mid + 1 + j];

	i = 0;
	j = 0;
	k = left;
	while (i < n1 && j < n2) {
		if (L[i] <= R[j]) {
			arr[k] = L[i];
			i++;
		}
		else {
			arr[k] = R[j];
			j++;
		}
		k++;
	}

	while (i < n1) {
		arr[k] = L[i];
		i++;
		k++;
	}

	while (j < n2) {
		arr[k] = R[j];
		j++;
		k++;
	}

	free(L);
	free(R);
}

void mergeSort(int arr[], int left, int right) {
	if (left < right) {
		int mid = left + (right - left) / 2;
		mergeSort(arr, left, mid);
		mergeSort(arr, mid + 1, right);
		merge(arr, left, mid, right);
	}
}
// 配列をランダムにシャッフルする。Fisher-Yates シャッフル法
void shuffleArray(int arr[], int n) {
	for (int i = n - 1; i > 0; i--) {
		int j = rand() % (i + 1);  // 0 から i までのランダムなインデックスを生成
		// arr[i] と arr[j] を交換
		int temp = arr[i];
		arr[i] = arr[j];
		arr[j] = temp;
	}
}