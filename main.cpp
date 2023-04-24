#include <bits/stdc++.h>
#define sz(n) (int)(n).size()
using namespace std;
const int N = 10000, M = 50000;
/*
 * g[v] is a set containing neighbours of v
 * color[v] is a current state of vertex v
 * height[v] denotes height of vertex tree in palm tree
 * dp[v] is minimum height of vertex that is accessible from subtree of v with no more than one backedge
 * n, m is a number of vertexes and edges respectively
 */
set<int> g[N];
int color[N], height[N], dp[N];
int n, m;
long start_time;
/*
 * This function is used to just output result. nothing interesting
 */
void output_result(bool planar) {
    cout << "The given graph is" << (planar ? " " : " not ") << "planar" << '\n';
    cerr << double(::clock() - start_time) * 1e3 / CLOCKS_PER_SEC << "ms\n";
    exit(0);
}
/*
 * This function is used to recursively (with depth-first-search) paint connectivity component to color c
 * CAUTION: at the moment of call all vertexes in component must have color[v] != c
 */
void paint_component(int v, int c) {
    color[v] = c;
    for (int to : g[v]) {
        if (color[to] != c) paint_component(to, c);
    }
}
/*
 * Code below is used to find all bridges in the connectivity component at linear time
 * it recursively (with depth-first-search) calculates dp[v] and determines if the edge is bridge
 * CAUTION: at the moment of call all vertexes in component must have color[v] == 0
 */
vector<pair<int, int>> bridges;
void find_bridges(int v, int parent) {
    color[v] = 1;
    dp[v] = height[v];
    for (int to : g[v]) if (to != parent) {
        if (color[to] == 0) {
            height[to] = height[v] + 1;
            find_bridges(to, v);
            dp[v] = min(dp[v], dp[to]);
            if (dp[to] > height[v]) bridges.emplace_back(v, to);
        } else {
            dp[v] = min(dp[v], height[to]);
        }
    }
}
/*
 * This function does what it states:
 * it recursively (with depth-first-search) calculates dp[v]
 * CAUTION: at the moment of call all vertexes in component must have color[v] == 0
 */
void calculate_dp(int v) {
    color[v] = 1;
    dp[v] = height[v];
    for (int to : g[v]) {
        if (color[to] == 0) {
            height[to] = height[v] + 1;
            calculate_dp(to);
            dp[v] = min(dp[v], dp[to]);
        } else {
            dp[v] = min(dp[v], height[to]);
        }
    }
}
/*
 * Code below is used to check if two-connected component planar
 *
 * max_color2 is max_color2
 * vtx is vector of all vertexes that belongs to current two-connected component
 * vtx_set is a set that contains all vertexes from vtx
 * g2 is graph, induced by vertexes of component
 * E for each edge (u, v) it contains u ^ v (it is used to easily determine the neighbour)
 * state[edge] is a state of edge. it is:
 *      2 if edge has already been embedded,
 *      1 if it belongs to segment and was watched,
 *      0 if it hasn't been watched yet
 * color2 is like color but for in-component usage
 * placed contains all vertexes that have been already embedded
 */
int max_color2 = 0;
vector<int> vtx, g2[N];
int E[M], state[M], color2[N];
set<int> vtx_set, placed;
/*
 * This function is used to recursively (with depth-first-search) find path from v to targ and save its edges to cur_path
 * return value is true if path is found and false otherwise
 */
bool find_path(int v, int targ, vector<int> & cur_path) {
    color2[v] = max_color2;
    for (int id : g2[v]) if (id != cur_path.back()) {
        int to = v ^ E[id];
        if (to == targ) {
            cur_path.emplace_back(id);
            return true;
        }
        if (!placed.contains(to) && color2[to] != max_color2) {
            cur_path.emplace_back(id);
            if (find_path(to, targ, cur_path)) return true;
            cur_path.pop_back();
        }
    }
    return false;
}
/*
 * This function is used to make stat[e] = c, e in segment edges
 * it also stores all touching vertexes of segment to touching_vtx
 */
void paint_segment(int v, int c, set<int> & touching_vtx) {
    for (int id : g2[v]) if (state[id] != c) {
        state[id] = c;
        int to = v ^ E[id];
        if (!placed.contains(to)) paint_segment(to, c, touching_vtx);
        else touching_vtx.insert(to);
    }
}
/*
 * It is what you think it is
 * struct that describes Faces
 * each Face has vts --- set of its vertexes and tour --- its vertexes in correct order
 */
struct Face {
    set<int> vts;
    vector<int> tour;
    explicit Face(vector<int> & a) : tour(a) {
        for (int el : tour) vts.insert(el);
    }
    Face() = default;
};
/*
 * And this struct describes Segments
 * each Segment has a start_vertex and a start_edge --- v_start and id_start respectively
 * good_faces is a list of all faces that fit to him
 * touch is a set of its touching vertexes
 */
struct Segment {
    set<int> touch;
    vector<int> good_faces;
    int v_start, id_start;
    Segment(int v, int id) : v_start(v), id_start(id) {}
};
/*
 * This is most important (and the longest) function
 * it checks if current two-connected component (its vertexes should be colored with c) is planar
 */
bool check(int c) {
    /* Initializing */
    vtx.clear();
    vtx_set.clear();
    placed.clear();
    int top = 0;
    for (auto & ar : g2) ar.clear();
    for (int v = 0; v < n; ++v) {
        if (color[v] == c) {
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
    { /* For planar two-connected graph m <= 3n - 6. Checking this. */
        int edges_cnt = 0;
        for (int v: vtx) edges_cnt += sz(g2[v]);
        int n_cur = sz(vtx);
        edges_cnt /= 2;
        if (edges_cnt + 1 == n_cur) return true;
        if (edges_cnt > 3 * n_cur - 6) return false;
    }
    vector<Face> faces;
    { /* Adding primary cycle and faces */
        vector<int> cur_path, cycle;
        int v = vtx[0];
        assert(!g2[v].empty());
        cur_path.emplace_back(g2[v][0]);
        max_color2++;
        assert(find_path(v ^ E[g2[v][0]], v, cur_path));
        for (int id : cur_path) {
            placed.insert(v);
            cycle.emplace_back(v);
            v ^= E[id];
            state[id] = 2;
        }
        faces.emplace_back(cycle);
        faces.emplace_back(cycle);
    }
    vector<Segment> segments;
    { /* Computing segments */
        for (int v : placed) {
            for (int id : g2[v]) if (state[id] == 0) {
                segments.emplace_back(v, id);
                int to = v ^ E[id];
                if (placed.contains(to)) {
                    segments.back().touch = {v, to};
                    state[id] = 1;
                } else {
                    paint_segment(v ^ E[id], 1, segments.back().touch);
                }
            }
        }
    }
    while (!segments.empty()) {
        /* COMMON STEP */
        int id = 0;
        int s = sz(segments);
        /* For each segment s we compute Gamma(s) (segments[i].good_faces) */
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
            /* we want to find id that sz(segments[id].good_faces) is minimal */
            if (sz(segments[i].good_faces) < sz(segments[id].good_faces)) id = i;
        }
        /* if |Gamma(s)| == 0 our graph is not planar */
        if (segments[id].good_faces.empty()) return false;
        swap(segments[id], segments[s - 1]);
        auto & [touch, good_faces, v_start, id_start] = segments.back();
        int targ = *touch.begin();
        if (targ == v_start) targ = *touch.rbegin();
        /* now we want to add path v_start -> targ to our embedding */
        if (!placed.contains(v_start ^ E[id_start])) paint_segment(v_start ^ E[id_start], 0, touch);
        vector<int> cur_path{id_start};
        max_color2++;
        /* if our path is not a single edge we call find_path */
        if (!placed.contains(v_start ^ E[id_start])) assert(find_path(v_start ^ E[id_start], targ, cur_path));
        /* we list all vertexes of path to path_vtx */
        int cur_v = v_start;
        vector<int> path_vtx{cur_v};
        for (int x : cur_path) {
            state[x] = 2;
            cur_v = cur_v ^ E[x];
            path_vtx.emplace_back(cur_v);
            placed.insert(cur_v);
        }
        /* this code splits Face into two by our path */
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
        /* remove old face and segment, add new faces */
        faces.pop_back();
        faces.emplace_back(fst);
        faces.emplace_back(snd);
        segments.pop_back();
        /* compute new segments */
        for (int v : path_vtx) {
            for (int x : g2[v]) if (state[x] == 0) {
                segments.emplace_back(v, x);
                int to = v ^ E[x];
                if (!placed.contains(to)) paint_segment(to, 1, segments.back().touch);
                else segments.back().touch = {v, to};
            }
        }
    }
    return true;
}
/*
 * this functions recursively (with depth-first-search) paints two-connected components and calls check()
 */
int max_color = 1;
void dfs(int v, int c) {
    color[v] = c;
    for (int to : g[v]) if (color[to] == 0) {
        if (dp[to] >= height[v]) {
            int new_color = max_color++;
            dfs(to, new_color);
            color[v] = new_color;
            if (!check(new_color)) output_result(false);
            color[v] = c;
        } else {
            dfs(to, c);
        }
    }
}


void solve() {
    cin >> n >> m;
    fill(color, color + n, 0);
    for (int i = 0; i < m; ++i) {
        int a, b;
        cin >> a >> b;
        a--, b--;
        if (a == b) continue;
        g[a].insert(b);
        g[b].insert(a);
    }
    for (int v = 0; v < n; ++v) if (color[v] == 0) find_bridges(v, v);
    for (auto [u, v] : bridges) {
        g[u].erase(v);
        g[v].erase(u);
    }
    fill(color, color + n, 0);
    for (int v = 0; v < n; ++v) if (color[v] == 0) calculate_dp(v);
    fill(color, color + n, 0);
    for (int v = 0; v < n; ++v) if (color[v] == 0) {
        int c = max_color++;
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