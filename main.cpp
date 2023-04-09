#include <bits/stdc++.h>
#define sz(n) (int)(n).size()
using namespace std;
const int N = 10000, M = 50000;
set<int> g[N];
int col[N], h[N], dp[N];
int n, m;
long start_time;
void output_result(bool planar) {
    cout << "The given graph is" << (planar ? " " : " not ") << "planar" << '\n';
    cerr << double(::clock() - start_time) * 1e3 / CLOCKS_PER_SEC << "ms\n";
    exit(0);
}
void paint(int v, int c) {
    col[v] = c;
    for (int to : g[v]) {
        if (col[to] != c) paint(to, c);
    }
}
vector<pair<int, int>> bridges;
void find_bridges(int v, int par) {
    col[v] = 1;
    dp[v] = h[v];
    for (int to : g[v]) if (to != par) {
        if (col[to] == 0) {
            h[to] = h[v] + 1;
            find_bridges(to, v);
            dp[v] = min(dp[v], dp[to]);
            if (dp[to] > h[v]) bridges.emplace_back(v, to);
        } else {
            dp[v] = min(dp[v], h[to]);
        }
    }
}
void calc_dp(int v) {
    col[v] = 1;
    dp[v] = h[v];
    for (int to : g[v]) {
        if (col[to] == 0) {
            h[to] = h[v] + 1;
            calc_dp(to);
            dp[v] = min(dp[v], dp[to]);
        } else {
            dp[v] = min(dp[v], h[to]);
        }
    }
}
int color2 = 0;
vector<int> vtx, g2[N];
int E[M], stat[M], col2[N];
set<int> vtx_set, placed;
bool find_path(int v, int targ, vector<int> & cur_path) {
    col2[v] = color2;
    for (int id : g2[v]) if (id != cur_path.back()) {
        int to = v ^ E[id];
        if (to == targ) {
            cur_path.emplace_back(id);
            return true;
        }
        if (!placed.contains(to) && col2[to] != color2) {
            cur_path.emplace_back(id);
            if (find_path(to, targ, cur_path)) return true;
            cur_path.pop_back();
        }
    }
    return false;
}
void paint_segment(int v, int c, set<int> & touching_vtx) {
    for (int id : g2[v]) if (stat[id] != c) {
        stat[id] = c;
        int to = v ^ E[id];
        if (!placed.contains(to)) paint_segment(to, c, touching_vtx);
        else touching_vtx.insert(to);
    }
}
struct Face {
    set<int> vts;
    vector<int> tour;
    explicit Face(vector<int> & a) : tour(a) {
        for (int el : tour) vts.insert(el);
    }
    Face() = default;
};
struct Segment {
    set<int> touch;
    vector<int> good_faces;
    int v_start, id_start;
    Segment(int v, int id) : v_start(v), id_start(id) {}
};
bool check(int c) {
    vtx.clear();
    vtx_set.clear();
    placed.clear();
    int top = 0;
    for (auto & ar : g2) ar.clear();
    for (int v = 0; v < n; ++v) {
        if (col[v] == c) {
            vtx.emplace_back(v);
            vtx_set.insert(v);
        }
    }
    for (int v : vtx) for (int to : g[v]) if (vtx_set.contains(to)) {
        if (v < to) {
            g2[v].emplace_back(top);
            g2[to].emplace_back(top);
            E[top] = v ^ to;
            top++;
        }
    }
    int edges_cnt = 0;
    for (int v : vtx) edges_cnt += sz(g2[v]);
    int n_cur = sz(vtx);
    edges_cnt /= 2;
    if (edges_cnt + 1 == n_cur) return true;
    if (edges_cnt > 3 * n_cur - 6) return false;
    vector<Face> faces;
    { /* ADDING PRIMARY CYCLE AND FACES */
        vector<int> cur_path, cycle;
        int v = vtx[0];
        assert(!g2[v].empty());
        cur_path.emplace_back(g2[v][0]);
        color2++;
        assert(find_path(v ^ E[g2[v][0]], v, cur_path));
        for (int id : cur_path) {
            placed.insert(v);
            cycle.emplace_back(v);
            v ^= E[id];
            stat[id] = 2;
        }
        faces.emplace_back(cycle);
        faces.emplace_back(cycle);
    }
    vector<Segment> segments;
    { /* COMPUTING SEGMENTS */
        for (int v : placed) {
            for (int id : g2[v]) if (stat[id] == 0) {
                segments.emplace_back(v, id);
                int to = v ^ E[id];
                if (placed.contains(to)) {
                    segments.back().touch = {v, to};
                    stat[id] = 1;
                } else {
                    paint_segment(v ^ E[id], 1, segments.back().touch);
                }
            }
        }
    }
    while (!segments.empty()) {
        int id = 0;
        int s = sz(segments);
        for (int i = 0; i < s; ++i) {
            segments[i].good_faces.clear();
            for (int j = 0; j < sz(faces); ++j) {
                bool ok = true;
                for (int x : segments[i].touch) {
                    if (!faces[j].vts.contains(x)) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    segments[i].good_faces.emplace_back(j);
                }
            }
            if (sz(segments[i].good_faces) < sz(segments[id].good_faces)) id = i;
        }
        if (segments[id].good_faces.empty()) return false;
        swap(segments[id], segments[s - 1]);
        auto & [touch, good_faces, v_start, id_start] = segments.back();
        int targ = *touch.begin();
        if (targ == v_start) targ = *touch.rbegin();
        if (!placed.contains(v_start ^ E[id_start])) paint_segment(v_start ^ E[id_start], 0, touch);
        vector<int> cur_path{id_start};
        color2++;
        if (!placed.contains(v_start ^ E[id_start])) assert(find_path(v_start ^ E[id_start], targ, cur_path));
        int v = v_start;
        vector<int> path_vtx{v};
        for (int x : cur_path) {
            stat[x] = 2;
            v = v ^ E[x];
            path_vtx.emplace_back(v);
            placed.insert(v);
        }
        vector<int> fst, snd;
        auto & f = faces[good_faces[0]];
        int pos = 0;
        while (f.tour[pos] != v_start) ++pos;
        for (pos = (pos + 1) % sz(f.tour); f.tour[pos] != targ; pos = (pos + 1) % sz(f.tour)) {
            snd.emplace_back(f.tour[pos]);
        }
        for (pos = (pos + 1) % sz(f.tour); f.tour[pos] != v_start; pos = (pos + 1) % sz(f.tour)) {
            fst.emplace_back(f.tour[pos]);
        }
        for (int v : path_vtx) fst.emplace_back(v);
        std::reverse(path_vtx.begin(), path_vtx.end());
        for (int v : path_vtx) snd.emplace_back(v);
        swap(faces[good_faces[0]], faces.back());
        faces.pop_back();
        faces.emplace_back(fst);
        faces.emplace_back(snd);
        segments.pop_back();
        for (int v : path_vtx) {
            for (int x : g2[v]) if (stat[x] == 0) {
                segments.emplace_back(v, x);
                int to = v ^ E[x];
                if (!placed.contains(to)) paint_segment(to, 1, segments.back().touch);
                else segments.back().touch = {v, to};
            }
        }
    }
    return true;
}

int maxc = 1;
void dfs(int v, int c) {
    col[v] = c;
    for (int to : g[v]) if (col[to] == 0) {
        if (dp[to] >= h[v]) {
            int new_col = maxc++;
            dfs(to, new_col);
            col[v] = new_col;
            if (!check(new_col)) output_result(false);
            col[v] = c;
        } else {
            dfs(to, c);
        }
    }
}


void solve() {
    cin >> n >> m;
    fill(col, col + n, 0);
    for (int i = 0; i < m; ++i) {
        int a, b;
        cin >> a >> b;
        a--, b--;
        if (a == b) continue;
        g[a].insert(b);
        g[b].insert(a);
    }
    int edges = 0;
    for (int v = 0; v < n; ++v) edges += sz(g[v]);
    edges /= 2;
    if (edges > 3 * n) {
        output_result(false);
    }
    for (int v = 0; v < n; ++v) if (col[v] == 0) find_bridges(v, v);
    for (auto [u, v] : bridges) {
        g[u].erase(v);
        g[v].erase(u);
    }
    fill(col, col + n, 0);
    for (int v = 0; v < n; ++v) if (col[v] == 0) calc_dp(v);
    fill(col, col + n, 0);
    for (int v = 0; v < n; ++v) if (col[v] == 0) {
        int c = maxc++;
        dfs(v, c);
        if (!check(c)) output_result(false);
    }
    output_result(true);
}

signed main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    cin.exceptions(cin.failbit);
#ifdef LOCAL
    freopen("../stream.in", "r", stdin);
    freopen("../stream.out", "w", stdout);
#endif
    start_time = ::clock();
    solve();
    return 0;
}