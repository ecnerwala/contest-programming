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
	std::vector<int64_t> R(N); for (auto& r : R) cin >> r;
	std::vector<int64_t> C(N); for (auto& c : C) cin >> c;
	std::vector<std::vector<std::pair<int, int64_t>>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; int64_t l; cin >> u >> v >> l; u--, v--;
		adj[u].push_back({v, l});
		adj[v].push_back({u, l});
	}

	std::vector<std::pair<int64_t, int64_t>> nodes(N);

	auto do_cuts = [&](int cur, std::pair<int64_t, int64_t> nval) -> int {
		auto should_cut = [&](int a) -> bool {
			if (a == -1) return false;
			auto v1 = nodes[a];
			if (nval.second >= v1.second) {
				return true;
			}
			if (a == 0) return false;
			auto v0 = nodes[a-1];
			pair<int64_t, int64_t> s0{v1.first - v0.first, v0.second - v1.second};
			pair<int64_t, int64_t> s1{nval.first - v1.first, v1.second - nval.second};
			assert(s0.first >= 0 && s0.second >= 0);
			assert(s1.first >= 0 && s1.second >= 0);
			__int128 cross = __int128(s0.first) * __int128(s1.second) - __int128(s0.second) * __int128(s1.first);
			return cross <= 0;
		};

		int mi = -1, ma = cur+1;
		while (ma - mi > 1) {
			int md = mi + (ma - mi) / 2;
			if (should_cut(md)) ma = md;
			else mi = md;
		}
		return mi;
	};

	std::vector<double> ans(N);
	std::y_combinator([&](auto self, int cur, int prv, int64_t depth, int dp) -> void {
		{
			int cnd = do_cuts(dp, {depth, C[cur]});
			if (cnd == -1) ans[cur] = -1;
			else ans[cur] = double(depth - nodes[cnd].first) / double(nodes[cnd].second - C[cur]);
		}
		int p = do_cuts(dp, {depth, R[cur]});
		auto old = std::exchange(nodes[p+1], {depth, R[cur]});
		for (auto [nxt, len] : adj[cur]) {
			if (nxt == prv) continue;
			self(nxt, cur, depth + len, p+1);
		}
		nodes[p+1] = old;
	})(0, -1, 0, -1);

	for (int i = 1; i < N; i++) {
		if (ans[i] < -0.5) cout << "Unreachable" << '\n';
		else cout << fixed << setprecision(12) << ans[i] << '\n';
	}

	return 0;
}
