#!/bin/bash


create_include()
{

# ライブラリのソースコードがあるディレクトリ
LIBCDIR="libc"

# ヘッダーファイル
HEADER_FILE="src/library_functions.h"

# マーカー
MARKER="@@@function"

# ヘッダーファイルの初期化
echo "// Automatically generated header file" > "$HEADER_FILE"
echo "" >> "$HEADER_FILE"
echo "#include <stdio.h>" >> "$HEADER_FILE"
echo "" >> "$HEADER_FILE"

# ライブラリのソースコードを処理してヘッダーファイルに関数のプロトタイプを追加
trimmed_strings=()
while read -r source_file; do
    marker_found=false
    while IFS= read -r line; do
        if [[ $marker_found == true && $line =~ [^[:space:]] ]]; then
            # '('が登場するまでの文字列にマッチする
            function_name=$(echo "$line" | sed -E 's/([^()]*)\(.*$/\1/') 
            echo "$function_name();" >> "$HEADER_FILE"
            marker_found=false

            # 関数名から最初の単語（型など）を除去し、配列に追加
            trimmed_string=$(echo "$function_name" | sed 's/^[^ ]* //')
            trimmed_strings+=("$trimmed_string")
        elif [[ $line == *"$MARKER"* ]]; then
            marker_found=true
        fi
    done < "$source_file"
done < <(find "$LIBCDIR" -type f -name "*.c")

# 関数ポインタ構造体の定義
echo "" >> "$HEADER_FILE"
echo "typedef struct {" >> "$HEADER_FILE"
echo "    const char *name;" >> "$HEADER_FILE"
echo "    void* func;" >> "$HEADER_FILE"
echo "} s_FunctionPtr;" >> "$HEADER_FILE"
echo "" >> "$HEADER_FILE"

# グローバル構造体変数とint型変数の宣言
echo "s_FunctionPtr g_FunctionsPtr[] = {" >> "$HEADER_FILE"
for func in "${trimmed_strings[@]}"; do
	echo "    {"\"$func\"", (void*)$func}," >> "$HEADER_FILE"
done
echo "};" >> "$HEADER_FILE"
echo "" >> "$HEADER_FILE"
echo "int g_numFunctionsPtr = sizeof(g_FunctionsPtr) / sizeof(g_FunctionsPtr[0]);" >> "$HEADER_FILE"
}


if [ "$1" == "comp" ]; then
	# make comp を実行
	make comp -f makefile.mk
elif [ "$1" == "link" ]; then
	# make link を実行
	make link -f makefile.mk
else
	# デフォルトの場合はすべてのコマンドを順に実行
	make comp -f makefile.mk
	# ./create_include.sh
	create_include
	make link -f makefile.mk
fi