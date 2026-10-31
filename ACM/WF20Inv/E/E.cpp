#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int H, P; cin >> H >> P;
		int64_t tot = (int64_t(1) << H) - 1;
		int64_t avail = 1;
		int64_t ans = 0;
		while (tot > 0 && avail < P) {
			tot -= avail;
			avail *= 2;
			ans++;
		}
		ans += (tot + P-1) / P;
		cout << ans << '\n';
	}

	return 0;
}
