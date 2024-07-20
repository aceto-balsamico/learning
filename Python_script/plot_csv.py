import pandas as pd
import plotly.graph_objects as go
from io import StringIO
import numpy as np
import tkinter as tk
from tkinter import filedialog

def generate_graph(csv_file_path):
    # X軸、Y軸の名前、単位を設定
    x_axis_name = 'Voltage'
    y_axis_name = 'Count'
    x_axis_unit = 'V'
    y_axis_unit = 'Δ'
    x_axis_tick_interval = 0.5

    # CSVファイルを読み込み
    with open(csv_file_path, 'r') as file:
        lines = file.readlines()

    # 空行でデータブロックを分ける
    data_blocks = []
    current_block = []

    for line in lines:
        if line.strip() == "":
            if current_block:
                data_blocks.append(current_block)
                current_block = []
        else:
            current_block.append(line)

    if current_block:
        data_blocks.append(current_block)

    # 各データブロックごとにグラフを作成
    for block in data_blocks:
        # タイトル部分を抽出
        title_lines = [line for line in block if line.startswith('####')]
        title = ''.join(title_lines).replace('####', '').strip()

        # "Voltage"が含まれる行を見つける
        data_start_index = next(i for i, line in enumerate(block) if 'Voltage' in line)

        # データ部分をデータフレームとして読み込む
        data_str = ''.join(block[data_start_index:])
        df = pd.read_csv(StringIO(data_str))

        # 1列目をX軸として設定し、数値以外のデータを含む行を除外
        df = df[pd.to_numeric(df[df.columns[0]], errors='coerce').notna()]
        df = df.dropna(axis=1, how='all')  # データがない列を削除

        if df.empty:
            continue

        # データフレームの変形
        df_melted = pd.melt(df, id_vars=[df.columns[0]], var_name='Data Name', value_name='Value')

        # 色のグラデーション用設定
        def get_color_scale(num_colors):
            return [(i / (num_colors - 1), 0, 1 - (i / (num_colors - 1))) for i in range(num_colors)]

        # グラフの作成
        fig = go.Figure()

        # 色のグラデーション設定
        num_colors = len(df.columns) - 1
        color_scale = get_color_scale(num_colors)

        for idx, name in enumerate(df.columns[1:]):
            df_filtered = df_melted[df_melted['Data Name'] == name]

            # 散布図としてプロット
            fig.add_trace(go.Scatter(x=df_filtered[df.columns[0]], y=df_filtered['Value'],
                                     mode='markers+lines',
                                     name=name,
                                     line=dict(color=f'rgb({int(color_scale[idx][0]*255)}, {int(color_scale[idx][1]*255)}, {int(color_scale[idx][2]*255)})'),
                                     marker=dict(color=f'rgb({int(color_scale[idx][0]*255)}, {int(color_scale[idx][1]*255)}, {int(color_scale[idx][2]*255)})')))

        # X軸の目盛りを設定
        x_min = df[df.columns[0]].min()
        x_max = df[df.columns[0]].max()

        # X軸の最小値と最大値を調整（0.5単位の倍数にする）
        x_min = np.floor(x_min / x_axis_tick_interval) * x_axis_tick_interval
        x_max = np.ceil(x_max / x_axis_tick_interval) * x_axis_tick_interval

        # 小数点を含む範囲で目盛りを生成
        def generate_ticks(start, end, interval):
            start = float(start)
            end = float(end)
            interval = float(interval)
            ticks = []
            while start <= end:
                ticks.append(round(start, 1))  # 小数点以下1桁でラウンド
                start += interval
            return ticks

        x_ticks = generate_ticks(x_min, x_max, x_axis_tick_interval)

        fig.update_xaxes(tickmode='array', tickvals=x_ticks, ticktext=[f'{tick:.1f}' for tick in x_ticks],
                         title_text=f'{x_axis_name} ({x_axis_unit})')

        # Y軸を対数スケールに設定し、目盛りを10のべき乗で表示
        y_min = df_melted['Value'].min()
        y_max = df_melted['Value'].max()

        # Y軸の目盛りを10のべき乗に設定
        def generate_log_ticks(start, end):
            ticks = []
            power = np.floor(np.log10(start)) if start > 0 else 0
            while 10**power <= end:
                # 10のべき乗の目盛りとその倍数も含める（例: 0.1, 1, 10）
                ticks.extend([10**power])
                # ticks.extend([10**power, 2*10**power, 5*10**power])
                power += 1
            ticks = sorted(set(ticks))  # 重複を削除し、ソート
            return [round(tick, 2) for tick in ticks]

        if y_min > 0:
            y_ticks = generate_log_ticks(y_min, y_max)
        else:
            y_ticks = generate_log_ticks(1, y_max)

        fig.update_yaxes(type='log', title_text=f'{y_axis_name} ({y_axis_unit})',
                         tickmode='array', tickvals=y_ticks, ticktext=[f'{tick:.2f}' for tick in y_ticks])

        # グラフのレイアウト設定
        fig.update_layout(title=title,
                          xaxis_title=f'{x_axis_name} ({x_axis_unit})',
                          yaxis_title=f'{y_axis_name} ({y_axis_unit})',
                          showlegend=True)

        # グラフを表示
        fig.show()

def open_file_dialog():
    file_path = filedialog.askopenfilename(filetypes=[("CSV files", "*.csv")])
    if file_path:
        generate_graph(file_path)

# Tkinterアプリのセットアップ
root = tk.Tk()
root.title("CSVグラフ生成ツール")

frame = tk.Frame(root, padx=20, pady=20)
frame.pack(padx=10, pady=10)

label = tk.Label(frame, text="CSVファイルを選択してください:")
label.pack()

button = tk.Button(frame, text="ファイルを選択", command=open_file_dialog)
button.pack(pady=10)

root.mainloop()