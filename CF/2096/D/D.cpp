#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		int64_t MI = 1e10;
		int64_t a = 0;
		int64_t b = 0;
		int N; cin >> N;
		for (int i = 0; i < N; i++) {
			int64_t x, y; cin >> x >> y;
			x += MI;
			y += MI;
			a ^= x + y;
			b ^= x;
		}
		cout << b - MI << ' ' << (a - b) - MI << '\n';
	}

	return 0;
}
