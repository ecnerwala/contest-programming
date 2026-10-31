#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		vector<int> A(N); for (auto& a : A) { cin >> a; a--; }

		vector<int> last_idx(N, -1);

		vector<int> dp(N+1);
		dp[0] = 0;
		for (int i = 0; i < N; i++) {
			// we can delete this guy
			dp[i+1] = dp[i] + 1;
			if (last_idx[A[i]] != -1) {
				dp[i+1] = min(dp[i+1], dp[last_idx[A[i]]] + (i - last_idx[A[i]] - 1));
			}
			last_idx[A[i]] = i;
		}
		cout << dp[N] << '\n';
	}

	return 0;
}

// 2 2 1 2 1 2 2
// Optimal: 2 2 1 1 2 2
// Delete aba algorithm: 2 2 2 2
//
// If we had 22222 and we wanted to insert a 2
// We can just delete instead 2222
