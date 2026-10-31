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
	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	std::vector<int> par(N, -1);
	std::vector<int> depth(N, -1);
	std::vector<int> by_depth; by_depth.reserve(N);
	par[0] = -1;
	depth[0] = 0;
	by_depth.push_back(0);
	for (int z = 0, d = 1; z < int(by_depth.size()); d++) {
		for (int en = int(by_depth.size()); z < en; z++) {
			int cur = by_depth[z];
			for (int nxt : adj[cur]) {
				if (nxt == par[cur]) continue;
				par[nxt] = cur;
				depth[nxt] = d;
				by_depth.push_back(nxt);
			}
		}
	}
	assert(int(by_depth.size()) == N);

	std::vector<int> dist_0(N);
	for (int i = 0; i < N; i++) {
		dist_0[i] = (S[i] == '1' ? N+1 : 0);
	}
	for (int z = N-1; z > 0; z--) {
		int cur = by_depth[z];
		dist_0[par[cur]] = std::min(dist_0[par[cur]], dist_0[cur] + 1);
	}
	for (int z = 1; z < N; z++) {
		int cur = by_depth[z];
		dist_0[cur] = std::min(dist_0[cur], dist_0[par[cur]] + 1);
	}

	int Q; cin >> Q;
	for (int q = 0; q < Q; q++) {
		int K; cin >> K;
		assert(K <= N);
		// can take only if dist_0 > K
		std::vector<bool> used(N);
		for (int z = N-1; z >= 0; z--) {
			int deep_v = by_depth[z];
			if (S[deep_v] == '0' || used[deep_v]) continue;
			std::y_combinator([&](auto self, int cur, int prv, int max_dist) -> std::pair<int, int> {
				std::pair<int, int> res = dist_0[cur] > K ? std::pair<int, int>{depth[cur], cur} : std::pair<int, int>{N+1, -1};
				if (max_dist > 0) {
					for (int nxt : adj[cur]) {
						if (nxt == prv) continue;
						self(nxt, cur);
					}
				}
			});
		}
	}

	return 0;
}
