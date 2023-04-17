#include <bits/stdc++.h>
#define sz(n) (int)(n).size()
#define dbg(x) cerr << #x << " = " << x << '\n';
using namespace std;
mt19937 rnd(chrono::high_resolution_clock::now().time_since_epoch().count());
uniform_real_distribution<> dist(0, 1);
using ll = int64_t;
using ld = long double;
constexpr ll MAXD = 1e4;
constexpr int EPOCH = 10000, ITER = 10000;
constexpr ld ALPHA = 1e-3, BETA = 1e-2, GAMMA = 1e3;
constexpr ld PERFECT_LEN = 1000, PERFECT_EDGE_TO_NODE_DISTANCE = 100;
struct vec {
    static ll rand_coord() {
        return (ll) (rnd() % MAXD);
    }
    ll x, y;
    vec(ll x, ll y) : x(x), y(y) {}
    vec() : x(rand_coord()), y(rand_coord()) {}
    void rand() {
        x = rand_coord();
        y = rand_coord();
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
inline bool on_segment(const vec & a, const vec & b, const vec & x) {
    return ((b - a) ^ (x - a)) == 0 && (a - x) * (b - x) <= 0;
}
//check if segments AB and CD have common point and return penalty
inline ld intersection_penalty(const vec & a, const vec & b, const vec & c, const vec & d) {
    if (on_segment(a, b, c) || on_segment(a, b, d) || on_segment(c, d, a) || on_segment(c, d, b)) return GAMMA * 1e2;
    ll r1 = (c - a) ^ (b - a), r2 = (d - a) ^ (b - a);
    ll r3 = (b - c) ^ (d - c), r4 = (a - c) ^ (d - c);
    if (r1 == 0 && r2 == 0) return 0;
    return int(sign(r1) * sign(r2) <= 0 && sign(r3) * sign(r4) <= 0) * GAMMA;
}
inline ld distance_penalty(const vec & x, const vec & a, const vec & b) {
    vec v = b - a, rv = a - b, s = x - a, t = x - b;
    ld dst = min(s.len(), t.len());
    if (v * s >= 0 && rv * t >= 0) dst = abs(s ^ t) / v.len();
    if (dst < 1) return BETA / dst;
    if (dst < PERFECT_EDGE_TO_NODE_DISTANCE) return BETA * (PERFECT_EDGE_TO_NODE_DISTANCE - dst);
    return 0;
}
inline ld len_penalty(const vec & a) {
    return ALPHA * abs(a.len() - PERFECT_LEN);
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
    auto get_intersection_penalty = [&](int a, int b) {
        ld pen = 0;
        for (auto [c, d] : edges) {
            if (a == c || a == d || b == c || b == d) continue;
            pen += intersection_penalty(points[a], points[b], points[c], points[d]);
        }
        return pen;
    };
    auto get_node_distance_penalty = [&](int a) {
        ld pen = 0;
        for (auto [c, d] : edges) {
            if (a == c || a == d) continue;
            pen += distance_penalty(points[a], points[c], points[d]);
        }
        return pen;
    };
    auto get_edge_distance_penalty = [&](int a, int b) {
        ld pen = 0;
        for (int v = 0; v < n; ++v) {
            if (a == v || b == v) continue;
            pen += distance_penalty(points[v], points[a], points[b]);
        }
        return pen;
    };
    for (int epoch = 0; epoch < EPOCH; ++epoch) {
        for (auto & v : points) v.rand();
        ld pen = 0;
        for (int i = 0; i < m; ++i) {
            auto [a, b] = edges[i];
            pen += len_penalty(points[b] - points[a]);
            pen += get_intersection_penalty(a, b);
            pen += get_edge_distance_penalty(a, b);
        }
        ld T = 1000;
        for (int iter = 0; iter < ITER; ++iter) {
            T *= 0.9998;
            ld cur_pen = pen;
            int v = rnd() % n;
            for (int to : g[v]) {
                cur_pen -= 2 * len_penalty(points[to] - points[v]);
                cur_pen -= 2 * get_intersection_penalty(v, to);
                cur_pen -= get_edge_distance_penalty(v, to);
            }
            cur_pen -= get_node_distance_penalty(v);
            vec old(points[v]);
            points[v].rand();
            for (int to : g[v]) {
                cur_pen += 2 * len_penalty(points[to] - points[v]);
                cur_pen += 2 * get_intersection_penalty(v, to);
                cur_pen -= get_edge_distance_penalty(v, to);
            }
            cur_pen += get_node_distance_penalty(v);
            if (dist(rnd) < exp((pen - cur_pen) / T)) {
                pen = cur_pen;
            } else {
                points[v] = old;
            }
        }
        if (pen < GAMMA) {
            cerr << "COOL!" << ' ' << pen << '\n';
            for (int i = 0; i < n; ++i) cout << points[i].x << ' ' << points[i].y << '\n';
            return;
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