## Planarity testing

+ `main.cpp` принимает на вход граф и возвращает результат проверки на планарность.

+ `graph_generator.cpp` принимает на вход число вершин и число рёбер и генерирует связный граф с таким числом вершин и рёбер.

+ `rectangle_grid_generator.cpp` принимает на вход число `n` и возвращает граф, представляющий из себя квадратную сетку размером `n * n`.

+ `draw.py` содержит функции для отрисовки заданного графа по координатам вершин. Для использование необходимо наличие библиотек `matplotlib` и `networkx`.

+ `run.py` берёт входные данные из `stream.in` (для корректной работы они должны задавать граф), проверяет этот граф на планарность с запуская `main.cpp` и, если граф планарен, то рисует его с помощью `draw.py` (Картинка сохраняется в `mygraph.svg`). 

+ `check.py` для каждого теста из `./tests/` проверяет корректность ответа (правильные ответы лежат в `./answers`) и для планарных графов сохраняет отрисовку в `./plots/`.