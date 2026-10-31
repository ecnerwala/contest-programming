#include<bits/stdc++.h>
using namespace std;

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
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	int N; cin >> N;
	vector<pair<int, pair<int, int>>> edges(N-1);
	for (int e = 0; e < N-1; e++) {
		int u, v, w; cin >> u >> v >> w; u--, v--;
		edges[e] = {w, {u, v}};
	}
	sort(edges.begin(), edges.end());

	vector<int> par(2*N-1, -1);
	auto getPar = std::y_combinator([&](auto self, int a) -> int {
		return par[a] < 0 ? a : (par[a] = self(par[a]));
	});

	vector<int> sz(2*N-1, 1);
	vector<pair<int, int>> ch(2*N-1);
	for (int e = 0; e < N-1; e++) {
		int u, v; tie(u,v) = edges[e].second;
		u = getPar(u);
		v = getPar(v);

		ch[e+N] = {u,v};

		sz[e+N] = sz[u] + sz[v];

		assert(par[u] == -1 && par[v] == -1);
		par[u] = par[v] = e+N;
	}

	int64_t ans = 0;
	int numE = N-1;
	int cur = 2*N-2;
	while (numE) {
		assert(cur >= N);
		int u, v; tie(u, v) = ch[cur];
		if (sz[u] > sz[v]) swap(u, v);
		assert(sz[u] <= sz[v]);
		int curE = min(numE, 2 * sz[u]);

		ans += int64_t(curE) << edges[cur - N].first;

		numE -= curE;
		cur = v;
	}
	cout << ans << '\n';

	return 0;
}
