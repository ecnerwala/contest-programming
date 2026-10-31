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
	std::vector<std::vector<std::tuple<int, int, char>>> adj(N);
	for (int e = 0; e < M; e++) {
		int u, v; char c; cin >> u >> v >> c;
		u--, v--;
		adj[u].push_back({e, v, c});
		adj[v].push_back({e, u, c});
	}

	std::string S; cin >> S;

	std::vector<bool> used(N);
	std::vector<int> par(N, -1);
	auto get_par = [&](int a) -> int {
		while(par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};
	auto merge = [&](int a, int b) -> bool {
		a = get_par(a);
		b = get_par(b);
		if (a == b) return false;
		if (par[a] > par[b]) std::swap(a, b);
		par[a] += par[b];
		par[b] = a;
		return true;
	};

	std::vector<int> ans; ans.reserve(N-1);

	auto dfs = std::y_combinator([&](auto self, int cur) -> void {
		assert(used[cur]);
		for (auto [e, nxt, c] : adj[cur]) {
			if (used[nxt]) continue;
			if (S[nxt] == c) {
				bool did_merge = merge(cur, nxt);
				assert(did_merge);
				used[nxt] = true;
				ans.push_back(e);
				self(nxt);
			}
		}
	});

	for (int i = 0; i < N; i++) {
		if (used[i]) continue;
		for (auto [e, j, c] : adj[i]) {
			if (used[j]) continue;
			if (S[i] == c && S[j] == c) {
				// start a tree from here
				// add edge e to the answer
				bool did_merge = merge(i, j);
				assert(did_merge);
				used[i] = true;
				used[j] = true;
				ans.push_back(e);
				dfs(i);
				dfs(j);
			}
		}
	}

	for (int i = 0; i < N; i++) {
		if (!used[i]) {
			cout << "No" << '\n';
			exit(0);
		}
	}

	for (int i = 0; i < N; i++) {
		for (auto [e, j, c] : adj[i]) {
			if (merge(i, j)) {
				ans.push_back(e);
			}
		}
	}
	assert(int(ans.size()) == N-1);

	std::sort(ans.begin(), ans.end());
	cout << "Yes" << '\n';
	for (int i = 0; i < N-1; i++) {
		cout << ans[i]+1 << " \n"[i+1==N-1];
	}

	return 0;
}
