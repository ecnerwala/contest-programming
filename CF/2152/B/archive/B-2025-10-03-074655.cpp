#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t N, RK, CK, RD, CD; cin >> N >> RK >> CK >> RD >> CD;
		int64_t dx = 0;
		if (RD > RK) {
			dx = RD - 0;
		} else if (RD < RK) {
			dx = N - RD;
		}
		int64_t dy = 0;
		if (CD > CK) {
			dy = CD - 0;
		} else if (CD < CK) {
			dy = N - CD;
		}
		int64_t ans = std::max(dx, dy);
		cout << ans << '\n';
	}

	return 0;
}
