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
	std::string S; cin >> S;
	assert(int(S.size()) == N);
	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	std::vector<int> depth(N, -1);

	// we want the deepest, so we'll store -depth, idx
	std::vector<std::set<std::pair<int, int>>> dp_down_set(N);

	std::vector<int64_t> dp_down_val(N);
	std::y_combinator([&](auto self, int cur, int prv) -> void {
		depth[cur] = (prv == -1) ? 0 : depth[prv] + 1;
		if (prv != -1) {
			auto it = std::find(adj[cur].begin(), adj[cur].end(), prv);
			assert(it != adj[cur].end());
			adj[cur].erase(it);
		}
		for (int nxt : adj[cur]) {
			assert(nxt != prv);
			self(nxt, cur);
			dp_down_val[cur] += dp_down_val[nxt];
		}

		// sort in decreasing size, so heaviest first
		std::sort(adj[cur].begin(), adj[cur].end(), [&](auto i, auto j) { return dp_down_set[i].size() > dp_down_set[j].size(); });
		if (!adj[cur].empty()) {
			{
				int keep_sz = (adj[cur].size() >= 2 ? int(dp_down_set[adj[cur][1]].size()) : 0);
				std::swap(dp_down_set[cur], dp_down_set[adj[cur][0]]);
				// min the next guy down
				for (auto it : dp_down_set[cur]) {
					if (int(dp_down_set[adj[cur][0]].size()) < keep_sz) {
						dp_down_set[adj[cur][0]].insert(it);
					} else {
						break;
					}
				}
			}
			for (int z = 1; z < int(adj[cur].size()); z++) {
				int nxt = adj[cur][z];
				for (auto it : dp_down_set[nxt]) {
					dp_down_set[cur].insert(it);
				}
			}
		}
		if (S[cur] == '0') {
			// no snail, so let the deepest guy come up
			if (!dp_down_set[cur].empty()) {
				auto it = dp_down_set[cur].begin();
				auto [d, vert] = *it;
				d = -d;
				dp_down_set[cur].erase(it);
				assert(d >= depth[cur]);
				dp_down_val[cur] += d - depth[cur];
				dp_down_set[cur].insert({-depth[cur], cur});
			}
		} else {
			dp_down_set[cur].insert({-depth[cur], cur});
		}
	})(0, -1);

	std::vector<int64_t> ans(N);
	std::set<std::pair<int, int>> dp_up_set;
	int64_t dp_up_val = 0;
	std::y_combinator([&](auto self, int cur, int prv) -> void {
		auto do_insert = [&](int nxt) -> void {
			dp_up_val += dp_down_val[nxt];
			for (auto [d, vert] : dp_down_set[nxt]) {
				d = -d;
				assert(d >= depth[cur]);
				dp_up_set.insert({2 * depth[cur] - d, vert});
			}
		};
		auto do_erase = [&](int nxt) -> void {
			dp_up_val -= dp_down_val[nxt];
			for (auto [d, vert] : dp_down_set[nxt]) {
				d = -d;
				assert(d >= depth[cur]);
				bool did_erase = dp_up_set.erase({2 * depth[cur] - d, vert});
				assert(did_erase);
			}
		};
		for (int nxt : adj[cur]) {
			assert(nxt != prv);
			do_insert(nxt);
		}
		auto do_self = [&]() -> std::optional<std::pair<int, int>> {
			if (S[cur] == '0') {
				if (!dp_up_set.empty()) {
					auto it = dp_up_set.begin();
					auto [d, vert] = *it;
					dp_up_set.erase(it);
					assert(d <= depth[cur]);
					dp_up_val += depth[cur] - d;
					dp_up_set.insert({depth[cur], cur});
					assert(vert != cur);
					return std::pair<int, int>({d, vert});
				} else {
					return std::nullopt;
				}
			} else {
				dp_up_set.insert({depth[cur], cur});
				return std::pair<int, int>({depth[cur], cur});
			}
		};
		auto undo_self = [&](std::optional<std::pair<int, int>> v) {
			if (!v) return;
			bool did_erase = dp_up_set.erase({depth[cur], cur});
			assert(did_erase);
			auto [d, vert] = *v;
			if (vert != cur) {
				dp_up_set.insert({d, vert});
			}
		};

		{
			auto v = do_self();
			ans[cur] = dp_up_val;
			undo_self(v);
		}

		for (int nxt : adj[cur]) {
			assert(nxt != prv);
			do_erase(nxt);

			auto v = do_self();
			self(nxt, cur);
			undo_self(v);

			do_insert(nxt);
		}
		for (int nxt : adj[cur]) {
			assert(nxt != prv);
			do_erase(nxt);
		}
	})(0, -1);

	for (int i = 0; i < N; i++) {
		cout << ans[i] << " \n"[i+1==N];
	}

	return 0;
}
