#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t N, D; cin >> N >> D;
		// (D+1)^sites >= N
		int64_t ans = 0;
		while (N > 1) {
			N = (N + D) / (D+1);
			ans++;
		}
		cout << ans << '\n';
	}

	return 0;
}
