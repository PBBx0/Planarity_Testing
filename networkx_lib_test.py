import networkx as nx
import matplotlib.pyplot as plt

# Define the nodes and edges of the graph
n, m = map(int, input().split())
nodes, edges = [], []
for i in range(n):
    nodes.append(i + 1)
for i in range(m):
    a, b = map(int, input().split())
    edges.append((a, b))

# Create the graph object and add the nodes and edges
G = nx.Graph()
G.add_nodes_from(nodes)
G.add_edges_from(edges)

# Check if the graph is planar
is_planar, embedding = nx.check_planarity(G)

# If the graph is planar, draw it
if is_planar:
    print("This graph is planar.")
    pos = nx.combinatorial_embedding_to_pos(embedding)
    nx.draw(G, pos=pos, with_labels=True, node_color='red')
    plt.savefig("mygraph.svg")
else:
    print("This graph is not planar.")