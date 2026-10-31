#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<int> H(N);
		for (auto& h : H) cin >> h;

		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < M; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}
		int Q; cin >> Q;
		struct query_t {
			int h;
			int u, v;
			int q;
		};
		std::vector<query_t> queries(Q);
		for (int q = 0; q < Q; q++) {
			int a, b, e; cin >> a >> b >> e; a--, b--;
			queries[q] = {H[a] + e, a, b, q};
		}
		std::sort(queries.begin(), queries.end(), [&](auto a, auto b) { return a.h < b.h; });
		std::vector<std::pair<int, int>> verts(N);
		for (int i = 0; i < N; i++) {
			verts[i] = {H[i], i};
		}
		std::sort(verts.begin(), verts.end());
		auto vit = verts.begin();
		std::vector<bool> alive(N);
		std::vector<bool> ans(Q);
		std::vector<int> par(N, -1);
		auto get_par = [&](int a) -> int {
			while (par[a] >= 0) {
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
		for (auto [h, u, v, q] : queries) {
			for (; vit != verts.end() && vit->first <= h; vit++) {
				int cur = vit->second;
				for (int nxt : adj[cur]) {
					if (!alive[nxt]) continue;
					merge(cur, nxt);
				}
				alive[cur] = true;
			}
			assert(alive[u]);
			ans[q] = alive[u] && alive[v] && (get_par(u) == get_par(v));
		}
		for (int q = 0; q < Q; q++) {
			cout << (ans[q] ? "YES" : "NO") << '\n';
		}
		if (T) cout << '\n';
	}

	return 0;
}
