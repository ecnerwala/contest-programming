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

struct Dinic {
	struct Edge {
		int to, rev;
		ll c, oc;
		ll flow() { return max(oc - c, 0LL); } // if you need flows
	};
	vi lvl, ptr, q;
	vector<vector<Edge>> adj;
	Dinic(int n) : lvl(n), ptr(n), q(n), adj(n) {}
	void addEdge(int a, int b, ll c, ll rcap = 0) {
		adj[a].push_back({b, sz(adj[b]), c, c});
		adj[b].push_back({a, sz(adj[a]) - 1, rcap, rcap});
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
		rep(L,0,31) do { // 'int L=30' maybe faster for random data
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

#undef rep
#undef all
#undef sz
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int CASE_NUM; cin >> CASE_NUM;
	while (CASE_NUM--) {
		int R, C; cin >> R >> C;
		int C_R, C_C; cin >> C_R >> C_C;

		std::vector<std::string> diags(R);
		for (auto& r : diags) cin >> r;

		std::vector<std::string> filled(R);
		for (auto& r : filled) cin>> r;

		std::vector<std::vector<int>> diag_cost(R, std::vector<int>(C));
		for (auto& r : diag_cost) for (auto& c : r) cin >> c;

		const int V = R*C*2;
		std::vector<std::vector<std::pair<int, int>>> adj(V);

		std::vector<std::vector<int>> v_adj(V);
		std::vector<std::vector<int>> p_adj((R+1)*(C+1));
		auto add_pt = [&](int i, int j, int z, int px, int py) -> void {
			if (filled[i][2*j+z] == '#') {
				int x = (i*C+j)*2+z;
				int y = px*(C+1)+py;
				v_adj[x].push_back(y);
				p_adj[y].push_back(x);
			}
		};

		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				int d = (diags[i][j] == '/') ? 1 : -1;

				add_pt(i, j, 0, i+0, j+0);
				add_pt(i, j, 0, i+1, j+0);
				add_pt(i, j, 1, i+0, j+1);
				add_pt(i, j, 1, i+1, j+1);
				if (d == 1) {
					add_pt(i, j, 0, i+0, j+1);
					add_pt(i, j, 1, i+1, j+0);
				} else {
					add_pt(i, j, 0, i+1, j+1);
					add_pt(i, j, 1, i+0, j+0);
				}

				adj[(i*C+j)*2+0].push_back({j-1 >= 0 ? ((i*C+(j-1))*2+1) : -1, C_C});
				adj[(i*C+j)*2+0].push_back({i-d >= 0 && i-d < R ? (((i-d)*C+j)*2 + (diags[i-d][j] == diags[i][j] ? 1 : 0)) : -1, C_R});
				adj[(i*C+j)*2+0].push_back({(i*C+j)*2+1, diag_cost[i][j]});

				adj[(i*C+j)*2+1].push_back({(i*C+j)*2+0, diag_cost[i][j]});
				adj[(i*C+j)*2+1].push_back({i+d >= 0 && i+d < R ? (((i+d)*C+j)*2 + (diags[i+d][j] == diags[i][j] ? 0 : 1)) : -1, C_R});
				adj[(i*C+j)*2+1].push_back({j+1 <  C ? ((i*C+(j+1))*2+0) : -1, C_C});
			}
		}

		std::vector<int> cc_id(V, -1);
		std::vector<int> q; q.reserve(V);
		int num_ccs = 0;
		for (int st = 0; st < V; st++) {
			if (cc_id[st] == -1 && filled[st/(2*C)][st%(2*C)] == '#') {
				int id = num_ccs++;
				cc_id[st] = id;
				q.push_back(st);
				for (int z = 0; z < int(q.size()); z++) {
					int cur = q[z];
					// TODO: handle touching at corner too
					for (int p : v_adj[cur]) {
						for (int nxt : p_adj[p]) {
							if (cc_id[nxt] == -1) {
								cc_id[nxt] = id;
								q.push_back(nxt);
							}
						}
					}
					for (auto [nxt, _] : adj[cur]) {
						if (cc_id[nxt] == -1) {
							cc_id[nxt] = id;
							if (filled[nxt/(2*C)][nxt%(2*C)] == '#') {
								q.push_back(nxt);
							}
						}
					}
				}
				q.clear();
			}
		}

		/*
		for (int i = 0; i < R; i++) {
			for (int j = 0; j < 2*C; j++) {
				int v = cc_id[i*(2*C)+j];
				if (v == -1) cerr << '-';
				else cerr << v;
			}
			cerr << '\n';
		}
		*/

		std::vector<int> vert_id(V, -1);
		std::vector<int64_t> costs; costs.reserve(num_ccs);
		auto solve = std::y_combinator([&](auto self, std::vector<int> verts, int s_lo, int s_hi, std::optional<int64_t> known_cost) -> void {
			if (s_lo >= s_hi) return;
			if (s_hi - s_lo == 1 && known_cost) {
				costs.push_back(*known_cost);
				return;
			}

			int s_mi = s_lo + (s_hi - s_lo + 1) / 2;

			kactl::Dinic flower(int(verts.size())+2);
			int SOURCE = int(verts.size());
			int SINK = int(verts.size()) + 1;

			const int64_t INF = 1e9;

			for (int i = 0; i < int(verts.size()); i++) { vert_id[verts[i]] = i; }


			/*
			cerr << s_lo << ".." << s_mi << ".." << s_hi << '\n';
			for (int i = 0; i < R; i++) {
				for (int j = 0; j < 2*C; j++) {
					if (vert_id[i*(2*C)+j] == -1) {
						cerr << '.';
					} else {
						cerr << '#';
					}
				}
				cerr << '\n';
			}
			*/

			for (int i = 0; i < int(verts.size()); i++) {
				for (auto [u, l] : adj[verts[i]]) {
					int j = u == -1 ? -1 : vert_id[u];
					if (j == -1) {
						flower.addEdge(i, SINK, l);
					} else if (j > i) {
						// add undirected edge, only in 1 dir
						flower.addEdge(i, j, l, l);
					}
				}
				int s = cc_id[verts[i]];
				if (s != -1) {
					assert(s_lo <= s && s < s_hi);
					if (s < s_mi) {
						flower.addEdge(SOURCE, i, INF);
					} else {
						flower.addEdge(i, SINK, INF);
					}
				}
			}
			int64_t cost = flower.calc(SOURCE, SINK);
			/*
			cerr << s_lo << ".." << s_mi << ".." << s_hi << " cost " << cost << '\n';
			for (int i = 0; i < R; i++) {
				for (int j = 0; j < 2*C; j++) {
					if (vert_id[i*(2*C)+j] == -1) {
						cerr << '.';
					} else {
						if (flower.leftOfMinCut(vert_id[i*2*C+j])) {
							cerr << 'A';
						} else {
							cerr << 'B';
						}
					}
				}
				cerr << '\n';
			}
			*/

			for (int v : verts) { vert_id[v] = -1; }

			if (s_hi - s_lo == 1) {
				costs.push_back(cost);
			} else {
				std::vector<int> left_verts; left_verts.reserve(verts.size());
				std::vector<int> right_verts; right_verts.reserve(verts.size());
				for (int i = 0; i < int(verts.size()); i++) {
					if (flower.leftOfMinCut(i)) {
						left_verts.push_back(verts[i]);
					} else {
						right_verts.push_back(verts[i]);
					}
				}
				verts = {};
				flower = kactl::Dinic(0);

				self(left_verts, s_lo, s_mi, cost);
				self(right_verts, s_mi, s_hi, std::nullopt);
			}

		});

		std::vector<int> root_verts(V);
		std::iota(root_verts.begin(), root_verts.end(), 0);
		solve(root_verts, 0, num_ccs, std::nullopt);

		std::sort(costs.begin(), costs.end());
		cout << costs.size() << '\n';
		for (int i = 0; i < int(costs.size()); i++) {
			cout << costs[i] << " \n"[i+1==int(costs.size())];
		}
	}

	return 0;
}
