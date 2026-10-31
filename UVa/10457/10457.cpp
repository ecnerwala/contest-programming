#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M;
	while (cin >> N >> M) {
		vector<pair<int, array<int, 2>>> edges(M);
		for (auto& it : edges) {
			int u, v, w; cin >> u >> v >> w;
			u--, v--;
			it = {w, {u, v}};
		}
		sort(edges.begin(), edges.end());

		int F0, F1; cin >> F0 >> F1;

		vector<int> par(N);
		auto reset = [&]() { fill(par.begin(), par.end(), -1); };
		auto get_par = [&](int a) -> int {
			while (par[a] >= 0) {
				if (par[par[a]] >= 0) par[a] = par[par[a]];
				a = par[a];
			}
			return a;
		};
		auto merge = [&](array<int, 2> e) -> bool {
			int a = e[0], b = e[1];
			a = get_par(a), b = get_par(b);
			if (a == b) return false;
			if (par[b] < par[a]) swap(a, b);
			par[a] += par[b];
			par[b] = a;
			return true;
		};

		int K; cin >> K;
		while (K--) {
			int source, dest; cin >> source >> dest; source--, dest--;
			int ans = edges.back().first - edges.front().first;
			for (int st = M-1; st >= 0; st--) {
				reset();
				for (int e = st; e < M; e++) {
					if (merge(edges[e].second) && get_par(source) == get_par(dest)) {
						ans = min(ans, edges[e].first - edges[st].first);
						break;
					}
				}
			}
			ans += F0 + F1;
			cout << ans << '\n';
		}
	}

	return 0;
}
