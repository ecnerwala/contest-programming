#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, RK, CK, RD, CD; cin >> N >> RK >> CK >> RD >> CD;
		int64_t ans = 0;
		if (RD > RK) {
			ans += RD - 0;
		} else if (RD < RK) {
			ans += N - RD;
		}
		if (CD > CK) {
			ans += CD - 0;
		} else if (CD < CK) {
			ans += N - CD;
		}
		cout << ans << '\n';
	}

	return 0;
}
