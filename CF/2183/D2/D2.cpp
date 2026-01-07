#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}
		std::vector<std::vector<std::pair<int, int>>> by_depth(N);
		[&](this auto&& self, int cur, int prv, int depth) -> void {
			by_depth[depth].push_back({cur, prv});
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				self(nxt, cur, depth+1);
			}
		}(0, -1, 0);
		int ans = 0;
		for (int d = 0; d < N; d++) {
			if (by_depth[d].empty()) break;
			if (d > 0 && [&]() -> bool {
				int p = by_depth[d][0].second;
				for (auto [_, op] : by_depth[d]) if (op != p) return false;
				return true;
			}()) {
				ans = std::max(ans, int(by_depth[d].size())+1);
			} else {
				ans = std::max(ans, int(by_depth[d].size()));
			}
		}
		std::vector<std::vector<int>> resp(ans);
		std::vector<int> last_depth(ans, N+1);
		std::vector<int> forbidden_loc(N, -1);
		std::vector<std::pair<std::pair<int, int>, int>> forbiddens; forbiddens.reserve(N);
		for (int d = N-1; d >= 0; d--) {
			if (by_depth[d].empty()) continue;
			auto& verts = by_depth[d];
			// each guy is forbidden in some place
			for (auto [cur, prv] : verts) {
				if (forbidden_loc[cur] != -1) {
					forbiddens.push_back({{cur, prv}, forbidden_loc[cur]});
				}
			}
			if (forbiddens.size() == 1) {
				auto [cur, prv] = forbiddens[0].first;
				int loc = 0;
				while (last_depth[loc] == d+1) loc++;
				cerr << "put " << d << ' ' << cur << ' ' << prv << ' ' << loc << '\n';
				resp[loc].push_back(cur);
				last_depth[loc] = d;
				if (prv != -1) forbidden_loc[prv] = loc;
			} else {
				for (int z = 0; z < int(forbiddens.size()); z++) {
					auto [cur, prv] = forbiddens[z].first;
					int loc = forbiddens[(z+1)%int(forbiddens.size())].second;
					cerr << "put " << d << ' ' << cur << ' ' << prv << ' ' << loc << '\n';
					assert(last_depth[loc] == d+1);
					resp[loc].push_back(cur);
					last_depth[loc] = d;
					if (prv != -1) forbidden_loc[prv] = loc;
				}
			}
			forbiddens.clear();
			{
				int loc = 0;
				for (auto [cur, prv] : verts) {
					if (forbidden_loc[cur] == -1) {
						while (last_depth[loc] == d) loc++;
						cerr << "put " << d << ' ' << cur << ' ' << prv << ' ' << loc << '\n';
						resp[loc].push_back(cur);
						last_depth[loc] = d;
						if (prv != -1) forbidden_loc[prv] = loc;
					}
				}
			}
		}
		cout << ans << '\n';
		for (auto& o : resp) {
			cout << o.size();
			for (auto v : o) cout << ' ' << v+1;
			cout << '\n';
		}
	}

	return 0;
}
