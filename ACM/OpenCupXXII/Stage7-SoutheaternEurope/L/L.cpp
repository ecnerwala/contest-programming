#include <bits/stdc++.h>

namespace kactl {
using ll = long long;
typedef unsigned long long ull;
ull modmul(ull a, ull b, ull M) {
	ll ret = a * b - M * ull(1.L / M * a * b);
	return ret + M * (ret < 0) - M * (ret >= (ll)M);
}
ull modpow(ull b, ull e, ull mod) {
	ull ans = 1;
	for (; e; b = modmul(b, b, mod), e /= 2)
		if (e & 1) ans = modmul(ans, b, mod);
	return ans;
}
bool isPrime(ull n) {
	if (n < 2 || n % 6 % 4 != 1) return (n | 1) == 3;
	ull A[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022},
	    s = __builtin_ctzll(n-1), d = n >> s;
	for (ull a : A) {   // ^ count trailing zeroes
		ull p = modpow(a%n, d, n), i = s;
		while (p != 1 && p != n - 1 && a % n && i--)
			p = modmul(p, p, n);
		if (p != n-1 && i != s) return 0;
	}
	return 1;
}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	using kactl::isPrime;

	std::vector<int64_t> basis; basis.reserve(64);
	int T; cin >> T;
	while (T--) {
		basis.clear();
		auto add_vec = [&](int64_t v) {
			for (auto x : basis) v = min(v, v ^ x);
			if (v) basis.push_back(v);
		};
		int64_t L, R; cin >> L >> R;
		for (int z = 0; z <= 20000 && L+z <= R; z++) {
			if (isPrime(L+z)) add_vec(L+z);
			if (isPrime(R-z)) add_vec(R-z);
		}
		std::mt19937_64 mt(48);
		std::uniform_int_distribution<int64_t> dist(L, R);
		for (int z = 0; z <= 20000; z++) {
			int64_t cnd = dist(mt);
			if (isPrime(cnd)) add_vec(cnd);
		}

		cout << (int64_t(1) << int(basis.size())) << '\n';
	}

	return 0;
}
