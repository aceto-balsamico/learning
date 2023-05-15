#include <x86intrin.h>
#pragma omp simd
#pragma omp parallel
//-funroll-loops
//-fomit-frame-pointer
/* コンパイラオプションは、コンパイラの全体的な振る舞いを変更するために使用されるオプションです。一方、#pragmaは、特定のコードブロックや関数についてのみ、コンパイラの動作を制御するために使用されます。
具体的には、コンパイラオプションは、コンパイラの最適化レベルや使用するCPUアーキテクチャなどのグローバルな設定を変更します。一方、#pragmaは、特定のコードブロックについて最適化レベルやアライメント、ループアンローリングの有効化などのローカルな設定を行うことができます。
また、#pragmaはコンパイラに対するヒントとして使用されることが多く、コンパイラがどのように最適化を行うかは、コンパイラの実装に依存します。一方、コンパイラオプションは、コンパイラの挙動を明示的に指定することができるため、より制御が可能です。
そのため、コンパイラオプションはプロジェクト全体の最適化レベルを統一する際に使用され、#pragmaは個別の関数やループなど、限定された範囲での最適化を行う際に使用されることが多いです。
 */
int popcnt_xor(const uint8_t* data1, const uint8_t* data2, size_t size) {
    size_t i;
    __m512i zmm1, zmm2, zmm3;
    int count = 0;
    for (i = 0; i < size; i += 64) {
        zmm1 = _mm512_loadu_si512((__m512i*)(data1 + i));
        zmm2 = _mm512_loadu_si512((__m512i*)(data2 + i));
        zmm3 = _mm512_xor_si512(zmm1, zmm2);
        count += _mm_popcnt_u64(_mm512_movepi8_mask(zmm3));
        count += _mm_popcnt_u64(_mm512_movepi8_mask(_mm512_srli_epi16(zmm3, 8)));
        count += _mm_popcnt_u64(_mm512_movepi8_mask(_mm512_srli_epi16(zmm3, 16)));
        count += _mm_popcnt_u64(_mm512_movepi8_mask(_mm512_srli_epi16(zmm3, 24)));
        count += _mm_popcnt_u64(_mm512_movepi8_mask(_mm512_srli_epi16(zmm3, 32)));
        count += _mm_popcnt_u64(_mm512_movepi8_mask(_mm512_srli_epi16(zmm3, 40)));
        count += _mm_popcnt_u64(_mm512_movepi8_mask(_mm512_srli_epi16(zmm3, 48)));
        count += _mm_popcnt_u64(_mm512_movepi8_mask(_mm512_srli_epi16(zmm3, 56)));
    }
    return count;
}

int popcnt_xor(const uint8_t* data1, const uint8_t* data2, size_t size) {
    size_t i;
    __m512i zmm1, zmm2, zmm3;
    int count = 0;
    for (i = 0; i < size; i += 64) {
        zmm1 = _mm512_loadu_si512((__m512i*)(data1 + i));
        zmm2 = _mm512_loadu_si512((__m512i*)(data2 + i));
        zmm3 = _mm512_xor_si512(zmm1, zmm2);
        count += _mm_popcnt_u64(_mm512_movepi8_mask(zmm3));
        count += _mm_popcnt_u64(_mm512_movepi8_mask(_mm512_srli_epi16(zmm3, 8)));
        count += _mm_popcnt_u64(_mm512_movepi8_mask(_mm512_srli_epi16(zmm3, 16)));
        count += _mm_popcnt_u64(_mm512_movepi8_mask(_mm512_srli_epi16(zmm3, 24)));
        count += _mm_popcnt_u64(_mm512_movepi8_mask(_mm512_srli_epi16(zmm3, 32)));
        count += _mm_popcnt_u64(_mm512_movepi8_mask(_mm512_srli_epi16(zmm3, 40)));
        count += _mm_popcnt_u64(_mm512_movepi8_mask(_mm512_srli_epi16(zmm3, 48)));
        count += _mm_popcnt_u64(_mm512_movepi8_mask(_mm512_srli_epi16(zmm3, 56)));
    }
    return count;
}

