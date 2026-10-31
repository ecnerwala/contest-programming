#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < M; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}

		std::vector<std::pair<int, int>> vals(N);
		for (int i = 0; i < N; i++) vals[i] = {A[i], i};
		std::sort(vals.begin(), vals.end());

		std::vector<int> alive(N, false);
		std::vector<int> par(N, -1);

		std::vector<int> uf_par(N, -1);
		auto get_uf_par = [&](int a) {
			while (uf_par[a] >= 0) {
				if (uf_par[uf_par[a]] >= 0) uf_par[a] = uf_par[uf_par[a]];
				a = uf_par[a];
			}
			return a;
		};

		std::vector<int> sz(N);
		std::vector<bool> bad(N, false);

		int num_merges = 0;

		for (auto [v, i] : vals) {
			sz[i] = 1;
			for (auto j : adj[i]) {
				if (!alive[j]) continue;
				j = get_uf_par(j);
				if (j == i) continue;
				num_merges++;
				par[j] = uf_par[j] = i;
				if (v > sz[j]) {
					bad[j] = true;
				}
				sz[i] += sz[j];
			}
			alive[i] = true;
		}
		if (num_merges != N-1) {
			cout << "NO" << '\n';
			continue;
		}
		bool good = false;
		for (int z = N-1; z >= 0; z--) {
			auto [v, i] = vals[z];
			if (par[i] == -1) {
				assert(z == N-1);
			} else {
				if (bad[par[i]]) bad[i] = true;
			}
			if (v == 0 && !bad[i]) {
				good = true;
			}
		}
		cout << (good ? "YES" : "NO") << '\n';
	}

	return 0;
}
