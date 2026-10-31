#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int x1, y1, x2, y2, x3, y3; cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
		cout << (x1 ^ x2 ^ x3) << ' ' << (y1 ^ y2 ^ y3) << '\n';
	}

	return 0;
}
