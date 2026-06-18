#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	const int V = int(1e6);
	std::vector<int> primes; primes.reserve(V);
	std::vector<int> pfac(V+1);
	for (int i = 2; i <= V; i++) {
		if (!pfac[i]) {
			primes.push_back(i);
			pfac[i] = i;
		}
		for (int p : primes) {
			if (int64_t(i) * p > V) break;
			pfac[i*p] = p;
			if (p == pfac[i]) break;
		}
	}
	int cnt_smooth = 0;
	for (int i = 2; i <= V; i++) {
		int v = i;
		while (pfac[v] != v) v /= pfac[v];
		if (v < 150) cnt_smooth++;
	}
	std::cerr << cnt_smooth << '\n';
	return 0;
}
