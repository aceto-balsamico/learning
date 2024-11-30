#!/bin/bash

create_include()
{
    # ライブラリのソースコードがあるディレクトリ
    LIBCDIR="dev_program"

    # ヘッダーファイル
    HEADER_FILE="body/library_functions.h"

    # メニューに追加する関数のマーカー
    MARKER="@@@function"

    # ヘッダーファイルのヘッダー
    echo "// Automatically generated header file" > "$HEADER_FILE"
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

                # 取得した関数名から型名を除去し、配列に追加
                trimmed_string=$(echo "$function_name" | sed 's/^[^ ]* //')
                trimmed_strings+=("$trimmed_string $subdir_name")
            elif [[ $line == *"$MARKER"* ]]; then
                marker_found=true
            fi
        done < "$source_file"
    done < <(find "$LIBCDIR" -type f -name "*.c" | sort)

    # 構造体の定義
    echo "" >> "$HEADER_FILE"
    echo "typedef struct" >> "$HEADER_FILE"
    echo "{" >> "$HEADER_FILE"
    echo "    void* func;" >> "$HEADER_FILE"
    echo "    const char *func_name;" >> "$HEADER_FILE"
    echo "    const char *subdir_name;" >> "$HEADER_FILE"
    echo "} s_FunctionPtr;" >> "$HEADER_FILE"
    echo "" >> "$HEADER_FILE"

    # グローバルの構造体配列とそのサイズを示す変数を定義
    echo "s_FunctionPtr g_FunctionsPtr[] =" >> "$HEADER_FILE"
    echo "{" >> "$HEADER_FILE"
    for entry in "${trimmed_strings[@]}"; do
        func_name=$(echo "$entry" | cut -d' ' -f1)
        func_subdir=$(echo "$entry" | cut -d' ' -f2)
        echo "    {(void*)$func_name, \"$func_name\", \"$func_subdir\"}," >> "$HEADER_FILE"
    done
    echo "};" >> "$HEADER_FILE"
    echo "" >> "$HEADER_FILE"
    echo "int g_numFunctionsPtr = sizeof(g_FunctionsPtr) / sizeof(g_FunctionsPtr[0]);" >> "$HEADER_FILE"
}

# RAMディスクの設定
RAMDISK_SIZE=1G
RAMDISK_MOUNT_POINT="/mnt/ramdisk"

# RAMディスクの作成
setup_ramdisk() 
{
	# sudo mount -t tmpfs -o size=1G,mode=1777 tmpfs "$HOME/ramdisk"
	sudo mount -t tmpfs -o size=1G,mode=1777 tmpfs "$RAMDISK_MOUNT_POINT"
	echo "RAMDISK: $RAMDISK_MOUNT_POINT"
}

num_jobs=$(sysctl -n hw.logicalcpu)
# RAMディスク上のディレクトリ作成関数
setup_ramdisk_dirs() {
    export OBJDIR="$RAMDISK_MOUNT_POINT/obj"
    export BINDIR="$RAMDISK_MOUNT_POINT/bin"
    
    # RAMディスク上に必要なディレクトリの作成
    sudo mkdir -p "$OBJDIR"
    sudo mkdir -p "$BINDIR"
    sudo mkdir -p "$OBJDIR/src" "$OBJDIR/body"
    
    # srcとbodyディレクトリ内のサブディレクトリも作成
    find src -type d -exec sudo mkdir -p "$OBJDIR"/{} \;
    find body -type d -exec sudo mkdir -p "$OBJDIR"/{} \;

	sudo chown $USER:$USER $HOME/ramdisk
	chmod 777 $HOME/ramdisk
}

# ビルド関数
build_project() {
    # 初期設定（デフォルト：デバッグオプション）
    compile_options="-g -Wall"
    ramdisk_mode=false
    
    # 引数に応じたオプション設定
    for arg in "$@"; do
        case $arg in
            -op)
                compile_options="-O3"
                ;;
            -ram)
                ramdisk_mode=true
                ;;
        esac
    done

	rm obj/body/menu.o

    if $ramdisk_mode; then
        # RAMディスク上でのビルド
        echo "ファストビルドを開始します (RAMディスク使用)"
		setup_ramdisk
        setup_ramdisk_dirs
        echo "Logical Proc=$num_jobs"
        time make SHELL_ARG="$compile_options"  OBJDIR="$OBJDIR" BINDIR="$BINDIR" -j"$num_jobs"

        # RAMディスクからローカルのobjとbinディレクトリへコピー
        echo "ビルドが完了しました。出力ファイルを保存先へ移動します..."
        cp -r "$OBJDIR/"* obj/
        cp -r "$BINDIR/"* bin/
        
        # RAMディスクのアンマウント
        echo "RAMディスクをアンマウントします..."
        sudo umount "$RAMDISK_MOUNT_POINT"
    else
        # 通常ビルド
        echo "通常ビルドを開始します..."
        echo "Logical Proc=$num_jobs"
        time make SHELL_ARG="$compile_options" -j"$num_jobs"
    fi
}
# 戻り値よりmakeの結果を確認
return_value=1
create_include
build_project "$@"
return_value=$?

if [ $return_value -eq 0 ]; then
	echo -e "\033[33m"
	echo "GLORIOUS SUCCESS"
	echo -e "\033[m"
else
	echo -e "\033[31m"
	echo "FATALITY"
	echo -e "\033[m"
fi
