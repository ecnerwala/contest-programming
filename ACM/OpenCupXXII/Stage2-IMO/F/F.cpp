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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int P, Q; cin >> P >> Q;
	while (Q--) {
		int a, b, c, d; cin >> a >> b >> c >> d;
		int s1 = a + b; s1 = (s1 >= P ? s1 - P : s1);
		int s2 = c + d; s2 = (s2 >= P ? s2 - P : s2);
		if (s1 != s2) {
			cout << -1 << '\n';
		} else {
			assert(s1 != 0);
			int f = mod_inv_in_range(P-s1, P);
			a = int(int64_t(a) * int64_t(f) % P);
			c = int(int64_t(c) * int64_t(f) % P);
			int ans = 0;
			while (true) {
				int dist = c - a;
				dist = (dist < 0) ? dist + P : dist;
				if (dist < (int64_t(1) << ans)) {
					break;
				}
				ans++;
				a <<= 1;
				if (a >= P) a -= P;
			}
			cout << ans << '\n';
		}
	}

	return 0;
}
