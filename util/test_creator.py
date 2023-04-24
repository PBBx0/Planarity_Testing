from subprocess import run
run(["g++", "-std=c++20", "./rectangle_grid_generator.cpp", "-o", "gen"])
for id in range(1, 17):
    inp = f'{id}'
    r = run("../gen", check=True, capture_output=True, timeout=1.0, input=inp.encode())
    graph = r.stdout.decode()
    with open(f"../tests/test{14 + id}.txt", "w") as f:
        f.write(graph)
    open(f"../answers/test{id + 14}.txt", "w")