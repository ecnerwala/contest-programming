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

	int rt = 0;
	while (adj[rt].size() != 1) rt++;

	std::vector<int> res; res.reserve(N);
	std::y_combinator([&](auto self, int cur, int prv, int level) -> void {
		if (level % 3 == 1) {
			res.push_back(int(adj[cur].size()));
		}
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			self(nxt, cur, level+1);
		}
	})(rt, -1, 0);
	std::sort(res.begin(), res.end());
	for (int i = 0; i < int(res.size()); i++) {
		cout << res[i] << " \n"[i+1==int(res.size())];
	}
	return 0;
}
