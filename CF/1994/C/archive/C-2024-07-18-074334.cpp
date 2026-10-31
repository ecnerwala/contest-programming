#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, X; cin >> N >> X;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;

		std::vector<int> dp(N+1);
		int64_t cur = 0;
		for (int i = 0, j = 0; i < N; cur -= A[i++]) {
			while (j < N && cur <= X) cur += A[j++];
			// jump to j
			dp[j] += dp[i];
		}
		cout << std::accumulate(dp.begin(), dp.end(), int64_t(-N)) << '\n';
	}

	return 0;
}
