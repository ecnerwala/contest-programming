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
	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	struct dp_t {
		int v_even;
		int v_odd;
		int v_none;
	};
	auto a = std::y_combinator([&](auto self, int cur, int prv) -> dp_t {
		int cnt_even = 0;
		int cnt_odd_0 = 0;
		int cnt_odd_1 = N+1;
		int cnt_odd_2 = N+1;
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			auto ch = self(nxt, cur);
			cnt_even += std::min(ch.v_even, ch.v_none);
			// each of them is either odd or none
			std::tie(cnt_odd_0, cnt_odd_1, cnt_odd_2) = std::make_tuple(
				cnt_odd_0 + ch.v_none,
				std::min(cnt_odd_0 + ch.v_odd, cnt_odd_1 + ch.v_none),
				std::min(cnt_odd_1 + ch.v_odd, cnt_odd_2 + std::min(ch.v_none, ch.v_odd))
			);
		}
		return dp_t {
			std::min(cnt_odd_1, cnt_odd_2),
			cnt_even,
			std::min(cnt_even, cnt_odd_2) + 1
		};
	})(0, -1);
	cout << a.v_none - 1 << '\n';

	return 0;
}
