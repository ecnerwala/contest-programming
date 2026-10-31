#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false); std::cin.tie(nullptr);
	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) { std::cin >> a; }
		std::vector<int> B(N);
		for (auto& b : B) { std::cin >> b; }
		int64_t best = -1;
		int best_ways = 0;

		const int MOD = int(1e6) + 3;
		// we need some really big factorials
		std::vector<int> fact(MOD);
		std::vector<int> ifact(MOD);
		fact[0] = 1;
		for (int i = 1; i <= MOD-1; i++) fact[i] = int(int64_t(fact[i-1]) * i % MOD);
		assert(fact[MOD-1] == MOD-1);
		ifact[MOD-1] = MOD-1;
		for (int i = MOD-1; i >= 1; i--) ifact[i-1] = int(int64_t(ifact[i]) * i % MOD);

		for (int ops = 0; true; ops++) {
			bool bad = false;
			int64_t extra = ops;
			std::vector<int64_t> cnt(ops+1);
			int64_t ways = 1;
			for (int i = 0; i < N; i++) {
				int a = A[i], b = B[i];
				if ((a << ops) > b) {
					bad = true;
					break;
				}
				int d = b - (a << ops);
				for (int z = 0; z < ops; z++) {
					cnt[(d >> z) & 1]++;
				}
				cnt[ops] += (d >> ops);
				ways *= ifact[d >> ops];
				ways %= MOD;
				// we need to count the number of ops of each type
			}
			if (bad) break;
			for (auto v : cnt) {
				if (v >= MOD) ways = 0;
				else ways *= fact[v], ways %= MOD;
			}
			if (best == -1 || extra < best) {
				best = extra;
				best_ways = 0;
			}
			if (best == extra) {
				best_ways += ways;
				best_ways %= MOD;
			}
		}
		std::cout << best << ' ' << best_ways << '\n';
	}
}
