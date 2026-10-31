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
	vector<vector<int>> adj(N);
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
	}

	vector<int> ans(N);

	vector<int> stk; stk.reserve(N);
	vector<int> idx(N, -1);
	vector<int> lowval(N);
	int cur_idx = 0;
	for (int root = 0; root < N; root++) {
		if (idx[root] == -1) {
			std::y_combinator([&](auto self, int cur) -> void {
				assert(idx[cur] == -1);
				int st = int(stk.size());
				idx[cur] = cur_idx++;
				lowval[cur] = idx[cur];
				stk.push_back(cur);
				for (int nxt : adj[cur]) {
					if (idx[nxt] == -1) {
						self(nxt);
						lowval[cur] = min(lowval[cur], lowval[nxt]);
					} else if (idx[nxt] != -2) {
						lowval[cur] = min(lowval[cur], idx[nxt]);
					}
				}
				if (lowval[cur] == idx[cur]) {
					int en = int(stk.size());
					int val = 0;
					for (int z = st; z < en; z++) {
						val ++;
						for (int nxt : adj[stk[z]]) {
							val += ans[nxt];
						}
					}
					for (int z = st; z < en; z++) {
						ans[stk[z]] = val;
						idx[stk[z]] = -2;
					}
					stk.resize(st);
				}
			})(root);
			assert(stk.empty());
		}
	}
	assert(cur_idx == N);

	for (int i = 0; i < N; i++) {
		cout << ans[i]-1 << '\n';
	}

	return 0;
}
