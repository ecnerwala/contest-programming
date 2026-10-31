#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> H(N); for (auto& h : H) cin >> h;
		int ans = 0;
		for (int i = 0; i < N; i++) {
			ans = std::max(ans, H[i] + i);
		}
		cout << ans << '\n';
	}

	return 0;
}

// We want the longest path going right then up
