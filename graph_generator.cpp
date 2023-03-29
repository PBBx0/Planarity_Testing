#include <bits/stdc++.h>
using namespace std;
mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
signed main() {
#ifdef LOCAL
    ::freopen("../stream.in", "w", stdout);
#endif
    int n, m;
    cin >> n >> m;
    cout << n << ' ' << m << '\n';
    for (int i = 0; i < m; ++i) {
        cout << rnd() % n + 1 << ' ' << rnd() % n + 1 << '\n';
    }
    return 0;
}