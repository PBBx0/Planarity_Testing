from subprocess import run
import draw
# run(["g++", "-std=c++20", "-O3", "main.cpp", "-o", "solve"])
# run(["g++", "-std=c++20", "-O3", "create_embedding.cpp", "-o", "embed"])
inp = open("stream.in", "r").read().strip()
r1 = run("./solve", input=inp.encode(), check=True, capture_output=True, timeout=5.0)
output = r1.stdout.decode()
print(output)
if "not" in output:
    exit(0)
r2 = run("./embed", input=inp.encode(), check=True, capture_output=True)
inp = f'{inp}\n{r2.stdout.decode()}'
draw.plot_planar_graph(inp)
draw.save_pic()