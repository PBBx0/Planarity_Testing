import matplotlib.pyplot as plt
import networkx as nx


def parse_input(inp):
    lst = list(map(int, inp.split()))
    n, m = lst[0], lst[1]
    nodes, edges = [], []
    for i in range(1, n + 1):
        nodes.append(i)
    for i in range(2, 2 + m + m, 2):
        edges.append((lst[i], lst[i + 1]))
    return n, m, nodes, edges


def plot_planar_graph(inp):
    plt.clf()
    n, m, nodes, edges = parse_input(inp)
    G = nx.Graph()
    G.add_nodes_from(nodes)
    G.add_edges_from(edges)
    # Get embedding
    is_planar, embedding = nx.check_planarity(G)
    assert is_planar
    pos = nx.combinatorial_embedding_to_pos(embedding)
    nx.draw(G, pos=pos, with_labels=True, node_color='red')


def save_pic(filename="mygraph.svg"):
    plt.savefig(filename)