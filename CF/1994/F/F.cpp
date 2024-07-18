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

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<std::vector<int>> must_edges(N);
		std::vector<std::vector<int>> can_edges(N);
		for (int e = 0; e < M; e++) {
			int u, v, c; cin >> u >> v >> c; u--, v--;
			if (c == 0) {
				can_edges[u].push_back(v);
				can_edges[v].push_back(u);
			} else if (c == 1) {
				must_edges[u].push_back(v);
				must_edges[v].push_back(u);
			} else assert(false);
		}
		bool good = true;
		std::vector<bool> vis(N);
		for (int st = 0; st < N; st++) {
			if (vis[st]) continue;
			bool root_extra = std::y_combinator([&](auto self, int cur) -> bool {
				assert(!vis[cur]);
				vis[cur] = true;
				for (int nxt : can_edges[cur]) {
					if (!vis[nxt]) {
						bool has_extra = self(nxt);
						if (has_extra) {
							must_edges[nxt].push_back(cur);
							must_edges[cur].push_back(nxt);
						}
					}
				}
				return int(must_edges[cur].size()) & 1;
			})(st);
			if (root_extra) {
				good = false;
				break;
			}
		}
		if (good) {
			cout << "YES" << '\n';
		} else {
			cout << "NO" << '\n';
		}
	}

	return 0;
}
