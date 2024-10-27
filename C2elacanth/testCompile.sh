#!/bin/bash

create_include()
{
    # ライブラリのソースコードがあるディレクトリ
    LIBCDIR="analysis_library"

    # ヘッダーファイル
    HEADER_FILE="analysis/library_functions.h"

    # メニューに追加する関数のマーカー
    MARKER="@@@function"

    # ヘッダーファイルのヘッダー
    echo "// Automatically generated header file" > "$HEADER_FILE"
    echo "" >> "$HEADER_FILE"
    echo "#include <stdio.h>" >> "$HEADER_FILE"
    echo "" >> "$HEADER_FILE"

    # 関数のプロトタイプを追加
    trimmed_strings=()
    subdir_map=()

    while read -r source_file; do
        # ソースファイルのディレクトリを取得
        subdir=$(dirname "$source_file")
        subdir_name=${subdir#$LIBCDIR/}
        subdir_name=${subdir_name%%/*}

        # サブディレクトリ名が一度も記録されていなければ追加
        if [[ ! "${subdir_map[@]}" =~ "$subdir_name" ]]; then
            subdir_map+=("$subdir_name")
        fi

        marker_found=false
        while IFS= read -r line; do
            if [[ $marker_found == true && $line =~ [^[:space:]] ]]; then
                # '('が登場するまでの文字列にマッチする
                function_name=$(echo "$line" | sed -E 's/([^()]*)\(.*$/\1/') 
                echo "$function_name();" >> "$HEADER_FILE"
                marker_found=false

                # 関数名から最初の単語（型など）を除去し、配列に追加
                trimmed_string=$(echo "$function_name" | sed 's/^[^ ]* //')
                trimmed_strings+=("$trimmed_string $subdir_name")
            elif [[ $line == *"$MARKER"* ]]; then
                marker_found=true
            fi
        done < "$source_file"
    done < <(find "$LIBCDIR" -type f -name "*.c")

    # 構造体の定義
    echo "" >> "$HEADER_FILE"
    echo "typedef struct" >> "$HEADER_FILE"
    echo "{" >> "$HEADER_FILE"
    echo "    const char *name;" >> "$HEADER_FILE"
    echo "    void* func;" >> "$HEADER_FILE"
    echo "    const char *subdirName;" >> "$HEADER_FILE"
    echo "} s_FunctionPtr;" >> "$HEADER_FILE"
    echo "" >> "$HEADER_FILE"

    # グローバルの構造体配列とそのサイズを示す変数を定義
    echo "s_FunctionPtr g_FunctionsPtr[] =" >> "$HEADER_FILE"
    echo "{" >> "$HEADER_FILE"
    for entry in "${trimmed_strings[@]}"; do
        func_name=$(echo "$entry" | cut -d' ' -f1)
        func_subdir=$(echo "$entry" | cut -d' ' -f2)
        echo "    {\"$func_name\", (void*)$func_name, \"$func_subdir\"}," >> "$HEADER_FILE"
    done
    echo "};" >> "$HEADER_FILE"
    echo "" >> "$HEADER_FILE"
    echo "int g_numFunctionsPtr = sizeof(g_FunctionsPtr) / sizeof(g_FunctionsPtr[0]);" >> "$HEADER_FILE"
}

create_include