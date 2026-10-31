#include <bits/stdc++.h>

int solve(int N, int X, int Y) {
	if (int g = std::gcd(X, Y); g != 1) {
		return solve(N / g + 1, X / g, Y / g) * (N % g) + solve(N / g, X / g, Y / g) * (g - N % g);
	}
	int P = X+Y;
	int ans = 0;
	for (int s : {0,1}) {
		std::array<int, 2> dp{-N, -N};
		dp[s] = 0;
		for (int i = 0; i < P; i++) {
			dp = {std::max(dp[0], dp[1]), dp[0] + (N / P) + (i * X % P < N % P)};
		}
		ans = std::max(ans, dp[s]);
	}
	return ans;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, X, Y; cin >> N >> X >> Y;
	cout << solve(N, X, Y) << '\n';

	return 0;
}
