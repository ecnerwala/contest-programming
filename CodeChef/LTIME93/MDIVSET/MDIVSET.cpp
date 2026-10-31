#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int V = 7500;
	vector<int> pfac(V+1);
	vector<int> primeId(V+1);
	vector<int> primes;
	for (int i = 2; i <= V; i++) {
		if (!pfac[i]) {
			primeId[i] = int(primes.size());
			primes.push_back(i);
			pfac[i] = i;
		}
		for (int p : primes) {
			if (p > V / i) break;
			pfac[i*p] = p;
			if (p == pfac[i]) break;
		}
	}

	int T; cin >> T;
	while (T--) {
		int N, C; cin >> N >> C;
		vector<int> A(N);
		for (auto& a : A) cin >> a;

		if (C <= 50) {
			vector<int> bad(1 << 15);
			for (int a : A) {
				int msk = 0;
				while (a > 1) {
					msk |= 1 << primeId[pfac[a]];
					a /= pfac[a];
				}
				bad[(1 << 15) - 1 - msk] = true;
			}
			int best = -1;
			for (int m = (1 << 15) - 1; m > 0; m--) {
				if (bad[m]) {
					for (int v = m; v; v &= v-1) {
						bad[m ^ (v & -v)] = true;
					}
				} else {
					if (best == -1 || __builtin_popcount(m) < __builtin_popcount(best)) best = m;
				}
			}
			assert(best != -1);
			vector<int> ans;
			for (int i = 0; i < 15; i++) {
				if (best & (1 << i)) ans.push_back(primes[i]);
			}
			cout << ans.size() << '\n';
			for (int i = 0; i < int(ans.size()); i++) {
				cout << ans[i] << " \n"[i+1==int(ans.size())];
			}
		} else {
			cout << 0 << '\n';
		}
	}

	return 0;
}
