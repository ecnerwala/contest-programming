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
	int N, M; cin >> N >> M;
	vector<array<int, 2>> edges(M);
	for (auto& e : edges) { cin >> e[0] >> e[1]; e[0]--, e[1]--; }

	vector<int> C(N); for (auto& c : C) cin >> c;

	vector<bool> dir(M); // positive is forwards
	vector<vector<int>> adj(N);
	for (int z = 0; z < M; z++) {
		auto e = edges[z];
		if (C[e[0]] == C[e[1]]) {
			adj[e[0]].push_back(z);
			adj[e[1]].push_back(z);
		} else {
			dir[z] = (C[e[0]] > C[e[1]]);
		}
	}

	vector<bool> vis(N, false);
	vector<bool> in_stack(N, false);
	for (int st = 0; st < N; st++) {
		if (vis[st]) continue;
		std::y_combinator([&](auto self, int cur, int prvE) -> void {
			assert(!vis[cur]);
			vis[cur] = true;
			in_stack[cur] = true;
			for (int z : adj[cur]) {
				int nxt = edges[z][0] ^ edges[z][1] ^ cur;
				if (vis[nxt]) {
					if (z != prvE && in_stack[nxt]) {
						// go upwards
						dir[z] = (nxt == edges[z][1]);
					}
					continue;
				}
				dir[z] = (nxt == edges[z][1]);
				self(nxt, z);
			}
			in_stack[cur] = false;
		})(st, -1);
	}

	for (int z = 0; z < M; z++) {
		cout << (dir[z] ? "->" : "<-") << '\n';
	}

	return 0;
}

// All edges go from bigger to smaller
// Equal edges must be in the same SCC, so we can use dfs-tree
