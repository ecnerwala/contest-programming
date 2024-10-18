#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t A, B, C; cin >> A >> B >> C;
	auto get_cnt = [&](int64_t k) -> int64_t {
		int64_t res = k / A / (B / std::gcd(A, B)) + k / B / (C / std::gcd(B, C)) + k / C / (A / std::gcd(C, A));
		int64_t x = A, y = B / std::gcd(A, B);
		int64_t z = C;
		// lcm(x * y, z)
		z /= std::gcd(x, z);
		z /= std::gcd(y, z);
		res -= 2 * k / x / y / z;
		assert(res >= 0);
		return res;
	};
	for (int i = 0; i <= 50; i++) {
		cerr << i << ' ' << get_cnt(i) << '\n';
	}

	int64_t N; cin >> N;
	int64_t MX = int64_t(1e18);
	int64_t lo = -1;
	int64_t hi = MX + 1;
	while (hi - lo > 1) {
		int64_t md = lo + (hi - lo) / 2;
		if (get_cnt(md) >= N) hi = md;
		else lo = md;
	}
	if (hi > MX) cout << -1 << '\n';
	else cout << hi << '\n';

	return 0;
}
