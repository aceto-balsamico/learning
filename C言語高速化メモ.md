---
marp: true
---
コンパイラとは？

コンパイラは複数の役割を持ちます。それぞれを簡単に説明します。

___
1. **Lexical Analysis**
- ソースコードを構成する文字列を意味のある最小単位(トークン)に分割する処理のこと。
2. **Syntax Analysis**
- 1で得られたトークンをもとにソースコードの構文を解析し、構文木を作成する。
3. **Semantic Analysis**
- 2で作成した構文木を使用し、変数の宣言、関数の定義、型のチェック、スコープの解決などが行われる。
4. **Intermediate Code Generation**
- 3で得られた情報をもとに中間コードを生成する。これはソースコードの構造を表す中間表現であり、プラットフォーム依存の機能を含まない抽象的な表現である。
___
5. **Optimization**
- 生成された中間コードに対して最適化を適用する。コードの削減、ループ展開、定数畳み込み、不要な計算の除去、コードの並べ替えがここで行われる。
6. **Code Generation**
- 最適化された中間コードをターゲットプラットフォームに合わせた機械語やアセンブリコードに変換する。
7. **Linking**
- 複数のソースファイルから生成されたオブジェクトファイルを隣家を使って単一の実行ファイルに結合する。

Optimizerは最適化の処理を行う部分であり、中間コードに対して様々な最適化手法を適用することで、生成されるコードの性能を改善することができます。
___
# inline展開
C言語のinline関数を使用して関数呼び出しを減らす最適化手法の一つ。
```
inline int add(int x, int y)
{
	return x + y;
}
void main()
{
	int a = 10, b = 20, c;
	c = add(a, b);
	printf("%d\n", c);
}
```

add関数をinline展開したとするとmain関数内add関数の呼び出しをx+yの式に置き換えます。これよりadd関数の呼び出しによるオーバーヘッドがなくなり、プログラムの実行速度が向上します。
inlineキーワードを付けることでコンパイラはinline展開を試みます。必ず行うわけではありません。また、コードサイズは増大します。
___
# builtin関数
CやC++においてコンパイラによって提供される組み込み関数(builtin function)です。
ユーザーはこれらの関数を通常の関数のように呼び出すことができますが、コンパイラはこれらの関数を呼び出すことなくプログラムの中で直接命令に展開することができます。GCCはコンパイル時にコンパイルされるプログラムが実行されるプラットフォームのアーキテクチャや命令セットを特定するため、プラットフォーム依存の最適化を行うことができます。つまり、プラットフォームやコンパイラのバージョン、コンパイル時の最適化レベルによって展開のされ方は異なります。
builtin関数は通常の関数よりも高速であることが多く、プログラムの実行速度を向上させることができます。ただし、プログラムの移植性が低下すること、必ずしも高速化するとは限らないことを意識して使用することが望ましいです。
___
# intrinsic関数
特定のプロセッサアーキテクチャ向けに最適化された組み込み関数のことをintrinsic関数といいます。有名なものだとIntelとArmがあり、それぞれAVX512f.hやarm_neon.hなどのheaderライブラリでプロセッサメーカーから提供されています。主にSIMD命令や浮動小数点演算を効率的に実行するために使用されます。intrinsic関数はそれぞれ特定のプロセッサ命令を直接呼び出すことができるのに加え、CやC++といった(アセンブリ言語と比較して)理解しやすい言語で書くことができます。つまり、高水準なコードで低水準なプロセッサ命令を直接呼び出すことができるのです。ただし、プロセッサやコンパイラがある程度の互換性を保っていますが、一部の関数の機能は特定のプロセッサにしか搭載されていない場合もあります。故にintrinsic関数を使用すると常に高速な動作になるとは限りません。
___
# SIMD命令
Single Instruction Multiple Data命令とはCPUが同時に複数のデータを処理するための命令セットのこと。これを使用することで一つの命令で複数のデータを同時に処理することができるため、処理速度を大幅に向上させることができます。そのかわり複雑な命令を出すことはできません。主にグラフィックス処理や音声処理、暗号化処理などで使用されますが、最近では機械学習やデータ処理にも使用され、SIMD命令ネイティブとも言えるデバイスがGPUです。代表的なSIMD命令セットとして、MMX,SSE,AVX,AVX-512などがあり、これらを使用するためにはそれぞれ対応するCPUが必要です。現在の最新命令セットはAVX-512であり、2016年(Skylake以降のCPU)にインテルにより発表された512ビット幅のベクトル演算をサポートしています。ここでいうベクトルとはスカラーの対義語としてのベクトルであり、要は512ビットの並列処理ができるということです。また、これらの命令セットはまとめてCPUがもつ基本的な命令に加えてより高度な機能を実現させるための命令セットということで拡張命令セットと呼ばれます。
___

***インラインアセンブラは使用しないのか？***
C言語にアセンブラコードを埋め込む機能であるインラインアセンブラを使用することで処理速度向上が見込めます。ただし、アセンブリ言語は最も低レベルで可読性が著しく低いこと、異なるアーキテクチャのCPU上では動作せず移植性が低いこと、コードの修正や保守の難易度が高くメンテナンスが困難であることからおすすめしません。これに対し、builtin関数やintrinsic関数はC言語の枠組みの中で実装されているため、これらの言語の知識がある程度ある人なら読むことができます。
___
# ベクトル演算とスカラー演算
ベクトル演算とは、複数のデータ要素をまとめて同時に演算を行うことを指します。一方、スカラー演算は、個々のデータ要素を単独で演算を行うことを指します。

ベクトル演算は、SIMD (Single Instruction Multiple Data) 命令を用いて実現されます。SIMD命令は、CPUに搭載された特別な命令セットによって実現されており、複数のデータ要素を同時に演算することができます。一方、スカラー演算は、一般的な命令セットを用いて実現されます。

ベクトル演算は、データ要素をまとめて処理するため、同じ演算を複数回行う必要がある場合や、配列の要素に一括で処理を行う場合などに有効です。一方で、データの依存関係が複雑であったり、データ数が少ない場合には、スカラー演算の方が効率的であることがあります。
___

SIMD命令による処理が行われるSIMDレジスタは通常のレジスタより大きく、複数のデータを同時に処理できます。通常レジスタのサイズは64bitCPUの場合は64bitです。つまりint型変数2個分です。一方でSIMDレジスタのサイズはAVX512に対応したCPUなら512ビットです。また、過去の命令セットにも対応していることがほとんどであるため、256ビットや128ビット、64ビットのSIMDレジスタも存在していることが多いです。ただし、メモリ領域からレジスタへのコピーは通常レジスタへコピーする速度と変わりません。そこで、メモリアラインメントを正しく事で格納速度を向上させることができます。これは通常レジスタを使用する場合も同様です。メモリアクセスはCPU内で最も遅い処理の一つとされています。これを向上させる数少ない手段です。
___
# メモリアラインメント
メモリ領域上のデータの配置を調整することをメモリアラインメントを正すといいます。メモリアラインメントが正しく行われている場合、データはキャッシュラインと呼ばれる固定サイズのメモリブロックに収まります。キャッシュラインはCPUがメモリアクセス時に一度の読み込むことができる採用単位であり、一般的には64バイトの大きさです。読み込まれたデータはキャッシュに保存され、それをレジスタなりへ転送します。正しくアラインメントされたデータは一度の転送でキャッシュラインに格納することができ、一度にレジスタへコピーすることができます。
つまり、32ビットのint型整数をアライメントする場合、4バイト境界に配置される必要があります。(4バイトアラインメント)理論的には64ビットCPUでは64バイトアラインメントが最も効率的であると言えます。
___
# その他のテクニック
1. ループアンローリング
- ループを回す回数を減らすことで分岐予測の失敗を減らす
2. パイプライン
- 複数の命令の実行を同時にすすめることで演算の遅延を最小限に抑える
3. プリフェッチ
- 次に必要とされるデータを予めメモリから読み込んでおくことでデータの待ち時間を減らす
4. ストアバッファ
- レジスタからメモリに書き込む際に一旦バッファに格納してから書き込むことで、メモリアクセスの待ち時間を短縮する
___
# CPUの仕事の仕方
1. フェッチ:CPUが命令を読み込み、メモリ領域のデータ(例.変数AとB)をロードする
2. デコード:命令を解釈して演算器に送信する
3. 実行:演算器がAとBを足し、結果を生成。
4. メモリアクセス:結果をメモリに書き込むためメモリアドレスを計算し、アクセスする。
5. ストア:演算結果がメモリに書き込まれる。
この一連のプロセスがCPU内で一連のパイプラインで行われます。異なる命令で各ステージが重なるように処理することができれば高速化に繋がります。
___
SIMD命令以外にも特定の演算を高速に実行できる特殊な演算ユニットがあります。
例えば、浮動小数点数の城さんと加算を同時に行うことができる演算ユニットであるFMA,ビット演算を高速に行うことができる命令セットであるBMIなどです。


