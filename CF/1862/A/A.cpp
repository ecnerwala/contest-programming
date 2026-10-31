#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<std::array<bool, 4>> dp(M, std::array<bool, 4>({false, false, false, false}));
		for (int i = 0; i < N; i++) {
			std::string s; cin >> s;
			for (int j = 0; j < M; j++) {
				char c = s[j];
				if (c == 'v') dp[j][0] = true;
				if (c == 'i') dp[j][1] = true;
				if (c == 'k') dp[j][2] = true;
				if (c == 'a') dp[j][3] = true;
			}
		}
		int cur = 0;
		for (int i = 0; i < M; i++) {
			if (cur < 4 && dp[i][cur]) cur++;
		}
		cout << (cur == 4 ? "YES" : "NO") << '\n';
	}

	return 0;
}
