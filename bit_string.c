#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 単一ビットの値を計算
unsigned int singleBitValue(int bit) {
	return 1U << bit;
}

// ビット範囲の値を計算
unsigned int bitRangeValue(int startBit, int endBit) {
	unsigned int value = 0;
	for (int i = startBit; i <= endBit; ++i) {
		value |= (1U << i);
	}
	return value;
}

// 文字列を解析してビット位置を取得し、対応する値を計算
unsigned int parseBitString(const char* bitString) {
	unsigned int result = 0;

	// 単一ビットの場合
	if (strchr(bitString, ':') == NULL) {
		int bit = atoi(bitString);
		result = singleBitValue(bit);
	}
	else {
		// ビット範囲の場合
		int startBit, endBit;
		sscanf(bitString, "%d:%d", &startBit, &endBit);
		if (startBit > endBit) {
			int temp = startBit;
			startBit = endBit;
			endBit = temp;
		}
		result = bitRangeValue(startBit, endBit);
	}

	return result;
}

int main() {
	char input[100];
	printf("Enter bit string (e.g., [5] or [2:0]): ");
	fgets(input, sizeof(input), stdin);

	// 文字列から[]を除去
	char* bitString = strtok(input, "[]");

	unsigned int result = parseBitString(bitString);
	printf("Result: 0x%X\n", result);

	return 0;
}
