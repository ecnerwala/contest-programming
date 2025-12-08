#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::array<int, 2>> X(N);
		for (auto& [x, y] : X) cin >> x >> y;
		std::sort(X.begin(), X.end(), [&](auto a, auto b) { return a[0] + a[1] < b[0] + b[1]; });
		int ans = 0;
		int max_x = -1;
		int max_y = -1;
		for (auto [x, y] : X) {
			ans = std::max(ans, max_x - x);
			ans = std::max(ans, max_y - y);
			max_x = std::max(max_x, x);
			max_y = std::max(max_y, y);
		}
		cout << ans << '\n';
	}

	return 0;
}
