#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

class MarriageAndSelectionChallenge {

public:

	string solve(string S) {
		int N = int(S.size());
		vector<array<int, 12>> nxt(N+1);
		nxt[N].fill(N);
		for (int i = N-1; i >= 0; i--) {
			nxt[i] = nxt[i+1];
			nxt[i][S[i]-'a'] = i;
		}

		vector<array<int, 1<<12>> dp(N);

		vector<array<int, 1<<12>> char_dp(12);
		for (int c = 0; c < 12; c++) char_dp[c].fill(0);

		array<int, 1<<12> any_dp;
		any_dp.fill(0);

		for (int i = N-1; i >= 0; i--) {
			dp[i].fill(-1);
			int c = S[i] - 'a';
			for (int m = 0; m < (1 << 12); m++) {
				if (m & (1 << c)) continue;
				dp[i][m|(1<<c)] = max(char_dp[c][m|(1<<c)], any_dp[m])+1;
				setmax(char_dp[c][m|(1<<c)], dp[i][m|(1<<c)]);
				setmax(any_dp[m|(1<<c)], dp[i][m|(1<<c)]);
			}
		}

		int i = 0, m = (1<<12)-1;
		int l = any_dp[m];
		string res = ""; res.reserve(l);
		int cur_char = -1;
		while (l > 0) {
			for (int c = 0; c < 12; c++) {
				if (!(c == cur_char || (m & (1 << c)))) continue;
				int j = nxt[i][c];
				if (j == N) continue;
				assert(dp[j][m|(1<<c)] <= l);
				if (dp[j][m|(1<<c)] != l) continue;
				m &= ~(1 << c);
				res += char('a'+c);
				l--;
				cur_char = c;
				i = j+1;
				break;
			}
		}
		return res;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::MarriageAndSelectionChallenge;

// vim:ft=cpp
