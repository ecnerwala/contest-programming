#include <bits/stdc++.h>

template <typename T> std::pair<T, T> gcd_and_mod_inv_in_range(T a, T m) {
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
	//assert(y == 1);
	return {y, vy < 0 ? m + vy : vy};
}


int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int W, MOD, D; cin >> W >> MOD >> D;
		std::vector<int> X(W);
		for (auto& x : X) { cin >> x; x--; }

		auto [g, k] = gcd_and_mod_inv_in_range(D, MOD);
		int mg = MOD / g;

		bool good = true;
		int64_t ans = 0;
		for (int i = 0; i < W-1-i; i++) {
			int v = X[i] - X[W-1-i];
			if (v < 0) v += MOD;
			if (v % g != 0) {
				good = false;
				break;
			}
			int ops = int(int64_t(v / g) * int64_t(k) % mg);
			ops = std::min(ops, mg - ops);
			ans += ops;
		}

		cout << "Case #" << case_num << ": ";
		if (good) {
			cout << ans << '\n';
		} else {
			cout << "IMPOSSIBLE" << '\n';
		}
	}

	return 0;
}
