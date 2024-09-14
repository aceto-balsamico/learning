#!/bin/bash

# コマンドライン引数をチェックして、適切な処理を実行する
if [ "$1" == "comp" ]; then
	# make comp を実行
	make comp -f makefile.mk
elif [ "$1" == "link" ]; then
	# make link を実行
	make link -f makefile.mk
else
	# デフォルトの場合はすべてのコマンドを順に実行
	make comp -f makefile.mk
	./create_include.sh
	make link -f makefile.mk
fi

echo -e "\033[33m"
echo "GLORIOUS SUCCESS"
echo -e "\033[m"

