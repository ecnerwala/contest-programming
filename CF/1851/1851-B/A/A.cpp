#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M, K, H; cin >> N >> M >> K >> H;
		auto solve = [&](int h) -> bool {
			int diff = std::abs(H - h);
			if (diff == 0) return false;
			if (diff > K * (M-1)) return false;
			return diff % K == 0;
		};
		int ans = 0;
		for (int i = 0; i < N; i++) {
			int h; cin >> h;
			ans += solve(h);
		}
		cout << ans << '\n';
	}

	return 0;
}
