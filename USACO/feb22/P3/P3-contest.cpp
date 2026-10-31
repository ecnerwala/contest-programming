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
		std::array<int, 81> dp;
		dp.fill(0);
		dp[2] = 1;
		for (int i = 0; i < N; i++) {
			bool can_2 = [&]() -> bool {
				if (i-1 < 0) return false;
				std::array<char, 2> c = {S[i-1], S[i]};
				sort(c.begin(), c.end());
				return (c[1] - c[0] == 3) || (c[1] - c[0] == 1 && (c[0] - '0') % 3 != 0);
			}();
			bool can_4 = [&]() -> bool {
				if (i-3 < 0) return false;
				std::array<char, 4> c = {S[i-3], S[i-2], S[i-1], S[i]};
				sort(c.begin(), c.end());
				return c[1] - c[0] == 1 && c[2] - c[0] == 3 && c[3] - c[2] == 1 && (c[0] - '0') % 3 != 0;
			}();
			std::array<int, 81> ndp;
			ndp.fill(0);
			for (int j = 1; j < 81; j++) {
				if (dp[j] == 0) continue;
				add(ndp[(j * 3) % 81], dp[j]);
				int w = [&]() -> int {
					// How many ways are there to assign the suffix of digits so it matches?
					if (j % 3) {
						return 1;
					} else if (j / 3 % 3) {
						return 1;
					} else if (j / 9 % 3) {
						return 3;
					} else if (j / 27 % 3) {
						return 13;
					} else {
						// j > 0
						assert(false);
					}
				}();
				bool good = (j % 3 == 2) || (j / 3 % 3 == 2 && can_2) || (j / 27 % 3 == 2 && can_4);
				fma(ndp[(j * 3) % 81 + 1 + good], w, dp[j]);
			}
			dp = ndp;
		}

		int ans = 0;
		for (int i = 2; i < 81; i += 3) {
			add(ans, dp[i]);
		}
		cout << ans << '\n';
	}

	return 0;
}
