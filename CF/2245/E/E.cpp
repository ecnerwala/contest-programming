#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<std::array<int, 2>> edges(N-1);
		for (auto& [u, v] : edges) {
			std::cin >> u >> v; u--, v--;
		}
		std::vector<int> deg(N);
		for (auto vs : edges) for (auto v : vs) deg[v] ^= 1;
		// middle ones must all be even, endpoints must be odd
		const int V = N + (N-1);
		std::vector<int> par(V, -1);
		auto get_par = [&](int a) -> int {
			while (par[a] >= 0) {
				if (par[par[a]] >= 0) par[a] = par[par[a]];
				a = par[a];
			}
			return a;
		};
		std::vector<std::array<int, 2>> cnt(V, {0, 0});
		auto merge = [&](int a, int b) -> bool {
			a = get_par(a), b = get_par(b);
			if (a == b) return false;
			if (par[a] > par[b]) std::swap(a, b);
			par[a] += par[b];
			par[b] = a;
			cnt[a][0] += cnt[b][0];
			cnt[a][1] += cnt[b][1];
			return true;
		};
		for (int i = 0; i < N; i++) {
			cnt[i] = {1, 0};
		}
		for (int e = 0; e < N-1; e++) {
			cnt[N+e] = {0, 1};
		}

		for (int e = 0; e < N-1; e++) {
			for (int v : edges[e]) {
				if (deg[v] == 0) {
					merge(N+e, v);
				}
			}
		}

		int64_t ans = 0;
		for (int v = 0; v < V; v++) {
			if (par[v] < 0) {
				int extra_edges = cnt[v][1] + 1 - cnt[v][0];
				ans += int64_t(extra_edges) * int64_t(extra_edges - 1) / 2;
			}
		}
		std::cout << ans << '\n';
	}

	return 0;
}
