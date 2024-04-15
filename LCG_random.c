#include <stdio.h>
// グローバル変数としてseedを保持
static unsigned long long seed = 0;

// seedの更新
void srand_custom(unsigned long long new_seed) {
    seed = new_seed;
}

// 疑似乱数生成
unsigned char rand_custom() {
    // LCGのパラメータ
    const unsigned long long a = 1664525;		//seedとかけて正の整数にする（seedは奇数）
    const unsigned long long c = 1013904223;	//mと互いに素の数
    const unsigned long long m = 1ULL << 32;	//2<<32くらいの大きな正の整数
    
    // 疑似乱数の計算
    seed = (a * seed + c) % m;
    
    // 結果を0から0xFFの範囲にマッピング
    return (unsigned char)(seed & 0xFF);
}

int main() {
    // seedを設定
    srand_custom(12345);
    
    // 疑似乱数を10回生成して表示
    for (int i = 0; i < 10; ++i) {
        printf("%02X ", rand_custom());
    }
    printf("\n");
    
    return 0;
}
