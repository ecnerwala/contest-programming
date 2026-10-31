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

	int N, K; cin >> N >> K;
	std::vector<int> W(N);
	for (auto& w : W) cin >> w;
	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	bool good = true;
	int64_t res = 0;
	auto [rmi, rma] = std::y_combinator([&](auto self, int cur, int prv) -> std::pair<int, int> {
		int64_t ch_min = 0;
		int64_t ch_max = 0;
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			auto [mi, ma] = self(nxt, cur);
			ch_min += mi;
			ch_max += ma;
		}
		ch_max = std::min<int64_t>(ch_max, K);
		if (W[cur] != -1) {
			ch_min += W[cur];
		}
		if (ch_min > K) good = false;
		res += ch_max;
		if (W[cur] != -1) {
			return {W[cur], W[cur]};
		} else {
			return {0, K - ch_max};
		}
	})(0, -1);
	res += rma;
	cout << (good ? -1 : res) << '\n';

	return 0;
}
