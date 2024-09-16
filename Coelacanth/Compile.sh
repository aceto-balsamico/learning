#!/bin/bash

# コマンドライン引数をチェックして、適切な処理を実行する
return_value=1
if [ "$1" == "comp" ]; then
	# make comp を実行
	make comp -f makefile.mk
	return_value=$?
elif [ "$1" == "link" ]; then
	# make link を実行
	make link -f makefile.mk
	return_value=$?
elif [ "$1" == "clean" ]; then
	# make link を実行
	make clean -f makefile.mk
	return_value=$?
else
	# デフォルトの場合はすべてのコマンドを順に実行
	make comp -f makefile.mk
	return_value=$?
	if [ $return_value -eq 0 ]; then
		./create_include.sh
		make link -f makefile.mk
		return_value=$?
	fi
fi
#echo "return_value = $return_value"
if [ $return_value -eq 0 ]; then
	echo -e "\033[33m"
	echo "GLORIOUS SUCCESS"
	echo -e "\033[m"
else
	echo -e "\033[31m"
	echo "FATALITY"
	echo -e "\033[m"
fi

