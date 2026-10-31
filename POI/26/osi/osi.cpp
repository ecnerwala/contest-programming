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
	vector<vector<pair<int, int>>> adj(N);
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back({2*e+0, v});
		adj[v].push_back({2*e+1, u});
	}

	string ans(M, '\0');

	vector<int> depth(N, -1);
	vector<int> lowval(N, -1);

	int num_scc = 0;
	for (int st = 0; st < N; st++) {
		if (depth[st] == -1) {
			depth[st] = 0;
			std::y_combinator([&](auto self, int cur, int prvE) -> void {
				lowval[cur] = depth[cur];
				for (auto [e, nxt] : adj[cur]) {
					if (e == (prvE ^ 1)) continue;
					if (depth[nxt] == -1) {
						ans[e>>1] = "><"[e&1];
						depth[nxt] = depth[cur] + 1;
						self(nxt, e);
						lowval[cur] = min(lowval[cur], lowval[nxt]);
					} else if (depth[nxt] < depth[cur]) {
						ans[e>>1] = "><"[e&1];
						lowval[cur] = min(lowval[cur], depth[nxt]);
					} else {
						// duplicate downedge
					}
				}
				if (lowval[cur] == depth[cur]) num_scc++;
			})(st, -1);
		}
	}

	cout << num_scc << '\n';
	cout << ans << '\n';

	return 0;
}
