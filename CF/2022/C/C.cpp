#include <bits/stdc++.h>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::array<std::string, 2> S; cin >> S[0] >> S[1];
		assert(N % 3 == 0);
		const int D = 2 * N / 3;
		std::vector<std::array<int, 2>> dp(D+1, std::array<int, 2>{0, 0});
		auto eval = [&](char a, char b, char c) -> bool {
			return (a == 'A') + (b == 'A') + (c == 'A') >= 2;
		};
		for (int cur = 0; cur < D; cur++) {
			if (cur & 1) {
				int x = cur*3/2;
				for (int z = 0; z < 2; z++) {
					assert(cur+1 <= D);
					setmax(dp[cur+1][0], dp[cur][z] + eval(S[z][x], S[z][x+1], S[!z][x+1]));
					if (cur+2 <= D) {
						setmax(dp[cur+2][z], dp[cur][z] + eval(S[z][x], S[z][x+1], S[z][x+2]) + eval(S[!z][x+1], S[!z][x+2], S[!z][x+3]));
					}
				}
			} else {
				int x = cur*3/2;
				// this is evenly lined up, we can transition in 2 ways
				assert(cur + 2 <= D);
				setmax(dp[cur+2][0], dp[cur][0] + eval(S[0][x], S[0][x+1], S[0][x+2]) + eval(S[1][x], S[1][x+1], S[1][x+2]));
				setmax(dp[cur+1][0], dp[cur][0] + eval(S[0][x], S[1][x], S[1][x+1]));
				setmax(dp[cur+1][1], dp[cur][0] + eval(S[0][x], S[1][x], S[0][x+1]));
			}
		}
		cout << dp[D][0] << '\n';
	}

	return 0;
}
