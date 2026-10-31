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

bool dfs(int a, int L, vector<vi>& g, vi& btoa, vi& A, vi& B) {
	if (A[a] != L) return 0;
	A[a] = -1;
	for (int b : g[a]) if (B[b] == L + 1) {
		B[b] = 0;
		if (btoa[b] == -1 || dfs(btoa[b], L + 1, g, btoa, A, B))
			return btoa[b] = a, 1;
	}
	return 0;
}

int hopcroftKarp(vector<vi>& g, vi& btoa) {
	int res = 0;
	vi A(g.size()), B(btoa.size()), cur, next;
	for (;;) {
		fill(all(A), 0);
		fill(all(B), 0);
		/// Find the starting nodes for BFS (i.e. layer 0).
		cur.clear();
		for (int a : btoa) if(a != -1) A[a] = -1;
		rep(a,0,sz(g)) if(A[a] == 0) cur.push_back(a);
		/// Find all layers using bfs.
		for (int lay = 1;; lay++) {
			bool islast = 0;
			next.clear();
			for (int a : cur) for (int b : g[a]) {
				if (btoa[b] == -1) {
					B[b] = lay;
					islast = 1;
				}
				else if (btoa[b] != a && !B[b]) {
					B[b] = lay;
					next.push_back(btoa[b]);
				}
			}
			if (islast) break;
			if (next.empty()) return res;
			for (int a : next) A[a] = lay;
			cur.swap(next);
		}
		/// Use DFS to scan for augmenting paths.
		rep(a,0,sz(g))
			res += dfs(a, 0, g, btoa, A, B);
	}
}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--; v -= N;
		adj[u].push_back(v);
	}

	std::vector<int> btoa(N, -1);
	int tot_matching = kactl::hopcroftKarp(adj, btoa);

	if (tot_matching < N) {
		cout << "NO" << '\n';
		std::vector<bool> used(N);
		for (int i = 0; i < N; i++) {
			if (btoa[i] != -1) used[btoa[i]] = true;
		}
		int rt = 0;
		while (used[rt]) rt++;

		std::vector<int> res; res.reserve(N);
		std::vector<bool> vis(N);
		std::y_combinator([&](auto self, int cur) -> void {
			if (vis[cur]) return;
			vis[cur] = true;
			res.push_back(cur);
			for (int nxt : adj[cur]) {
				int v = btoa[nxt];
				assert(v != -1);
				self(v);
			}
		})(rt);

		cout << res.size() << '\n';
		for (int z = 0; z < int(res.size()); z++) {
			cout << res[z]+1 << " \n"[z+1==int(res.size())];
		}
	} else {
		cout << "YES" << '\n';

		std::vector<std::pair<int, int>> edges; edges.reserve(M);
		for (int i = 0; i < N; i++) {
			edges.push_back({i, i});
		}

		for (int i = 0; i < N; i++) {
			for (auto& j : adj[i]) {
				j = btoa[j];
			}
		}

		int num_sccs = 0;
		std::vector<int> scc_id(N, -1);
		std::vector<int> scc_roots; scc_roots.reserve(N);
		{
			std::vector<int> idx(N, -1);
			int nxt_idx = 0;
			std::vector<int> lowval(N, -1);
			std::vector<int> stk; stk.reserve(N);
			for (int rt = 0; rt < N; rt++) {
				if (idx[rt] == -1) {
					std::y_combinator([&](auto self, int cur) -> void {
						assert(idx[cur] == -1);
						idx[cur] = nxt_idx++;
						stk.push_back(cur);
						lowval[cur] = idx[cur];
						for (int nxt : adj[cur]) {
							if (idx[nxt] == -1) {
								self(nxt);
								lowval[cur] = std::min(lowval[cur], lowval[nxt]);
							} else if (scc_id[nxt] == -1) {
								lowval[cur] = std::min(lowval[cur], idx[nxt]);
							}
						}
						if (lowval[cur] == idx[cur]) {
							int id = num_sccs++;
							scc_roots.push_back(cur);
							int prv = cur;
							while (true) {
								int v = stk.back(); stk.pop_back();
								scc_id[v] = id;
								if (prv != v) edges.push_back({prv, v});
								prv = v;
								if (v == cur) break;
							}
						}
					})(rt);
				}
			}
		}

		std::vector<std::vector<int>> scc_adj(num_sccs);
		for (int i = 0; i < N; i++) {
			for (int j : adj[i]) {
				assert(scc_id[i] >= scc_id[j]);
				if (scc_id[i] != scc_id[j]) {
					scc_adj[scc_id[i]].push_back(scc_id[j]);
				}
			}
		}

		for (int i = 0; i < num_sccs; i++) {
			std::sort(scc_adj[i].begin(), scc_adj[i].end(), std::greater<>());
		}

		std::vector<uint64_t> dp(num_sccs);
		for (int lo = 0; lo < num_sccs; lo += 64) {
			for (int i = lo; i < num_sccs; i++) {
				dp[i] = 0;
				if (i - lo < 64) {
					dp[i] = uint64_t(1) << (i - lo);
				}
				for (int j : scc_adj[i]) {
					if (j < lo) break;
					if (j - lo < 64 && !((dp[i] >> (j-lo)) & 1)) {
						edges.push_back({scc_roots[i], scc_roots[j]});
					}
					dp[i] |= dp[j];
				}
			}
		}

		cout << edges.size() << '\n';
		for (auto [u, v] : edges) {
			cout << u+1 << ' ' << v+N+1 << '\n';
		}
	}

	return 0;
}
