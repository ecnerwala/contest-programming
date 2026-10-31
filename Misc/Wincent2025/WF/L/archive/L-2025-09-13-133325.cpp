#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	assert(N == 100);
	int ans = 0;
	int best = 10000;
	for (int x_lo = 0; x_lo >= -10; x_lo--) {
		for (int x_hi = 0; x_hi <= 10; x_hi++) {
			for (int y_lo = 0; y_lo >= -10; y_lo--) {
				for (int y_hi = 0; y_hi <= 10; y_hi++) {
					int cnt = 0;
					for (int x = x_lo; x <= x_hi; x++) {
						for (int y = y_lo; y <= y_hi; y++) {
							if (std::abs(std::gcd(x, y)) == 1) cnt++;
						}
					}
					int dx = x_hi - x_lo + 1;
					int dy = y_hi - y_lo + 1;
					if (cnt >= 100) {
						int area = dx * dy;
						if (area <= best) {
							best = area;
							cerr << x_lo << ".." << x_hi << ' ' << y_lo << ".." << y_hi << ' ' << cnt << ' ' << area << '\n';
						}
					}
				}
			}
		}
	}

	int x_lo = -4, x_hi = 9;
	int y_lo = -5, y_hi = 5;
	cout << N << ' ' << (x_hi - x_lo + 1) << ' ' << (y_hi - y_lo + 1) << '\n';
	int cnt = 0;
	for (int x = x_lo; x <= x_hi; x++) {
		for (int y = y_lo; y <= y_hi; y++) {
			if (std::gcd(x, y) == 1) {
				assert(x != 0 || y != 0);
				if (cnt < N) {
					cout << 0 << ' ' << x-x_lo << ' ' << y - y_lo << '\n';
					cnt++;
				}
			}
		}
	}
	assert(cnt >= N);
	for (int i = 0; i < N; i++) {
		cout << i << ' ' << 0-x_lo << ' ' << 0-y_lo << '\n';
	}

	return 0;
}
