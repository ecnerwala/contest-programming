#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; std::cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}

		auto get_sz = [&](int rt) {
			std::vector<int> sz(N, 1);
			[&](this auto&& self, int cur, int prv) -> void {
				for (int nxt : adj[cur]) {
					if (nxt == prv) continue;
					self(nxt, cur);
					sz[cur] += sz[nxt];
				}
			}(rt, -1);
			return sz;
		};
		int rt = 0;
		{
			auto sz = get_sz(rt);
			while (true) {
				int n_rt = -1;
				for (int nxt : adj[rt]) {
					if (sz[nxt] > sz[rt]) continue;
					if (sz[nxt] * 2 > N) n_rt = nxt;
				}
				if (n_rt == -1) break;
				rt = n_rt;
			}
		}
		auto sz = get_sz(rt);

		std::vector<int64_t> ans(N+1);

		for (int i = 0; i < N; i++) {
			if (i == rt) continue;
			std::map<int, int> mp;
			for (int nxt : adj[i]) {
				if (sz[nxt] > sz[i]) continue;
				mp[sz[nxt]]++;
			}
			std::vector<std::pair<int, int>> freqs(mp.begin(), mp.end());
		}

		for (int i = 1; i <= N; i++) std::cout << ans[i] << '\n';
	}

	return 0;
}
