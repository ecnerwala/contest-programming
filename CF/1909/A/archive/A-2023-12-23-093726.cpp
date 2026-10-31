#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int min_x = 10000, max_x = -10000;
		int min_y = 10000, max_y = -10000;
		while (N--) {
			int x, y; cin >> x >> y;
			if (x < min_x) min_x = x;
			if (x > max_x) max_x = x;
			if (y < min_y) min_y = y;
			if (y > max_y) max_y = y;
		}
		cout << (((min_x >= 0) || (max_x <= 0) || (min_y >= 0) || (max_y <= 0)) ? "YES" : "NO") << '\n';
	}

	return 0;
}
