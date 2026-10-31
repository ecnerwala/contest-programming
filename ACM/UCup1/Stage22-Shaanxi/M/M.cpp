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

	std::vector<std::vector<int>> ops(10);
	std::vector<int> sz(N);
	std::vector<bool> dead(N);
	std::y_combinator([&](auto self_centroid, int rt, int depth) -> void {
		assert(!dead[rt]);
		std::y_combinator([&](auto self, int cur, int prv) -> int {
			if (dead[cur]) {
				return sz[cur] = 0;
			}
			int res = 1;
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				res += self(nxt, cur);
			}
			return sz[cur] = res;
		})(rt, -1);
		int tot_sz = sz[rt];
		while (true) {
			bool found = false;
			for (int nxt : adj[rt]) {
				if (!dead[nxt] && sz[nxt] < sz[rt] && sz[nxt] * 2 >= tot_sz) {
					rt = nxt;
					found = true;
					break;
				}
			}
			if (!found) break;
		}

		//cerr << "depth " << depth << " " << "centroid " << rt << '\n';
		ops[depth].push_back(rt);
		dead[rt] = true;
		for (int nxt : adj[rt]) {
			if (dead[nxt]) continue;
			self_centroid(nxt, depth + 1);
		}
	})(0, 0);

	std::reverse(ops.begin(), ops.end());

	int num_ops = 0;
	for (const auto& o : ops) {
		num_ops += !o.empty();
	}
	cout << num_ops << '\n';
	for (const auto& o : ops) {
		if (o.empty()) continue;
		cout << o.size();
		for (int v : o) cout << ' ' << v+1;
		cout << '\n';
	}

	return 0;
}
