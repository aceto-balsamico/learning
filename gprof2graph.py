import matplotlib.pyplot as plt

def parse_gprof(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()

    functions = []
    for line in lines:
        if line.startswith(" "):
            parts = line.split()
            if len(parts) >= 6 and parts[0].isdigit():
                func_info = {
                    'name': parts[5],
                    'time': float(parts[2]),
                    'calls': int(parts[3])
                }
                functions.append(func_info)
    return functions

def plot_graphs(functions):
    # Sort functions by total time in descending order
    functions.sort(key=lambda x: x['time'], reverse=True)

    # Extracting data
    names = [func['name'] for func in functions]
    times = [func['time'] for func in functions]
    calls = [func['calls'] for func in functions]

    # Creating subplots
    fig, ax1 = plt.subplots()

    # Plotting times
    color = 'tab:red'
    ax1.set_xlabel('Functions')
    ax1.set_ylabel('Time (%)', color=color)
    ax1.bar(names, times, color=color, alpha=0.6, label='Time (%)')
    ax1.tick_params(axis='y', labelcolor=color)
    ax1.tick_params(axis='x', rotation=90)

    # Create a secondary y-axis for call counts
    ax2 = ax1.twinx()
    color = 'tab:blue'
    ax2.set_ylabel('Calls', color=color)
    ax2.plot(names, calls, color=color, marker='o', linestyle='dashed', linewidth=2, markersize=5, label='Calls')
    ax2.tick_params(axis='y', labelcolor=color)

    # Adding legends
    ax1.legend(loc='upper left')
    ax2.legend(loc='upper right')

    # Adjust layout
    fig.tight_layout()
    plt.show()

if __name__ == "__main__":
    functions = parse_gprof('gprof.txt')
    plot_graphs(functions)
