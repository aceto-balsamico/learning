#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generateRandomData(int array[], int size) {
    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        array[i] = rand() % 256; // 0から255までのランダムな整数を生成
    }
}

int main() {
    int dataArray[160];
	int array_size = sizeof dataArray / sizeof dataArray[0];
    generateRandomData(dataArray, array_size);

    FILE *file = fopen("data.txt", "w");
    if (!file) {
        fprintf(stderr, "Fine Not Open\n");
        return 1;
    }

    for (int i = 0; i < array_size; i++) {
        fprintf(file, "%02X ", dataArray[i]); // 2桁の16進数で出力
    }

    fclose(file);


	system("python3 plot_script.py");
    return 0;
}
