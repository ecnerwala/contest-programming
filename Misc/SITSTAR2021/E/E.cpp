#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		int64_t X, Y; cin >> X >> Y;
		int64_t ans = 0;
		while (X) {
			ans += Y/X;
			Y %= X;
			swap(X, Y);
		}
		if (Y == 1) {
			ans--; // 1,1
		} else {
			ans = -1;
		}
		cout << ans << '\n';
	}

	return 0;
}
