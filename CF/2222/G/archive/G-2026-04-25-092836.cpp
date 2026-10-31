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

		ans[N] = N;

		for (int v = 0; v < N; v++) {
			if (N == 1) continue;
			int excl = *std::max_element(adj[v].begin(), adj[v].end(), [&](auto a, auto b) { return sz[a] < sz[b]; });
			if (v != rt) assert(sz[excl] > sz[v]);
			std::map<int, int> mp;
			for (int nxt : adj[v]) {
				if (nxt == excl) continue;
				mp[sz[nxt]]++;
			}
			std::vector<std::pair<int, int>> freqs(mp.begin(), mp.end());
			for (int i = 0; i < int(freqs.size()); i++) {
				ans[N - freqs[i].first] += freqs[i].first * freqs[i].second;
				if (freqs[i].second >= 2) {
					ans[N - freqs[i].first - freqs[i].first] +=
						(int64_t(freqs[i].second) * int64_t(freqs[i].second - 1) / 2)
						* int64_t(freqs[i].first) * int64_t(freqs[i].first);
				}
				for (int j = i+1; j < int(freqs.size()); j++) {
					ans[N - freqs[i].first - freqs[j].first] +=
						int64_t(freqs[i].first * freqs[i].second) * int64_t(freqs[j].first * freqs[j].second);
				}
			}

			if (v == rt) {
				// do excl with everything else
				ans[N - sz[excl]] += sz[excl];
				std::vector<int> excl_sz(N+1);
				[&](this auto&& self, int cur, int prv, int val) -> void {
					excl_sz[std::max(val, sz[cur])]++;
					for (auto nxt : adj[cur]) {
						if (nxt == prv) continue;
						self(nxt, cur, std::max(val, sz[cur] - sz[nxt]));
					}
				}(excl, rt, 0);

				std::vector<int> easy_sz(N+1);
				for (int ch : adj[v]) {
					if (ch == excl) continue;
					[&](this auto&& self, int cur, int prv, int val) -> void {
						easy_sz[std::max(val, sz[cur])]++;
						for (auto nxt : adj[cur]) {
							if (nxt == prv) continue;
							self(nxt, cur, std::max(val, sz[cur] - sz[nxt]));
						}
					}(ch, rt, N - sz[excl] - sz[ch]);
				}

				int64_t p_excl = 0;
				int64_t p_easy = 0;
				for (int i = 0; i <= N; i++) {
					ans[i] += p_excl * easy_sz[i];
					ans[i] += p_easy * excl_sz[i];
					ans[i] += int64_t(easy_sz[i]) * int64_t(excl_sz[i]);
					p_excl += excl_sz[i];
					p_easy += easy_sz[i];
				}
			}
		}

		for (int i = 1; i <= N; i++) std::cout << ans[i] << " \n"[i==N];
	}

	return 0;
}
