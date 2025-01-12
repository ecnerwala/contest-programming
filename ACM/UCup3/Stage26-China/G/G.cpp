#include <bits/stdc++.h>

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
	// coeff of a in x and y
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

template <typename T> struct extended_gcd_result {
	T gcd;
	T coeff_a, coeff_b;
};
template <typename T> extended_gcd_result<T> extended_gcd(T a, T b) {
	T x = a, y = b;
	// coeff of a and b in x and y
	T ax = 1, ay = 0;
	T bx = 0, by = 1;
	while (x) {
		T k = y / x;
		y %= x;
		ay -= k * ax;
		by -= k * bx;
		std::swap(x, y);
		std::swap(ax, ay);
		std::swap(bx, by);
	}
	return {y, ay, by};
}

template <typename T> T mod_inv(T a, T m) {
	a %= m;
	a = a < 0 ? a + m : a;
	return mod_inv_in_range(a, m);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t A, B, N; cin >> A >> B >> N;
		int64_t coeff = mod_inv(-B, A);
		int64_t v = N % A == 0 ? N / A : N + B;
		cerr << "case " << A << ' ' << B << ' ' << N << '\n';
		bool good;
		while (true) {
			cerr << "testing " << v << '\n';
			if (v <= B) {
				v = N % B;
				if (v == 0) v = B;
			}
			int64_t nxt_v = (v % A) * coeff % A * B + v;
			assert(nxt_v % A == 0);
			if (v % B == N % B && v <= N && N <= nxt_v) {
				good = true;
				break;
			}
			if (v <= B) {
				good = false;
				break;
			}
			v = nxt_v / A;
		}
		cout << (good ? "Yes" : "No") << '\n';
	}

	return 0;
}
