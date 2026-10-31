#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int MOD = int(1e9)+7;
	auto add = [&](int& a, int b) {
		a += b;
		if (a >= MOD) a -= MOD;
	};
	auto fma = [&](int& a, int b, int c) {
		a = int((a + int64_t(b) * int64_t(c)) % MOD);
	};

	int T; cin >> T;
	while (T--) {
		string S; cin >> S;
		int N = int(S.size());
		std::array<int, 32> dp;
		dp.fill(0);
		dp[3] = 1;
		for (int i = 0; i < N; i++) {
			bool can_2 = [&]() -> bool {
				if (i-1 < 0) return false;
				std::array<char, 2> c = {S[i-1], S[i]};
				std::sort(c.begin(), c.end());
				return (c[1] - c[0] == 3) || (c[1] - c[0] == 1 && (c[0] - '0') % 3 != 0);
			}();
			bool can_4 = [&]() -> bool {
				if (i-3 < 0) return false;
				std::array<char, 4> c = {S[i-3], S[i-2], S[i-1], S[i]};
				std::sort(c.begin(), c.end());
				return c[1] - c[0] == 1 && c[2] - c[0] == 3 && c[3] - c[2] == 1 && (c[0] - '0') % 3 != 0;
			}();
			std::array<int, 32> ndp;
			ndp.fill(0);
			for (int j = 1; j < 32; j++) {
				if (j == 16) continue;
				if (dp[j] == 0) continue;
				add(ndp[(j<<1) & 31], dp[j]);
				int w = std::array<int, 4>{1,1,3,13}[__builtin_ctz(j)];
				int nj = ((j & (j-1)) << 1) | 1;
				if ((nj & 4) || ((nj & 8) && can_2) || ((nj & 32) && can_4)) {
					nj |= 2;
				}
				fma(ndp[nj & 31], w, dp[j]);
			}
			dp = ndp;
		}

		int ans = 0;
		for (int i = 3; i < 32; i += 4) {
			add(ans, dp[i]);
		}
		cout << ans << '\n';
	}

	return 0;
}
