#include <bits/stdc++.h>

int64_t mod_inv(int64_t a, int64_t m) {
	return a == 1 ? 1 : m - mod_inv(m % a, a) * m / a;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	assert(N % 6 == 0);
	N /= 6;
	std::vector<std::array<int, 3>> W(2*N);
	for (auto& [x, y, z] : W) cin >> x >> y >> z;
	const int64_t MOD = 998244353;
	int64_t ways = 3;
	for (auto& w : W) {
		std::sort(w.begin(), w.end());
		if (w[0] == w[2]) {
			ways *= 3;
		} else if (w[0] == w[1]) {
			ways *= 2;
		} else {
			ways *= 1;
		}
	}
	int64_t numer = 1;
	for (int i = 1; i <= 2 * N; i++) {
		numer *= i;
		numer %= MOD;
	}
	int64_t denom = 1;
	for (int i = 1; i <= N; i++) {
		denom *= i;
		denom %= MOD;
	}
	denom *= denom;
	denom %= MOD;
	ways *= numer;
	ways %= MOD;
	ways *= mod_inv(denom, MOD);
	ways %= MOD;
	cout << ways << '\n';

	return 0;
}
