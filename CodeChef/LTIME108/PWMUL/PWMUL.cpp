#include <bits/stdc++.h>

namespace kactl {
typedef uint64_t ull;

ull M;
double divided;

ull modmul(ull a, ull b) {
	int64_t ret = a * b - M * ull(divided * a * b);
	return ret + M * (ret < 0) - M * (ret >= (int64_t)M);
}
ull modpow(ull b, ull e) {
	ull ans = 1;
	for (; e; b = modmul(b, b), e /= 2)
		if (e & 1) ans = modmul(ans, b);
	return ans;
}
}

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
	T vx = 1, vy = 0;
	while (x) {
		T k = y / x;
		y %= x;
		vy -= k * vx;
		std::swap(x, y);
		std::swap(vx, vy);
	}
	assert(y == 1);
	return vy < 0 ? m + vy : vy;
}

template <typename T> T mod_inv(T a, T m) {
	a %= m;
	a = a < 0 ? a + m : a;
	return mod_inv_in_range(a, m);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t P; cin >> P;
	int64_t A, B; cin >> A >> B;

	kactl::M = P;
	kactl::divided = 1. / P;

	int64_t order = P-1;
	assert(kactl::modpow(A, order) == 1);
	for (int64_t v = P-1, p = 2; v > 1; p++) {
		if (p * p > v) p = v;
		if (v % p == 0) {
			int e = 0;
			while (v % p == 0) {
				v /= p;
				e++;
			}
			for (int z = 0; z < e; z++) {
				if (kactl::modpow(A, order / p) == 1) {
					order /= p;
				} else {
					break;
				}
			}
		}
	}

	int64_t coset_size = (P-1) / order;
	cerr << "order = " << order << '\n';
	cerr << "coset_size = " << coset_size << '\n';
	if (order <= 60 * coset_size) {
		cerr << "case 1" << '\n';
		int64_t ans = B;
		int64_t v = B;
		for (int64_t i = 1; i < order; i++) {
			v = kactl::modmul(v, A);
			ans = std::min(ans, v);
		}
		cout << ans << '\n';
	} else {
		auto tgt = kactl::modpow(B, order);
		cerr << "case 2" << '\n';
		int64_t ans = 1;
		while (kactl::modpow(ans, order) != tgt) ans++;
		cout << ans << '\n';
	}

	return 0;
}
