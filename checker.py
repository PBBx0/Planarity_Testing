from subprocess import run
import os, draw

tests = os.listdir("./tests")
for test in tests:
    inp = open(f'./tests/{test}', 'r').read()
    r1 = run("cmake-build-debug/solve", input=inp.encode(), check=True, capture_output=True, timeout=5.0)
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
    r2 = run("cmake-build-debug/embed", input=inp.encode(), check=True, capture_output=True)
    inp = f'{inp}\n{r2.stdout.decode()}'
    draw.plot_planar_graph(inp)
    filename = test.split('.')[0]
    draw.save_pic(f'./plots/{filename}.svg')