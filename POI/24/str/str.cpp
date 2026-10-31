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
	vector<vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	vector<int> par(N);
	std::y_combinator([&](auto self, int cur, int prv) -> void {
		par[cur] = prv;
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			self(nxt, cur);
		}
	})(0, -1);

	vector<bool> onstrike(N);
	vector<int> chstrike(N);

	int num_cc = 1;

	int Q; cin >> Q;
	while (Q--) {
		int a; cin >> a;
		bool enter = a > 0;
		a = abs(a); a --;

		if (enter) {
			assert(!onstrike[a]);
			num_cc += int(adj[a].size())-1;
			onstrike[a] = true;
			num_cc -= chstrike[a];
			if (par[a] != -1) {
				chstrike[par[a]] ++;
				if (onstrike[par[a]]) num_cc--;
			}
		} else {
			assert(onstrike[a]);
			num_cc -= int(adj[a].size())-1;
			onstrike[a] = false;
			num_cc += chstrike[a];
			if (par[a] != -1) {
				chstrike[par[a]] --;
				if (onstrike[par[a]]) num_cc++;
			}
		}

		cout << num_cc << '\n';
	}

	return 0;
}
