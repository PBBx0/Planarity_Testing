#include <bits/stdc++.h>
using namespace std;
mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
signed main() {
#ifdef LOCAL
    ::freopen("../stream.in", "w", stdout);
#endif
    int n;
    cin >> n;
    int nv = (n + 1) * (n + 1) - 1;
    int m = n * n * 2;
    cout << nv << ' ' << m << '\n';
    pair<int, int> edges[m];
    int top = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            edges[top++] = {i * (n + 1) + j, (i + 1) * (n + 1) + j};
            edges[top++] = {i * (n + 1) + j, i * (n + 1) + j + 1};
        }
    }
    int vtx[nv];
    iota(vtx, vtx + nv, 1);
    shuffle(vtx, vtx + nv, rnd);
    shuffle(edges, edges + m, rnd);
    for (auto [a, b] : edges) cout << vtx[a] << ' ' << vtx[b] << '\n';
    return 0;
}