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
	const int NUM_P = 150;
	const int MAX_P = primes[NUM_P-1];
	std::vector<int> smooth; smooth.reserve(V);
	smooth.push_back(1);
	for (int z = 0; z < int(smooth.size()); z++) {
		int i = smooth[z];
		for (int p : primes) {
			if (p > MAX_P) break;
			if (int64_t(i) * p > V) break;
			smooth.push_back(i*p);
			if (p == pfac[i]) break;
		}
	}
	std::cerr << smooth.size() << '\n';
	return 0;
}
