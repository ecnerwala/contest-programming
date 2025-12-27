#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		std::string S; cin >> S;
		const int INF = 1e9;
		std::array<int, 2> dp{0, INF};
		for (char c : S) {
			if (c == 's') {
				dp = std::array<int, 2>{INF, std::min({dp[0], dp[1]})};
			} else if (c == 'u') {
				dp = std::array<int, 2>{dp[1], 1 + std::min({dp[0], dp[1]})};
			} else assert(false);
		}
		cout << dp[1] << '\n';
	}

	return 0;
}
