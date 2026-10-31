#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::array<int, 2>> X(N);
		for (auto& [x, y] : X) cin >> x >> y;
		int ans = 0;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if (X[i][0] > X[j][0] && X[i][1] < X[j][1]) {
					ans = std::max(ans, std::min(X[i][0] - X[j][0], X[j][1] - X[i][0]));
				}
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
