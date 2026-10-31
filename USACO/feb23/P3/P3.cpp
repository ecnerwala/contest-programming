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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<bool> used(N);
	{
		string s; cin >> s;
		for (int i = 0; i < N; i++) used[i] = s[i] == '1';
	}

	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	// first, prune off the dead ones
	std::vector<bool> dead(N);
	{
		std::vector<int> deg(N);
		std::vector<int> del_xor(N);
		std::vector<int> q; q.reserve(N);
		for (int i = 0; i < N; i++) {
			deg[i] = int(adj[i].size());
			if (!used[i] && deg[i] == 1) {
				q.push_back(i);
			}
		}
		for (int z = 0; z < int(q.size()); z++) {
			int cur = q[z];
			dead[cur] = true;
			int nxt = del_xor[cur];
			for (int n : adj[cur]) {
				nxt ^= n;
			}
			deg[nxt]--;
			del_xor[nxt] ^= cur;
			if (!used[nxt] && deg[nxt] == 1) {
				q.push_back(nxt);
			}
		}
	}

	for (int i = 0; i < N; i++) {
		if (dead[i]) {
			adj[i] = {};
			continue;
		}
		int sz = 0;
		for (int z = 0; z < int(adj[i].size()); z++) {
			int v = adj[i][z];
			if (dead[v]) continue;
			adj[i][sz++] = v;
		}
		adj[i].resize(sz);
	}

	int num_verts = 0;
	for (int i = 0; i < N; i++) {
		if (dead[i]) continue;

		if (used[i]) num_verts++;
	}
	assert(num_verts >= 1);
	int num_edges = 0;

	int E = 0;
	struct edge_t {
		int x; // st ^ en
		std::array<int, 2> ends;
		int l; // length in edges
		bool used = false;
	};
	std::vector<edge_t> edges; edges.reserve(N-1);
	std::vector<std::vector<int>> compressed_adj(N);

	int root = 0;
	while (!used[root]) root++;

	std::y_combinator([&](auto self, int cur, int prv) -> void {
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			int nxt_prv = cur;
			int l = 1;
			while (!used[nxt] && adj[nxt].size() == 2) {
				l++;
				int nxt_nxt = adj[nxt][0] ^ adj[nxt][1] ^ nxt_prv;
				nxt_prv = nxt;
				nxt = nxt_nxt;
			}
			int e = E++;
			edges.push_back({cur ^ nxt, {cur, nxt}, l});
			compressed_adj[cur].push_back(e);
			compressed_adj[nxt].push_back(e);
			//cerr << "edge " << e << " = " << cur << '-' << nxt << ' ' << l << '\n';
			self(nxt, nxt_prv);
		}
	})(root, -1);

	std::vector<int> unused_edges(E);
	std::iota(unused_edges.begin(), unused_edges.end(), 0);

	std::vector<int> vis(E, -1);

	std::vector<std::array<int64_t, 2>> dp(N);

	for (int K = 1; K <= N; K++) {
		//cerr << "K = " << K << '\n';

		for (int root_e : unused_edges) {
			if (vis[root_e] == K) continue;
			vis[root_e] = K;
			//cerr << "checking edge " << root_e << '\n';
			assert(!edges[root_e].used);
			auto dfs_down = std::y_combinator([&](auto self, int cur, int prv_e) -> void {
				if (used[cur]) {
					dp[cur] = {0, 0};
					return;
				}
				dp[cur] = {0, K+1};
				for (int e : compressed_adj[cur]) {
					if (e == prv_e) continue;
					vis[e] = K;
					int nxt = edges[e].x ^ cur;
					self(nxt, e);
					dp[cur][0] += std::min(dp[nxt][0], dp[nxt][1]);
					dp[cur][1] += std::min(dp[nxt][0], dp[nxt][1] + std::min(0, edges[e].l - 1 - K));
				}
			});
			auto [u, v] = edges[root_e].ends;
			dfs_down(u, root_e);
			dfs_down(v, root_e);
			bool d_u, d_v;
			if (dp[u][1] + dp[v][1] + edges[root_e].l - 1 - K <= std::min(dp[u][0], dp[u][1]) + std::min(dp[v][0], dp[v][1])) {
				d_u = true;
				d_v = true;

				{
					//cerr << "K = " << K << " " << "using root edge " << root_e << '\n';
					assert(!edges[root_e].used);
					edges[root_e].used = true;
					num_edges += edges[root_e].l;
					num_verts += edges[root_e].l - 1;
				}
			} else {
				d_u = dp[u][1] <= dp[u][0];
				d_v = dp[v][1] <= dp[v][0];
			}

			//cerr << "Best " << u << ':' << dp[u][0] << '/' << dp[u][1] << '=' << d_u << ' ' << v << ':' << dp[v][0] << '/' << dp[v][1] << '=' << d_v << ' ' << "mid " << edges[root_e].l << '\n';

			auto dfs_up = std::y_combinator([&](auto self, int cur, int prv_e, bool dir) -> void {
				if (used[cur]) {
					return;
				}
				if (dir == 1) {
					//cerr << "K = " << K << " " << "using vert " << cur << '\n';
					assert(!used[cur]);
					used[cur] = true;
					num_verts++;
				}

				for (int e : compressed_adj[cur]) {
					if (e == prv_e) continue;
					int nxt = edges[e].x ^ cur;

					bool ndir;
					if (dir == 1) {
						ndir = dp[nxt][1] + std::min(0, edges[e].l - 1 - K) <= dp[nxt][0];
						if (ndir && edges[e].l - 1 - K <= 0) {
							//cerr << "K = " << K << " " << "using edge " << e << '\n';
							assert(!edges[e].used);
							edges[e].used = true;
							num_edges += edges[e].l;
							num_verts += edges[e].l - 1;
						}
					} else {
						ndir = dp[nxt][1] <= dp[nxt][0];
					}
					if (used[nxt]) assert(ndir);

					self(nxt, e, ndir);
				}
			});
			dfs_up(u, root_e, d_u);
			dfs_up(v, root_e, d_v);
		}

		{
			int sz = 0;
			for (int i = 0; i < int(unused_edges.size()); i++) {
				int e = unused_edges[i];
				if (!edges[e].used) {
					unused_edges[sz++] = e;
				}
			}
			unused_edges.resize(sz);
		}
		cout << K * (num_verts - num_edges) + num_verts << '\n';
	}

	return 0;
}
