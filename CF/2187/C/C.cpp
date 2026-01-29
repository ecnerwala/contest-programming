#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		// Jerry should always take the furthest edge, since nothing is allowed to jump over it
		std::vector<int> nxt(N);
		std::iota(nxt.begin(), nxt.end(), 1);
		for (int e = 0; e < M; e++) {
			int u, v; cin >> u >> v; u--, v--;
			assert(u < v);
			assert(0 <= u && u < N-1);
			nxt[u] = std::max(nxt[u], v);
		}
		std::vector<int> sz(N);
		// cnt, cnt deeper
		std::vector<std::vector<std::pair<int, int>>> by_depth(N);
		int64_t ans = 0;
		for (int i = 0; i < N; i++) {
			sz[i]++;
			by_depth[i].push_back({1, sz[i]});
			if (i < N-1) {
				int p = nxt[i];
				sz[p] += sz[i];
				if (by_depth[p].size() < by_depth[i].size()) std::swap(by_depth[i], by_depth[p]);
				assert(by_depth[p].size() >= by_depth[i].size());
				for (int l = 1; l <= int(by_depth[i].size()); l++) {
					auto& a = by_depth[p].end()[-l];
					auto& b = by_depth[i].end()[-l];
					ans += int64_t(a.first) * int64_t(b.second) * int64_t(l);
					ans += int64_t(b.first) * int64_t(a.second) * int64_t(l);
					a.first += b.first;
					a.second += b.second;
				}
				by_depth[i] = {};
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
