#include <bits/stdc++.h>

int main() {
	int N; std::cin >> N;
	std::vector<int> C(N);
	std::cin >> C[0];
	std::vector<int> par(N);
	par[0] = -1;
	std::vector<int> depth(N);
	for (int i = 1; i < N; i++) {
		int a, u, v; std::cin >> a >> u >> v; u--, v--;
		par[i] = v;
		depth[i] = depth[v] + 1;
		if (depth[u] < depth[v]) std::swap(u, v);
		while (depth[u] > depth[v]) {
			a /= std::gcd(a, C[u]);
			u = par[u];
		}
		assert(depth[u] == depth[v]);
		while (u != v) {
			a /= std::gcd(a, C[u]); u = par[u];
			a /= std::gcd(a, C[v]); v = par[v];
		}
		assert(u == v);
		a /= std::gcd(a, C[u]);
		C[i] = a;
	}
	for (int i = 0; i < N; i++) {
		std::cout << C[i] << " \n"[i+1==N];
	}
}
