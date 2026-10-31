#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t X, Y, Z; cin >> X >> Y >> Z;
	std::array<int64_t, 2> dp{0, Z};
	std::string S; cin >> S;
	for (char c : S) {
		int tgt = c == 'a' ? 0 : 1;
		dp[!tgt] += Y;
		dp[tgt] += X;
		dp[0] = std::min(dp[0], dp[1] + Z);
		dp[1] = std::min(dp[1], dp[0] + Z);
	}
	cout << std::min(dp[0], dp[1]) << '\n';

	return 0;
}
