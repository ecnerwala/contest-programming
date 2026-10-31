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

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::vector<std::pair<int, int>>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back({-1, v});
			adj[v].push_back({-1, u});
		}

		int ROOT = 0;
		while (int(adj[ROOT].size()) > 1) ROOT++;

		std::y_combinator([&](auto self, int cur, int prv) -> int {
			if (prv != -1) {
				auto it = adj[cur].begin();
				while (it->second != prv) ++it;
				adj[cur].erase(it);
			}
			int best;
			if (adj[cur].empty()) {
				best = cur;
			} else {
				best = N;
				for (auto& [v, nxt] : adj[cur]) {
					v = self(nxt, cur);
					best = min(best, v);
				}
				std::sort(adj[cur].begin(), adj[cur].end());
			}
			return best;
		})(ROOT, -1);

		std::vector<int> ans; ans.reserve(N);
		auto solve_down = std::y_combinator([&](auto self, int cur) -> void {
			for (auto [_, nxt] : adj[cur]) {
				self(nxt);
			}
			ans.push_back(cur);
		});
		std::y_combinator([&](auto self, int cur) -> void {
			if (adj[cur].empty()) {
				ans.push_back(cur);
				return;
			}
			for (int z = 0; z+1 < int(adj[cur].size()); z++) {
				solve_down(adj[cur][z].second);
			}
			if (adj[cur].back().first < cur) {
				solve_down(adj[cur].back().second);
				ans.push_back(cur);
			} else {
				ans.push_back(cur);
				self(adj[cur].back().second);
			}
		})(ROOT);

		for (int i = 0; i < N; i++) {
			cout << ans[i]+1 << " \n"[i+1==N];
		}
	}

	return 0;
}
