#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> P(N+1);
		P[0] = 0;
		for (int i = 1; i <= N; i++) cin >> P[i];
		std::vector<int> S(N+1);
		for (int i = 0; i < N; i++) cin >> S[i];
		S[N] = 0;

		cout << ([&]() -> bool {
			for (int i = 0; i < N; i++) {
				int dp = P[i+1] - P[i];
				if (dp != 0 && dp != 1) return false;
				int ds = S[i] - S[i+1];
				if (ds != 0 && ds != 1) return false;
			}
			if (P[N] != S[0]) return false;

			for (int i = 0; i < N; i++) {
				if (P[i+1] + S[i] < P[N] + 1) return false;
			}
			return true;
		}() ? "YES" : "NO") << '\n';
	}

	return 0;
}
