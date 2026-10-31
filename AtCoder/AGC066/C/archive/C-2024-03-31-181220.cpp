#include <bits/stdc++.h>

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		std::string S; cin >> S;
		int N = int(S.size());
		std::vector<int> balance(N+1);
		balance[0] = N;
		for (int i = 0; i < N; i++) {
			balance[i+1] = balance[i] + (S[i] == 'B' ? +2 : -1);
		}
		std::vector<int> prv_B(3*N+1, N+1);
		std::vector<int> prv_any(3*N+1, N+1);
		std::vector<int> dp(N+1, N+1);
		dp[0] = 0;
		for (int i = 0; i < N; i++) {
			if (S[i] == 'B') setmin(prv_B[balance[i]], dp[i]);
			setmin(prv_any[balance[i]], dp[i]);

			dp[i+1] = dp[i] + 1;
			if (S[i] == 'B') {
				setmin(dp[i+1], prv_any[balance[i+1]]);
			}
			setmin(dp[i+1], prv_B[balance[i+1]]);
		}
		cout << (N - dp[N]) / 3 << '\n';
	}

	return 0;
}
