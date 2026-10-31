#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M, K, H; cin >> N >> M >> K >> H;
		int ans = 0;
		for (int i = 0; i < N; i++) {
			int h; cin >> h;
			int d = h - H;
			ans += d != 0 && d % K == 0 && -K * (M-1) <= d && d <= K * (M-1);
		}
		cout << ans << '\n';
	}

	return 0;
}
