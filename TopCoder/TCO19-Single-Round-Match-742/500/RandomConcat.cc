#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

const int MAXS = 70;
int S;
int nxt[MAXS][30];

const int MAXP = 17;
int P;
pair<int, int> nxtP[MAXP][MAXS];
double dp[1 << MAXP][MAXS];

class RandomConcat {

public:

	double expectation(vector <string> pieces, string needle) {
		S = 0;
		memset(nxt[0], 0, sizeof(nxt[0]));
		for (char c : needle) {
			int a = nxt[S][c-'a'];
			nxt[S][c-'a'] = S+1;
			S++;
			memcpy(nxt[S], nxt[a], sizeof(nxt[S]));
		}
		assert(S > 0);
		P = int(pieces.size());
		for (int p = 0; p < P; p++) {
			for (int s = 0; s <= S; s++) {
				int cur = s;
				int cnt = 0;
				for (char c : pieces[p]) {
					cur = nxt[cur][c-'a'];
					if (cur == S) {
						cnt++;
					}
				}
				nxtP[p][s] = {cur, cnt};
			}
		}

		memset(dp, 0, sizeof(dp));
		dp[0][0] = 1;
		double res = 0;
		for (int m = 0; m < (1 << P); m++) {
			int cnt = 0;
			for (int p = 0; p < P; p++) {
				if (m & (1 << p)) continue;
				cnt++;
			}

			for (int s = 0; s <= S; s++) {
				dp[m][s] /= cnt;
			}

			for (int p = 0; p < P; p++) {
				if (m & (1 << p)) continue;
				for (int s = 0; s <= S; s++) {
					res += dp[m][s] * nxtP[p][s].second;
					dp[m|(1<<p)][nxtP[p][s].first] += dp[m][s];
				}
			}
		}
		return res;
	}
};

}  // namespace ecnerwala

using ecnerwala::RandomConcat;

// vim:ft=cpp
