#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t w, x, y, z; cin >> w >> x >> y >> z;
		w += y * z;
		if (w > x) {
			cout << "overflow" << '\n';
		} else if (w < x) {
			cout << "unfilled" << '\n';
		} else {
			cout << "filled" << '\n';
		}
	}

	return 0;
}
