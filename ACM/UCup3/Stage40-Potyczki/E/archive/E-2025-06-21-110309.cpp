#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int64_t> A(N);
	for (auto& a : A) cin >> a;

	std::vector<int> dp(N);

	for (int i = 0; i < N; i++) {
		for (int d = 1; d <= i && d <= 120; d++) {
			if (__builtin_popcountll(A[i] ^ A[i - d]) <= d) {
				dp[i] = std::max(dp[i], dp[i-d]);
			}
		}
		dp[i]++;
	}

	cout << N - *std::max_element(dp.begin(), dp.end()) << '\n';

	return 0;
}
