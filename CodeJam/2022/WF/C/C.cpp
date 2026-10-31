#include <bits/stdc++.h>

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

namespace kactl {

using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

// https://github.com/kth-competitive-programming/kactl/blob/main/content/graph/PushRelabel.h
struct PushRelabel {
	struct Edge {
		int dest, back;
		ll f, c;
	};
	vector<vector<Edge>> g;
	vector<ll> ec;
	vector<Edge*> cur;
	vector<vi> hs; vi H;
	PushRelabel(int n) : g(n), ec(n), cur(n), hs(2*n), H(n) {}

	void addEdge(int s, int t, ll cap, ll rcap=0) {
		if (s == t) return;
		g[s].push_back({t, sz(g[t]), 0, cap});
		g[t].push_back({s, sz(g[s])-1, 0, rcap});
	}

	void addFlow(Edge& e, ll f) {
		Edge &back = g[e.dest][e.back];
		if (!ec[e.dest] && f) hs[H[e.dest]].push_back(e.dest);
		e.f += f; e.c -= f; ec[e.dest] += f;
		back.f -= f; back.c += f; ec[back.dest] -= f;
	}
	ll calc(int s, int t) {
		int v = sz(g); H[s] = v; ec[t] = 1;
		vi co(2*v); co[0] = v-1;
		rep(i,0,v) cur[i] = g[i].data();
		for (Edge& e : g[s]) addFlow(e, e.c);

		for (int hi = 0;;) {
			while (hs[hi].empty()) if (!hi--) return -ec[s];
			int u = hs[hi].back(); hs[hi].pop_back();
			while (ec[u] > 0)  // discharge u
				if (cur[u] == g[u].data() + sz(g[u])) {
					H[u] = 1e9;
					for (Edge& e : g[u]) if (e.c && H[u] > H[e.dest]+1)
						H[u] = H[e.dest]+1, cur[u] = &e;
					if (++co[H[u]], !--co[hi] && hi < v)
						rep(i,0,v) if (hi < H[i] && H[i] < v)
							--co[H[i]], H[i] = v + 1;
					hi = H[u];
				} else if (cur[u]->c && H[u] == H[cur[u]->dest]+1)
					addFlow(*cur[u], min(ec[u], cur[u]->c));
				else ++cur[u];
		}
	}
	bool leftOfMinCut(int a) { return H[a] >= sz(g); }
};
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		cerr << "Case #" << case_num << '\n';

		int V, E; cin >> V >> E;
		int tot_cnt = int(1e6) / V;
		assert(tot_cnt >= E);

		kactl::PushRelabel flower(2*V+2);
		int SOURCE = 2*V;
		int SINK = 2*V+1;

		std::vector<std::vector<int>> adj(V);

		std::vector<int> in_deg(V);
		std::vector<int> out_deg(V);

		for (int e = 0; e < E; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			out_deg[u]++;
			in_deg[v]++;

			flower.addEdge(u, V+v, tot_cnt);
		}

		for (int i = 0; i < V; i++) {
			flower.addEdge(SOURCE, i, tot_cnt - out_deg[i]);
			flower.addEdge(V+i, SINK, tot_cnt - in_deg[i]);
		}

		cout << "Case #" << case_num << ": ";
		if (flower.calc(SOURCE, SINK) != tot_cnt * V - E) {
			cout << "IMPOSSIBLE" << '\n';
		} else {
			for (int i = 0; i < V; i++) {
				for (auto e : flower.g[i]) {
					if (e.f > 0) {
						assert(V <= e.dest && e.dest < 2 * V);
						for (int z = 0; z < e.f; z++) {
							adj[i].push_back(e.dest - V);
						}
					}
				}
			}
			std::vector<int> path; path.reserve(tot_cnt * V + 1);
			std::vector<int> stk; stk.reserve(tot_cnt * V + 1);
			stk.push_back(0);
			while (!stk.empty()) {
				int cur = stk.back();
				if (!adj[cur].empty()) {
					int nxt = adj[cur].back(); adj[cur].pop_back();
					stk.push_back(nxt);
				} else {
					path.push_back(cur);
					stk.pop_back();
				}
			}
			std::reverse(path.begin(), path.end());
			if (int(path.size()) != tot_cnt * V + 1) {
				cout << "IMPOSSIBLE" << '\n';
			} else {
				cout << int(path.size()) << '\n';
				for (int z = 0; z < int(path.size()); z++) {
					cout << path[z]+1 << " \n"[z+1==int(path.size())];
				}
			}
		}
	}

	return 0;
}
