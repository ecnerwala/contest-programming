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

	int N, Q; cin >> N >> Q;
	std::vector<int> W(N);
	for (auto& w : W) cin >> w;
	std::vector<std::vector<std::pair<int, int64_t>>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; int64_t l; cin >> u >> v >> l; u--, v--;
		adj[u].push_back({v, l});
		adj[v].push_back({u, l});
	}

	std::vector<std::vector<std::pair<int, int64_t>>> queries(N);
	for (int q = 0; q < Q; q++) {
		int x; int64_t k; cin >> x >> k; x--;
		queries[x].push_back({q, k});
	}

	int global_mex;
	{
		std::vector<bool> global_has(N);
		for (auto w : W) {
			if (w < N) {
				assert(!global_has[w]);
				global_has[w] = true;
			}
		}
		global_mex = 0;
		while (global_mex < N && global_has[global_mex]) global_mex++;
	}
	std::vector<int> ans(Q, global_mex);

	std::vector<bool> dead(N);
	std::vector<int> sz(N);
	auto dfs_sz = std::y_combinator([&](auto self, int cur, int prv) -> int {
		if (dead[cur]) return sz[cur] = 0;
		int v = 1;
		for (auto [nxt, _] : adj[cur]) {
			if (nxt == prv) continue;
			v += self(nxt, cur);
		}
		return sz[cur] = v;
	});
	dfs_sz(0, -1);

	std::vector<std::tuple<int64_t, int, int>> pts; pts.reserve(N);
	struct best_two {
		int min_val; int min_sub;
		int min_val_2;
	};
	std::vector<std::pair<int64_t, best_two>> bests; bests.reserve(N+1);

	std::y_combinator([&](auto dfs_centroid, int rt) -> void {
		assert(!dead[rt]);
		int tot_sz = sz[rt];
		while (true) {
			int nxt_rt = -1;
			for (auto [ch, _] : adj[rt]) {
				if (!dead[ch] && sz[ch] < sz[rt] && 2 * sz[ch] > tot_sz) {
					nxt_rt = ch;
					break;
				}
			}
			if (nxt_rt == -1) break;
			rt = nxt_rt;
		}
		dfs_sz(rt, -1);

		// Now, just sort the points
		pts.clear();
		pts.push_back({0, W[rt], rt});
		for (auto [ch, l_ch] : adj[rt]) {
			std::y_combinator([&](auto self, int cur, int prv, int64_t cur_depth) -> void {
				pts.push_back({cur_depth, W[cur], ch});
				for (auto [nxt, l] : adj[cur]) {
					if (nxt == prv) continue;
					if (dead[nxt]) continue;
					self(nxt, cur, cur_depth + l);
				}
			})(ch, rt, l_ch);
		}
		std::sort(pts.begin(), pts.end(), std::greater<>());
		bests.clear();
		bests.push_back({1e18, {global_mex, -1, global_mex}});
		for (auto [d, w, ch] : pts) {
			auto b = bests.back().second;
			if (w < b.min_val) {
				if (ch != b.min_sub) {
					b.min_val_2 = b.min_val;
				}
				b.min_sub = ch;
				b.min_val = w;
			} else if (ch != b.min_sub && w < b.min_val_2) {
				b.min_val_2 = w;
			}
			bests.push_back({d, b});
		}
		auto solve_pt = [&](int cur, int64_t cur_depth, int ch) {
			for (auto [q, k] : queries[cur]) {
				int mi = 0;
				int ma = int(bests.size());
				while (ma - mi > 0) {
					int md = (mi + ma) / 2;
					if (cur_depth + bests[md].first > k) {
						mi = md;
					} else {
						ma = md;
					}
				}
				auto& b = bests[mi].second;
				int v = (b.min_sub == ch ? b.min_val_2 : b.min_val);
				ans[q] = std::min(ans[q], v);
			}
		};

		solve_pt(rt, 0, rt);
		for (auto [ch, l_ch] : adj[rt]) {
			std::y_combinator([&](auto self, int cur, int prv, int64_t cur_depth) -> void {
				solve_pt(cur, cur_depth, ch);
				for (auto [nxt, l] : adj[cur]) {
					if (nxt == prv) continue;
					if (dead[nxt]) continue;
					self(nxt, cur, cur_depth + l);
				}
			})(ch, rt, l_ch);
		}

		dead[rt] = true;
		sz[rt] = 0;
		for (auto [nxt, _] : adj[rt]) {
			if (!dead[nxt]) {
				dfs_centroid(nxt);
			}
		}
	});

	for (int q = 0; q < Q; q++) {
		cout << ans[q] << '\n';
	}

	return 0;
}
