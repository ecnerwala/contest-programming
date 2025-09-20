#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int x, y; cin >> x >> y;
		assert(x > 0 && y > 0);
		if (x < y) {
			cout << 2 << '\n';
		} else if (x - y <= 1) {
			cout << -1 << '\n';
		} else {
			cout << 3 << '\n';
		}
	}

	return 0;
}
