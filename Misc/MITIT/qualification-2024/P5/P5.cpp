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
	dfs(0, g, f);
	//rep(i,0,n) if (comp[i] < 0) dfs(i, g, f);
}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<std::vector<std::pair<int, int>>> adj(N);
		std::vector<std::vector<int>> adj2(N);
		for (int e = 0; e < M; e++) {
			int u, v, w; cin >> u >> v >> w; u--, v--;
			adj[u].push_back({v, w});
			adj2[u].push_back(v);
		}
		bool good = false;
		std::vector<int> successor(N, -1);
		std::vector<std::vector<std::pair<int, int>>> scc_adj(N);
		std::vector<int> depth(N, -1);
		kactl::scc(adj2, [&](const std::vector<int>& verts) {
			//cerr << "scc" << ' ' << int(verts.size()) << '\n';
			bool has_edge = false;
			for (int v : verts) {
				for (auto [u, e] : adj[v]) {
					assert(kactl::comp[u] != -1);
					if (kactl::comp[u] != kactl::comp[v]) continue;
					//cerr << v << ' ' << u << ' ' << e << '\n';
					scc_adj[v].push_back({u, e});
					has_edge = true;
				}
			}
			if (!has_edge) return;

			int g = 0;
			std::y_combinator([&](auto self, int cur, int d) -> void{
				if (depth[cur] != -1) {
					int c = std::abs(d - depth[cur]);
					if (g == 0) g = c;
					else if (c != 0) g = std::gcd(g, c);
					return;
				}
				depth[cur] = d;
				for (auto [nxt, _] : adj[cur]) {
					self(nxt, d+1);
				}
			})(verts[0], 0);
			assert(g != 0);
			//cerr << "gcd " << g << '\n';
			std::vector<int> cyc_colors(g, -1);
			for (int v : verts) {
				assert(depth[v] != -1);
				int d = depth[v] % g;
				for (auto [u, e] : scc_adj[v]) {
					if (cyc_colors[d] == -1) cyc_colors[d] = e;
					if (e != cyc_colors[d]) {
						//cerr << "mark good" << '\n';
						good = true;
						return;
					}
				}

			}
		});
		cout << (good ? "Yes" : "No") << '\n';
	}

	return 0;
}
