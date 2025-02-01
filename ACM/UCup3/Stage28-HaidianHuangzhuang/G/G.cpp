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

	int N, A, B; cin >> N >> A >> B;
	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	const int64_t INF = 1e18;
	cout << std::y_combinator([&](auto self, int cur, int prv) -> std::array<int64_t, 3> {
		std::array<int64_t, 3> res{0, B, INF};
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			std::array<int64_t, 3> ch = self(nxt, cur);
			ch[1] -= 2 * A;
			res = {
				res[0] + ch[0],
				std::min(std::min(res[0], res[2]) + ch[1], res[1] + std::min(ch[0], ch[2])),
				std::min({res[1] + ch[1], res[0] + ch[2], res[2] + ch[0], res[2] + ch[2]})
			};
		}
		return res;
	})(0, -1)[2] / 2 + int64_t(A) * int64_t(N+1) - B << '\n';

	return 0;
}
