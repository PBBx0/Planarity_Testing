#include <bits/stdc++.h>
#define sz(n) (int)(n).size()
#define dbg(x) cerr << #x << " = " << x << '\n';
using namespace std;
mt19937 rnd(chrono::high_resolution_clock::now().time_since_epoch().count());
uniform_real_distribution<> dist(0, 1);
using ll = int64_t;
using ld = long double;
constexpr ll MAXD = 993;
constexpr int EPOCH = 10000, ITER = 10000;
constexpr ld alpha = 0, PERFECT_LEN = 100;
set<pair<ll, ll>> all_points;
inline ll rand_coord() {
    return (ll) (rnd() % MAXD);
}
struct vec {
    ll x, y;
    vec(ll x, ll y) : x(x), y(y) {}
    vec() : x(rand_coord()), y(rand_coord()) {}
    void __rand() {
        x = rand_coord();
        y = rand_coord();
    }
    void rand() {
        all_points.erase({x, y});
        do {
            __rand();
        } while (all_points.contains({x, y}));
        all_points.emplace(x, y);
    }
    vec operator - (const vec & a) const {
        return vec(x - a.x, y - a.y);
    }
    vec operator + (const vec & a) const {
        return vec(x + a.x, y + a.y);
    }
    ll operator ^ (const vec & a) const {
        return x * a.y - y * a.x;
    }
    ll operator * (const vec & a) const {
        return x * a.x + y * a.y;
    }
    constexpr ld len() const {
        return sqrtl(x * x + y * y);
    }
};
inline ll sign(ll x) {
    return (x > 0) - (x < 0);
}
inline bool on_segment(vec & a, vec & b, vec & x) {
    return ((b - a) ^ (x - a)) == 0 && (a - x) * (b - x) <= 0;
}
//check if segments AB and CD have common point and return penalty
inline int penalty(vec & a, vec & b, vec & c, vec & d) {
    if (on_segment(a, b, c) || on_segment(a, b, d) || on_segment(c, d, a) || on_segment(c, d, b)) return 100;
    ll r1 = (c - a) ^ (b - a), r2 = (d - a) ^ (b - a);
    ll r3 = (b - c) ^ (d - c), r4 = (a - c) ^ (d - c);
    if (r1 == 0 && r2 == 0) return 0;
    return sign(r1) * sign(r2) <= 0 && sign(r3) * sign(r4) <= 0;
}
constexpr inline ld len_penalty(const vec & a) {
    return alpha * abs(a.len() - PERFECT_LEN);
}
void solve() {
    int n, m;
    cin >> n >> m;
    pair<int, int> edges[m];
    vector<int> g[n];
    for (auto & [a, b] : edges) {
        cin >> a >> b;
        --a, --b;
        g[a].emplace_back(b);
        g[b].emplace_back(a);
    }
    vec points[n];
    auto get_penalty = [&](int a, int b) {
        ld pen = 0;
        for (auto [c, d] : edges) {
            if (a == c || a == d || b == c || b == d) continue;
            pen += penalty(points[a], points[b], points[c], points[d]);
        }
        return pen;
    };
    for (int epoch = 0; epoch < EPOCH; ++epoch) {
        for (auto & v : points) v.rand();
        ld pen = 0;
        for (int i = 0; i < m; ++i) {
            auto [a, b] = edges[i];
            pen += len_penalty(points[b] - points[a]);
            pen += get_penalty(a, b);
        }
        ld T = 1000;
        for (int iter = 0; iter < ITER; ++iter) {
            T *= 0.9998;
            ld cur_pen = pen;
            int v = rnd() % n;
            for (int to : g[v]) {
                cur_pen -= 2 * len_penalty(points[to] - points[v]);
                cur_pen -= 2 * get_penalty(v, to);
            }
            vec old(points[v]);
            points[v].rand();
            for (int to : g[v]) {
                cur_pen += 2 * len_penalty(points[to] - points[v]);
                cur_pen += 2 * get_penalty(v, to);
            }
            if (dist(rnd) < exp((pen - cur_pen) / T)) {
                pen = cur_pen;
            } else {
                points[v] = old;
            }
            if (iter % 1000 == 0) cerr << pen << '\n';
            if (pen < 1) {
                cerr << "COOL!" << ' ' << pen << '\n';
                for (int i = 0; i < n; ++i) cout << points[i].x << ' ' << points[i].y << '\n';
                return;
            }
        }
    }
}

signed main() {
#ifdef LOCAL
    freopen("../stream.in", "r", stdin);
    freopen("../stream.out", "w", stdout);
#endif
    solve();
    return 0;
}