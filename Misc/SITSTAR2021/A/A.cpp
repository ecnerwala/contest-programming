#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int x, y; cin >> x >> y;
		cout << min(20 * (x-1), 5 * (y-1 + x-1)) << '\n';
	}

	return 0;
}
