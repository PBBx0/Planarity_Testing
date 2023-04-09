#include <bits/stdc++.h>
using namespace std;
mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
const int N = 1e5;
int dsu[N];
int find(int v) {
    return dsu[v] < 0 ? v : dsu[v] = find(dsu[v]);
}
bool unite(int a, int b) {
    a = find(a), b = find(b);
    if (a == b) return false;
    if (dsu[a] > dsu[b]) swap(a, b);
    dsu[a] += dsu[b];
    dsu[b] = a;
    return true;
}
signed main() {
#ifdef LOCAL
    ::freopen("../stream.in", "w", stdout);
#endif
    int n, m;
    cin >> n >> m;
    cout << n << ' ' << m << '\n';
    assert(m >= n - 1);
    fill(dsu, dsu + n, -1);
    for (int c = n; c >  1;) {
        int a = rnd() % n;
        int b = rnd() % n;
        if (unite(a, b)) {
            c--;
            cout << a + 1 << ' ' << b + 1 << '\n';
        }
    }
    for (int i = n - 1; i < m; ++i) {
        int a = rnd() % n;
        int b = rnd() % n;
        cout << a + 1 << ' ' << b + 1 << '\n';
    }
    return 0;
}