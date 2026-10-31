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

struct dp_t {
	// depth <= 32
	int depth;
	// allow overflowing
	uint64_t cost;
	uint64_t mult;
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	if (M != N-1) {
		cout << "NIE" << '\n';
		exit(0);
	}
	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	std::vector<std::vector<std::optional<dp_t>>> mem(N);
	for (int i = 0; i < N; i++) {
		if (adj[i].size() > 3) {
			cout << "NIE" << '\n';
			exit(0);
		}
		mem[i].resize(adj[i].size());
	}

	auto dfs = std::y_combinator([&](auto self, int cur, int prv) -> dp_t {
		for (int z = 0; z < int(adj[cur].size()); z++) {
			int nxt = adj[cur][z];
			if (nxt == prv) continue;
			if (!mem[cur][z]) {
				mem[cur][z] = self(nxt, cur);
			}
		}
		std::vector<dp_t> subs; subs.reserve(2);
		for (int z = 0; z < int(adj[cur].size()); z++) {
			int nxt = adj[cur][z];
			if (nxt == prv) continue;
			subs.push_back(*mem[cur][z]);
		}
		while (subs.size() < 2) subs.push_back(dp_t{0, 0, 0});
		return dp_t{
			1 + std::max(subs[0].depth, subs[1].depth),
			(subs[0].cost + subs[1].cost) + std::min(subs[0].mult, subs[1].mult),
			1 + 2 * subs[0].mult + 2 * subs[1].mult,
		};
	});
	std::optional<uint64_t> ans;
	for (int i = 0; i < N; i++) {
		if (adj[i].size() == 0) {
			ans = 0;
		} else if (adj[i].size() == 1) {
			// try it as 0
			int j = adj[i][0];
			auto r = dfs(j, i);
			if (r.depth <= 32) {
				uint64_t cnd = r.cost + r.mult;
				if (!ans || cnd < *ans) {
					ans = cnd;
				}
			}
		} else if (adj[i].size() == 2) {
			auto r = dfs(i, -1);
			if (r.depth <= 32) {
				uint64_t cnd = r.cost + r.mult;
				if (!ans || cnd < *ans) {
					ans = cnd;
				}
			}
		}
	}

	if (ans) {
		cout << *ans << '\n';
	} else {
		cout << "NIE" << '\n';
	}

	return 0;
}
