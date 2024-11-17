#include "common_func.h"
#define SIZE 20  		// ウエハの最大サイズ

void drawWaferMap(int x, int y) {
    int center = SIZE / 2; // 中心座標
    int radius = SIZE / 2; // ウエハの半径

    printf("   ");
    for (int i = 1; i <= SIZE; i++) {
        printf("%2d ", i);
    }
    printf("\n");

    for (int j = 1; j <= SIZE; j++) {
        printf("%2d ", j);
        for (int i = 1; i <= SIZE; i++) {
            int dx = i - center;
            int dy = j - center;
            double distance = sqrt(dx * dx + dy * dy);

            if (distance >= radius) {
                printf(" . "); // ウエハ外
            } else if (i == x && j == y) {
                printf("███"); // 特定座標（黒四角）
            } else {
                printf(" □ "); // ウエハ内（白四角）
            }
        }
        printf("\n");
    }
}
//@@@function
int WaferMapping() {
    int x, y;
    printf("Enter X coordinate (1-%d): ", SIZE);
    scanf("%d", &x);
    printf("Enter Y coordinate (1-%d): ", SIZE);
    scanf("%d", &y);

    if (x < 1 || x > SIZE || y < 1 || y > SIZE) {
        printf("Error: Coordinates out of range!\n");
        return 1;
    }

    drawWaferMap(x, y);

    return 0;
}