#include <stdio.h>
#include <string.h>

int DrawVerticalHistogram_of_ascii_keyboardvalue(void)
{
	printf("数字を入力してください\n");
	int i, j, ch, max;
	int cnt[10] = { 0 };
	while ((ch = getchar()) != EOF) {
		if ('0' <= ch && ch <= '9') {
			cnt[ch - '0']++;
		}
		if(ch == '\n') break;
	}

	/* あらかじめ最も多く出現した値を調べておく */
	max = 0;
	for (i = 0; i < 10; i++) {
		if (max < cnt[i]) {
			max = cnt[i];
		}
	}

	printf("数字文字の出現回数\n\n");
	/* グラフの上のほうから順番に表示する */
	for (j = max; j >= 1; --j) {
		for (i = 0; i < 10; ++i) {
			if (cnt[i] >= j) {
				printf("*  ");
			}
			else {
				printf("   ");
			}
		}
		putchar('\n');
	}

	/* 横軸の数字を表示する */
	for (i = 0; i < 10; ++i) {
		printf("%1d  ", i);
	}
	putchar('\n');

	return 0;
}

#define MAX_ELEMENTS 100
#define MAX_LABEL_LENGTH 20

typedef struct {
    char names[MAX_ELEMENTS][MAX_LABEL_LENGTH];
    int values[MAX_ELEMENTS];
    int numElements;
} Graph;

void makeGraph(Graph *graph) {
    graph->numElements = 0;
}

void inputElement(Graph *graph, const char name[], int value) {
    if (graph->numElements < MAX_ELEMENTS) {
        strncpy(graph->names[graph->numElements], name, MAX_LABEL_LENGTH - 1);
        graph->names[graph->numElements][MAX_LABEL_LENGTH - 1] = '\0'; // Ensure null-termination
        graph->values[graph->numElements] = value;
        graph->numElements++;
    } else {
        printf("Maximum number of elements reached!\n");
    }
}

void drawHorizontalBarGraph(const Graph *graph) {
    int max = 0;

    // 最も多く出現した値を調べる
    for (int i = 0; i < graph->numElements; i++) {
        if (max < graph->values[i]) {
            max = graph->values[i];
        }
    }

    // 横向きの棒グラフを表示
    printf("Horizontal Bar Graph:\n");
    for (int i = 0; i < graph->numElements; i++) {
        printf("%*s ", MAX_LABEL_LENGTH, graph->names[i]);
        printf("[%2d]:", graph->values[i]);
        for (int j = 0; j < graph->values[i]; j++) {
            printf("■");
        }
		printf("\n");
    }
}

int main() {
    Graph graph;

    // グラフの作成を宣言
    makeGraph(&graph);

    // 各要素の名前と値を設定
    inputElement(&graph, "Apple",  5);
    inputElement(&graph, "Banana", 7);
    inputElement(&graph, "Cherry", 3);
    // inputElement(&graph, "Cherry", 9);

    // 横向きの棒グラフを表示
    drawHorizontalBarGraph(&graph);

    return 0;
}
