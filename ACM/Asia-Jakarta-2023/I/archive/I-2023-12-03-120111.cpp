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
	std::vector<std::vector<std::pair<int, int>>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back({v, 0});
		adj[v].push_back({u, 1});
	}

	std::vector<std::array<int, 2>> ops; ops.reserve(N+1);
	auto add_op = [&](int x, int y, int xz) {
		if (x == N || y == N) return;
		if (xz == 1) ops.push_back({x, y});
		else ops.push_back({y, x});
	};

	std::array<int, 2> outer_range{N, N};
	auto root_range = std::y_combinator([&](auto self, int cur, int prv) -> std::array<int, 2> {
		std::array<int, 2> res{cur, cur};
		for (auto [nxt, cur_z] : adj[cur]) {
			if (nxt == prv) continue;
			auto nxt_res = self(nxt, cur);
			if (res[!cur_z] == cur && nxt_res[cur_z] == nxt) {
				// use existing edge
				res[!cur_z] = nxt_res[!cur_z];
			} else {
				// add edge from nxt_res to the end
				add_op(nxt_res[!cur_z], outer_range[cur_z], !cur_z);
				outer_range[cur_z] = nxt_res[cur_z];
			}
		}
		return res;
	})(0, -1);
	add_op(root_range[0], outer_range[1], 0);
	add_op(root_range[1], outer_range[0], 1);

	cout << ops.size() << '\n';
	for (auto [x, y] : ops) {
		cout << x+1 << ' ' << y+1 << '\n';
	}

	return 0;
}
