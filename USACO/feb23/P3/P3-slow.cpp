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

	std::vector<int> unused; unused.reserve(N);
	int num_verts = 0;
	for (int i = 0; i < N; i++) {
		if (used[i]) num_verts++;
		else unused.push_back(i);
	}

	std::vector<std::vector<int>> adj(N);
	int num_edges = 0;
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
		if (used[u] && used[v]) num_edges++;
	}

	std::vector<int> vis(N, 0);

	std::vector<std::array<int64_t, 2>> dp(N);

	for (int K = 1; K <= N; K++) {

		for (int root : unused) {
			if (vis[root] == K) continue;
			assert(!used[root]);
			std::y_combinator([&](auto self, int cur, int prv) -> void {
				if (used[cur]) {
					dp[cur] = {0, 0};
					return;
				}
				vis[cur] = K;
				dp[cur] = {0, K+1};
				for (int nxt : adj[cur]) {
					if (nxt == prv) continue;
					self(nxt, cur);
					dp[cur][0] += std::min(dp[nxt][0], dp[nxt][1]);
					dp[cur][1] += std::min(dp[nxt][0], dp[nxt][1] - K);
				}
			})(root, -1);
			std::y_combinator([&](auto self, int cur, int prv, bool dir) -> void {
				if (used[cur]) {
					return;
				}
				if (dir == 1) {
					used[cur] = true;
					num_verts++;
				}
				for (int nxt : adj[cur]) {
					if (nxt == prv) continue;
					bool ndir;
					if (dir == 1) ndir = dp[nxt][1] - K <= dp[nxt][0];
					else ndir = dp[nxt][1] <= dp[nxt][0];
					if (used[nxt]) assert(ndir);

					if (dir && ndir) num_edges++;

					self(nxt, cur, ndir);
				}
			})(root, -1, dp[root][1] <= dp[root][0]);
		}

		{
			int sz = 0;
			for (int i = 0; i < int(unused.size()); i++) {
				int v = unused[i];
				if (!used[v]) {
					unused[sz++] = v;
				}
			}
			unused.resize(sz);
		}
		cout << K * (num_verts - num_edges) + num_verts << '\n';
	}

	return 0;
}
