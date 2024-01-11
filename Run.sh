#!/bin/bash

# 開始時刻を記録
start_time=$(date +%s)

# 実行ファイルのパス
EXECUTABLE_PATH="./bin/executable"

# 実行ファイルの実行
$EXECUTABLE_PATH

# 終了時刻を記録
end_time=$(date +%s)

# 実行時間を計算
elapsed_time=$((end_time - start_time))
echo "Elapsed time: $elapsed_time seconds"

# gmon.out を txt ファイルに変換
gprof $EXECUTABLE_PATH > gmon_out.txt

# gmon_out.txt を編集して表示する関数や最小の時間割合に基づいてフィルタリング
awk -v elapsed_time=$elapsed_time -f filter_gprof.awk gmon_out.txt > filtered_gmon_out.txt

# 編集結果を表示
cat filtered_gmon_out.txt

# 不要なファイルを削除
rm gmon.out gmon_out.txt
