#include <bits/stdc++.h>

namespace kactl {

using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

struct Dinic {
	struct Edge {
		int to, rev;
		ll c, oc;
		ll flow() { return max(oc - c, 0LL); } // if you need flows
	};
	vi lvl, ptr, q;
	vector<vector<Edge>> adj;
	Dinic(int n) : lvl(n), ptr(n), q(n), adj(n) {}
	std::pair<int, int> addEdge(int a, int b, ll c, ll rcap = 0) {
		std::pair<int, int> res(a, sz(adj[a]));
		adj[a].push_back({b, sz(adj[b]), c, c});
		adj[b].push_back({a, sz(adj[a]) - 1, rcap, rcap});
		return res;
	}
	ll dfs(int v, int t, ll f) {
		if (v == t || !f) return f;
		for (int& i = ptr[v]; i < sz(adj[v]); i++) {
			Edge& e = adj[v][i];
			if (lvl[e.to] == lvl[v] + 1)
				if (ll p = dfs(e.to, t, min(f, e.c))) {
					e.c -= p, adj[e.to][e.rev].c += p;
					return p;
				}
		}
		return 0;
	}
	ll calc(int s, int t) {
		ll flow = 0; q[0] = s;
		//rep(L,0,31) do { // 'int L=30' maybe faster for random data
		int L = 30; do {
			lvl = ptr = vi(sz(q));
			int qi = 0, qe = lvl[s] = 1;
			while (qi < qe && !lvl[t]) {
				int v = q[qi++];
				for (Edge e : adj[v])
					if (!lvl[e.to] && e.c >> (30 - L))
						q[qe++] = e.to, lvl[e.to] = lvl[v] + 1;
			}
			while (ll p = dfs(s, t, LLONG_MAX)) flow += p;
		} while (lvl[t]);
		return flow;
	}
	bool leftOfMinCut(int a) { return lvl[a] != 0; }
};
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	kactl::Dinic flower(N+M + 2);
	const int SOURCE = N+M, SINK = N+M+1;
	std::vector<int> top_vert(N, -1);
	int next_vert = N;

	std::vector<int> par(N+M, -1);
	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};

	int64_t tot_demand = 0;
	for (int q = 0; q < M; q++) {
		int op; cin >> op;
		if (op == 1) {
			int i, j; cin >> i >> j; i--, j--;
			i = get_par(i);
			j = get_par(j);
			int v = next_vert++;
			flower.addEdge(i, v, N);
			flower.addEdge(j, v, N);
			par[i] = v;
			par[j] = v;
		} else if (op == 2) {
			int i; cin >> i; i--;
			top_vert[i] = get_par(i);
		} else if (op == 3) {
			int i, l, h; cin >> i >> l >> h; i--;
			i = get_par(i);
			int v = next_vert++;
			flower.addEdge(SOURCE, v, l);
			flower.addEdge(i, SINK, l);
			flower.addEdge(i, v, h-l);
			par[i] = v;
			tot_demand += l;
		} else assert(false);
	}

	std::vector<std::pair<int, int>> edge_locs(N);
	for (int i = 0; i < N; i++) {
		// lose all remaining marbles
		if (top_vert[i] == -1) {
			top_vert[i] = get_par(i);
		}
		// Add the marble edges
		edge_locs[i] = flower.addEdge(top_vert[i], i, 1);
	}

	int64_t f = flower.calc(SOURCE, SINK);
	if (f == tot_demand) {
		cout << "YES" << '\n';
		for (int i = 0; i < N; i++) {
			auto [a, b] = edge_locs[i];
			cout << (flower.adj[a][b].flow() ? 'R' : 'B');
		}
		cout << '\n';
	} else {
		cout << "NO" << '\n';
	}

	return 0;
}
