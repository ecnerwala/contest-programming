#include <bits/stdc++.h>

namespace kactl {
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

vi val, comp, z, cont;
int Time, ncomps;
template<class G, class F> int dfs(int j, G& g, F& f) {
	int low = val[j] = ++Time, x; z.push_back(j);
	for (auto e : g[j]) if (comp[e] < 0)
		low = min(low, val[e] ?: dfs(e,g,f));

	if (low == val[j]) {
		do {
			x = z.back(); z.pop_back();
			comp[x] = ncomps;
			cont.push_back(x);
		} while (x != j);
		f(cont); cont.clear();
		ncomps++;
	}
	return val[j] = low;
}
template<class G, class F> void scc(G& g, F f) {
	int n = sz(g);
	val.assign(n, 0); comp.assign(n, -1);
	Time = ncomps = 0;
	rep(i,0,n) if (comp[i] < 0) dfs(i, g, f);
}
}

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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::array<std::vector<std::vector<int>>, 2> adj;
		for (int z = 0; z < 2; z++) {
			adj[z].resize(N);
			for (int e = 0; e < N-1; e++) {
				int u, v; cin >> u >> v; u--, v--;
				adj[z][u].push_back(v);
				adj[z][v].push_back(u);
			}
		}

		std::array<std::vector<int>, 2> depth;
		std::array<std::vector<int>, 2> par;
		struct jump_t {
			int dist;
			int dest;
		};
		std::array<std::vector<jump_t>, 2> jumps;

		int V = 4*N;
		std::vector<std::vector<int>> graph_adj(V);
		for (int z = 0; z < 2; z++) {
			depth[z].assign(N, -1);
			par[z].assign(N, -1);
			jumps[z].assign(N, {-1, -1});

			std::y_combinator([&](auto self, int cur, int prv) -> void {
				depth[z][cur] = (prv == -1 ? 0 : depth[z][prv] + 1);
				par[z][cur] = prv;
				if (prv == -1) {
					jumps[z][cur] = {0, -1};
				} else if (jumps[z][prv].dest != -1 && jumps[z][jumps[z][prv].dest].dist == jumps[z][prv].dist){
					jumps[z][cur] = {1 + 2 * jumps[z][prv].dist, jumps[z][jumps[z][prv].dest].dest};
					graph_adj[z*(2*N) + 2*cur + 1].push_back(z * (2*N) + 2*jumps[z][prv].dest + 1);
					graph_adj[z*(2*N) + 2*cur + 1].push_back(z * (2*N) + 2*prv + 1);
					graph_adj[z*(2*N) + 2*cur + 1].push_back(z * (2*N) + 2*cur + 0);
				} else {
					jumps[z][cur] = {1, prv};
					graph_adj[z*(2*N) + 2*cur + 1].push_back(z * (2*N) + 2*cur + 0);
				}
				for (int nxt : adj[z][cur]) {
					if (nxt == prv) continue;
					self(nxt, cur);
				}
			})(0, -1);
		}

		for (int z = 0; z < 2; z++) {
			assert(par[z][0] == -1);
			for (int st = 1; st < N; st++) {
				int source = z * (2*N) + 2*st + 0;

				// traverse the path from a to b in graph[!z]
				int a = st, b = par[z][st];
				//cerr << z << ' ' << a << ' ' << b << ' ' << depth[!z][a] << ' ' << depth[!z][b] << '\n';
				if (depth[!z][a] > depth[!z][b]) std::swap(a, b);
				assert(depth[!z][a] <= depth[!z][b]);
				while (depth[!z][a] < depth[!z][b]) {
					if (jumps[!z][b].dist <= depth[!z][b] - depth[!z][a]) {
						graph_adj[source].push_back((!z) * (2*N) + 2*b + 1);
						b = jumps[!z][b].dest;
					} else {
						graph_adj[source].push_back((!z) * (2*N) + 2*b + 0);
						b = par[!z][b];
					}
					//cerr << z << ' ' << a << ' ' << b << ' ' << depth[!z][a] << ' ' << depth[!z][b] << '\n';
				}
				assert(depth[!z][a] == depth[!z][b]);
				while (a != b) {
					assert(depth[!z][a] == depth[!z][b]);
					assert(jumps[!z][a].dist == jumps[!z][b].dist);
					if (jumps[!z][a].dest != jumps[!z][b].dest) {
						graph_adj[source].push_back((!z) * (2*N) + 2*a + 1);
						graph_adj[source].push_back((!z) * (2*N) + 2*b + 1);
						a = jumps[!z][a].dest;
						b = jumps[!z][b].dest;
					} else {
						graph_adj[source].push_back((!z) * (2*N) + 2*a + 0);
						graph_adj[source].push_back((!z) * (2*N) + 2*b + 0);
						a = par[!z][a];
						b = par[!z][b];
					}
				}
			}
		}

		int cnt = 0;
		kactl::scc(graph_adj, [&](const auto& verts) {
			int num_0 = 0;
			std::array<int, 2> first_0{-1, -1};
			int num_1 = 0;
			std::array<int, 2> first_1{-1, -1};
			for (int v : verts) {
				if (v & 1) continue;
				if (v == 0 || v == 2 * N) continue;
				int z = v >= (2 * N);
				int a = (v - z * (2 * N)) / 2;
				assert(a != 0);
				int b = par[z][a];
				if (a > b) std::swap(a, b);
				if (z) {
					num_1++;
					first_1 = {a, b};
				} else {
					num_0++;
					first_0 = {a, b};
				}
			}
			assert(num_0 == num_1);
			if (num_0 == 0) return;
			if (num_0 == 1 && first_0 == first_1) return;
			cnt++;
		});

		const int MOD = int(1e9)+7;
		int ans = 1;
		for (int z = 0; z < cnt; z++) {
			ans += ans;
			ans = (ans >= MOD ? ans - MOD : ans);
		}
		cout << ans << '\n';

	}

	return 0;
}
