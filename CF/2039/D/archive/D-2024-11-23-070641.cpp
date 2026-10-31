#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<int> S(M); for (auto& s : S) cin >> s;

		std::vector<int> primes; primes.reserve(N);
		std::vector<int> pfac(N+1);
		std::vector<int> fac_cnt(N+1);
		fac_cnt[1] = 0;
		for (int i = 2; i <= N; i++) {
			if (!pfac[i]) {
				primes.push_back(i);
				pfac[i] = i;
				fac_cnt[i] = 1;
			}
			for (int p : primes) {
				if (int64_t(p) * int64_t(i) > N) break;
				pfac[i * p] = p;
				fac_cnt[i * p] = fac_cnt[i] + 1;
				if (p == pfac[i]) break;
			}
		}
		int x = *std::max_element(fac_cnt.begin(), fac_cnt.end());
		if (x >= M) {
			cout << -1 << '\n';
		} else {
			std::sort(S.begin(), S.end(), std::greater<>());
			for (int i = 1; i <= N; i++) {
				cerr << i << ' ' << fac_cnt[i] << '\n';
				cout << S[fac_cnt[i]] << " \n"[i==N];
			}
		}
	}

	return 0;
}

// in the i | j case, we need gcd(a_i, a_j) != a_i, so a_j is not a multiple of a_i
