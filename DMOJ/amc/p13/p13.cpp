#include <bits/stdc++.h>

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

struct mod_val {
	int v, m;
	friend mod_val operator & (mod_val a, mod_val b) {
		if (a.v > b.v) std::swap(a, b);
		return mod_val{a.v + int(int64_t(b.v - a.v) * mod_inv(a.m, b.m) % b.m) * a.m , a.m * b.m};
	}
};

int fact(int64_t N, int p, int e, int q) {
	if (e == 0) return 0;
	if (N == 0) return 1;
	int r = fact(N / p, p, e, q);
	for (int z = 1; z <= N % (2*q); z++) {
		if (z % p == 0) continue;
		r = int(int64_t(r) * int64_t(z) % q);
	}
	return r;
}

int ways(int64_t N, int64_t A, int p, int e, int q) {
	int64_t B = N - A;
	assert(A >= 0 && B >= 0);
	int num_p = 0;
	for (int64_t x = N, y = A, z = B, c = 0; x > 0; x /= p, y /= p, z /= p) {
		c = (x % p < (y % p + z % p + c));
		num_p += c;
	}
	num_p = std::min(num_p, e);

	int extra_p = 1;
	for (int z = 0; z < num_p; z++) { extra_p *= p, q /= p, e--; }

	int numer = fact(N, p, e, q);
	int denom = int(int64_t(fact(A, p, e, q)) * int64_t(fact(B, p, e, q)) % q);
	int r = int(int64_t(numer) * int64_t(mod_inv_in_range(denom, q)) % q) * extra_p;
	return r;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t N, M; int K; cin >> N >> M >> K;
	mod_val res{0, 1};
	for (int p = 2; K > 1; p++) {
		if (p * p > K) p = K;
		if (K % p == 0) {
			int q = 1;
			int e = 0;
			while (K % p == 0) {
				q *= p;
				e++;
				K /= p;
			}

			res = res & mod_val{ways(N, M, p, e, q), q};
		}
	}
	cout << res.v << '\n';

	return 0;
}
