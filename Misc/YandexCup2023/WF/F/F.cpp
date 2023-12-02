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

	int N, M, K; cin >> N >> M >> K;
	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	// just peel off leaves
	//assert(K == N);
	assert(N >= 2);
	std::vector<bool> vis(N);
	std::vector<std::vector<int>> ch(N);
	bool root_has = std::y_combinator([&](auto self, int cur) -> bool {
		assert(!vis[cur]);
		vis[cur] = true;
		for (int nxt : adj[cur]) {
			if (vis[nxt]) continue;
			bool has = self(nxt);
			if (has) {
				assert(ch[nxt].empty());
				ch[cur].push_back(nxt);
			}
		}
		return ch[cur].empty();
	})(0);
	if (root_has) {
		assert(!ch[adj[0][0]].empty());
		ch[adj[0][0]].push_back(0);
	}
	int num_ans = 0;
	for (int i = 0; i < N; i++) if (!ch[i].empty()) num_ans++;
	cout << num_ans << '\n';
	for (int i = 0; i < N; i++) if (!ch[i].empty()) {
		cout << ch[i].size() << ' ' << i+1;
		for (int v : ch[i]) {
			cout << ' ' << v+1;
		}
		cout << '\n';
	}

	return 0;
}
