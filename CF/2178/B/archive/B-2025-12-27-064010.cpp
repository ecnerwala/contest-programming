#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		std::string S; cin >> S;
		const int INF = 1e9;
		std::array<int, 3> dp{0, INF, INF};
		for (char c : S) {
			if (c == 's') {
				dp = std::array<int, 3>{INF, INF, std::min({dp[0], dp[1], dp[2]})};
			} else if (c == 'u') {
				dp = std::array<int, 3>{dp[1], dp[2], 1 + std::min({dp[0], dp[1], dp[2]})};
			} else assert(false);
		}
		cout << dp[2] << '\n';
	}

	return 0;
}
