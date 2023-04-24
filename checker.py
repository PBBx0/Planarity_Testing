import math
from subprocess import run
import os, draw
import matplotlib.pyplot as plt
import numpy

run(["g++", "-std=c++20", "-O3", "main.cpp", "-o", "solve"])
tests = os.listdir("./tests")
x, y = [], []
for test in tests:
    inp = open(f'./tests/{test}', 'r').read()
    r1 = run("./solve", input=inp.encode(), check=True, capture_output=True, timeout=5.0)
    output = r1.stdout.decode()
    old_output = open(f'./answers/{test}', 'r').read()
    if old_output == "":
        with open(f'./answers/{test}', 'w') as f:
            f.write(output)
    elif output != old_output:
        print(f'Wrong answer on {test}')
        break
    if "not" in output:
        continue
    x.append(int(inp.split()[0]))
    y.append(float(r1.stderr.decode()))
    draw.plot_planar_graph(inp)
    filename = test.split('.')[0]
    draw.save_pic(f'./plots/{filename}.svg')
plt.clf()
plt.xlabel("Количество вершин в графе")
plt.ylabel("Время работы в миллисекундах")
plt.plot(x, y)
plt.savefig("time effiquency.png")