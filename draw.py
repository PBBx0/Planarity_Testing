import matplotlib.pyplot as plt


def parse_input(inp):
    lst = list(map(int, inp.split()))
    n, m = lst[0], lst[1]
    edges, x, y = [], [], []
    for i in range(2, 2 + m + m, 2):
        edges.append((lst[i], lst[i + 1]))
    for i in range(m + m + 2, len(lst), 2):
        x.append(lst[i])
        y.append(lst[i + 1])
    return n, m, edges, x, y


def plot_planar_graph(inp):
    n, m, edges, x, y = parse_input(inp)
    plt.clf()
    plt.axis('off')
    plt.scatter(x, y, color='red')
    for a, b in edges:
        plt.plot([x[a - 1], x[b - 1]], [y[a - 1], y[b - 1]], color='black')
    for i in range(n):
        plt.annotate(f'{i + 1}', (x[i], y[i]), (x[i] + 10, y[i] + 10), color='blue')


def save_pic(filename="mygraph.svg"):
    plt.savefig(filename)