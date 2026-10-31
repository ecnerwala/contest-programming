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
		int N; cin >> N;
		std::vector<std::array<int, 2>> edges(N-1);
		for (auto& [u, v] : edges) { cin >> u >> v; u--, v--; }
		std::vector<int> deg_orig(N);
		for (auto [u, v] : edges) { deg_orig[u]++, deg_orig[v]++; }
		int num_leaves = 0;
		for (int i = 0; i < N; i++) {
			if (deg_orig[i] == 1) {
				num_leaves++;
			}
		}
		if (num_leaves == 2) {
			cout << 1 << '\n';
			continue;
		}
		std::vector<int> deg(N);
		std::vector<std::vector<int>> adj(N);
		for (auto [u, v] : edges) {
			adj[u].push_back(v);
			adj[v].push_back(u);
			if (deg_orig[u] == 1 || deg_orig[v] == 1) continue;
			deg[u]++;
			deg[v]++;
		}
		int root = -1;
		std::y_combinator([&](auto self, int cur, int prv) -> void {
			if (deg[cur] >= 3) root = cur;
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				self(nxt, cur);
			}
		})(0, -1);
		if (root == -1) {
			cout << 2 << '\n';
			continue;
		}
		int oroot = -1;
		std::y_combinator([&](auto self, int cur, int prv) -> void {
			if (deg[cur] >= 3) oroot = cur;
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				self(nxt, cur);
			}
		})(root, -1);
		assert(oroot != -1);

		std::vector<int> path; path.reserve(N);
		std::y_combinator([&](auto self, int cur, int prv) -> bool {
			path.push_back(deg[cur]);
			if (cur == oroot) return true;
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				if (self(nxt, cur)) return true;
			}
			assert(path.back() == deg[cur]);
			path.pop_back();
			return false;
		})(root, -1);
		int num_big = 0;
		for (int i = 0; i < N; i++) num_big += deg[i] >= 3;
		int path_big = 0;
		for (int v : path) path_big += v >= 3;
		assert(path_big <= num_big);
		if (path_big < num_big) {
			cout << 3 << '\n';
			continue;
		}
		bool good = true;
		for (int i = 0; i < int(path.size()); i++) {
			if (path[i] > 4) {
				good = false;
			}
			if (path[i] == 4) {
				int j = i+1;
				while (j < int(path.size()) && path[j] != 4) j++;
				if (j < int(path.size())) {
					assert(path[j] == 4);
					bool found = false;
					for (int k = i; k <= j; k++) {
						assert(path[k] >= 2);
						if (path[k] == 2) found = true;
					}
					if (!found) good = false;
				}
			}
		}
		cout << (good ? 2 : 3) << '\n';
	}

	return 0;
}
