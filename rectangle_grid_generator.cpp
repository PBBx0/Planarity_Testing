#include <bits/stdc++.h>
using namespace std;
signed main() {
#ifdef LOCAL
    ::freopen("../stream.in", "w", stdout);
#endif
    int n;
    cin >> n;
    cout << (n + 1) * (n + 1) - 1 << ' ' << n * n * 2 << '\n';
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << i * (n + 1) + j + 1 << ' ';
            cout << (i + 1) * (n + 1) + j + 1 << '\n';
            cout << i * (n + 1) + j + 1 << ' ';
            cout << i * (n + 1) + j + 2 << '\n';
        }
    }
    return 0;
}