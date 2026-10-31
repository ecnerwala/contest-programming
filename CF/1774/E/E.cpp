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

	int N, D; cin >> N >> D;
	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	std::vector<std::array<bool, 2>> needA(N, std::array<bool, 2>{false, false});
	for (int z = 0; z < 2; z++) {
		int ca; cin >> ca;
		while (ca--) {
			int v; cin >> v; v--;
			needA[v][z] = true;
		}
	}

	needA[0][0] = needA[0][1] = true;

	std::vector<int> stk; stk.reserve(N);
	std::y_combinator([&](auto self, int cur, int prv) -> void {
		stk.push_back(cur);
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			self(nxt, cur);
			for (int z = 0; z < 2; z++) {
				if (needA[nxt][z]) needA[cur][z] = true;
			}
		}
		stk.pop_back();
		if (int(stk.size()) >= D) {
			int anc = stk[int(stk.size()) - D];
			for (int z = 0; z < 2; z++) {
				if (needA[cur][z]) {
					needA[anc][!z] = true;
				}
			}
		}
	})(0, -1);
	int ans = 0;
	for (int i = 1; i < N; i++) {
		ans += needA[i][0];
		ans += needA[i][1];
	}
	cout << ans * 2 << '\n';

	return 0;
}
