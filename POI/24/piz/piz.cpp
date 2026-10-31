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

template <typename T> using max_pq = std::priority_queue<T, std::vector<T>, std::less<T>>;
template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	vector<vector<pair<int, int64_t>>> adj(N);

	int64_t ans = 0;
	for (int e = 0; e < N-1; e++) {
		int u, v; int64_t w; cin >> u >> v >> w; u--, v--;
		ans += 2 * w;
		adj[u].push_back({v, w});
		adj[v].push_back({u, w});
	}

	vector<int64_t> depth(N);
	vector<int64_t> deepest(N);
	depth[0] = 0;
	std::y_combinator([&](auto self, int cur) -> void {
		deepest[cur] = depth[cur];
		for (auto [nxt, w] : adj[cur]) {
			depth[nxt] = depth[cur]+w;
			{
				auto it = adj[nxt].begin();
				while (it->first != cur) ++it;
				adj[nxt].erase(it);
			}
			self(nxt);
			deepest[cur] = max(deepest[cur], deepest[nxt]);
		}
	})(0);

	max_pq<pair<int64_t, int>> pq;
	pq.push({deepest[0], 0});
	for (int z = 0; z < K; z++) {
		if (pq.empty()) break;
		auto [val, cur] = pq.top(); pq.pop();
		if (val < 0) break;
		ans -= val;
		while (!adj[cur].empty()) {
			int ch = -1;
			for (auto [nxt, w] : adj[cur]) {
				if (ch == -1 && deepest[nxt] == deepest[cur]) {
					ch = nxt;
				} else {
					pq.push({deepest[nxt] - 2 * depth[cur], nxt});
				}
			}
			cur = ch;
		}
	}

	cout << ans << '\n';

	return 0;
}
