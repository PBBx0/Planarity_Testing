import pyglet
import pyglet.shapes as shapes
width, height = 1000, 1000
window = pyglet.window.Window(width, height)
background = pyglet.image.SolidColorImagePattern((255, 255, 255, 255)).create_image(width,height)
batch = pyglet.graphics.Batch()
BLACK = (0, 0, 0)
line_width, node_size = 2, 5
n, m = map(int, input().split())
edges = []
for i in range(m):
    a, b = map(int, input().split())
    edges.append((a, b))
coords = []
for i in range(n):
    x, y = map(int, input().split())
    coords.append((x, y))
nodes, lines = [], []
for i in range(n):
    x, y = coords[i]
    nodes.append(shapes.Circle(x, y, node_size, color=BLACK, batch=batch))
for i in range(m):
    a, b = edges[i]
    x1, y1 = coords[a - 1]
    x2, y2 = coords[b - 1]
    lines.append(shapes.Line(x1, y1, x2, y2, width=line_width, color=BLACK, batch=batch))
@window.event
def on_draw():
    window.clear()
    background.blit(0, 0)
    batch.draw()

pyglet.app.run()