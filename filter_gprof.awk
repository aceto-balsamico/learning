# filter_gprof.awk

# 表示する最小のtimeの割合（例: 1%）
min_time_percentage=1

# 表示する関数の名前（例: main, func1, func2）
target_functions="main 
func1 
func2"

# BEGIN ブロックはファイルの最初に一度だけ実行
BEGIN {
    split(target_functions, target_function_array)
    in_target_function = 0
}

# 関数ごとの行を処理
/^[[:space:]]*[0-9]+[[:space:]]+[0-9]+[[:space:]]+[0-9]+\.[0-9]+[[:space:]]+[0-9]+\.[0-9]+[[:space:]]+/ {
    # timeの割合を取得
    time_percentage = $6

    # 関数名を取得
    function_name = $7

    # 表示する条件を満たすか判定
    if (time_percentage >= min_time_percentage || is_target_function(function_name)) {
        in_target_function = 1
    } else {
        in_target_function = 0
    }
}

# 表示する条件を満たす関数を出力
in_target_function == 1 {
    if (FNR == 1) {
        print "Elapsed Time:", elapsed_time
    }
    print $0
}

# 関数がtarget_functionsに含まれるか確認する関数
function is_target_function(name) {
    for (i in target_function_array) {
        if (name == target_function_array[i]) {
            return 1
        }
    }
    return 0
}
