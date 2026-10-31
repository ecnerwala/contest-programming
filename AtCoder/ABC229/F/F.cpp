#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;
	std::vector<int> B(N);
	for (auto& b : B) cin >> b;

	int64_t tot = std::accumulate(A.begin(), A.end(), int64_t(0)) + std::accumulate(B.begin(), B.end(), int64_t(0));

	const int64_t INF = 1e18;
	std::array<std::array<int64_t, 2>, 2> dp;
	dp[0][0] = 0;
	dp[0][1] = -INF;
	dp[1][0] = -INF;
	dp[1][1] = 0;

	for (int i = 0; i < N; i++) {
		for (int z = 0; z < 2; z++) {
			dp[z][1] += A[i];
			dp[z] = {std::max(dp[z][0], dp[z][1] + B[i]), std::max(dp[z][1], dp[z][0] + B[i])};
		}
	}

	cout << tot - std::max(dp[0][0], dp[1][1]) << '\n';

	return 0;
}
