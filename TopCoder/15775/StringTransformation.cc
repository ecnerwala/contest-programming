#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class StringTransformation {

public:

	bool solve(string S, string T) {
		int N = int(S.size());
		int M = int(T.size());
		vector<vector<pair<int, int>>> ranges(N);
		for (int i = 0; i+1 < N; i++) {
			int maxNext = i+1;
			for (int j = i+3; j < N; j += 2) {
				if (S[j-2] != S[j-1]) break;
				maxNext = j;
			}

			int maxSame = i+1;
			while (maxSame <= maxNext && S[maxSame] == S[i]) maxSame += 2;
			char v1 = maxSame >= N ? '\0' : S[maxSame];
			while (maxSame <= maxNext && S[maxSame] == v1) maxSame += 2;

			int firstDiff = maxSame;
			while (firstDiff <= maxNext && (S[firstDiff] == S[i] || S[firstDiff] == v1)) firstDiff += 2;
			// [i+1, maxSame) is all good
			// [firstDiff, maxNext] is all good
			ranges[i].emplace_back(i+1, maxSame - 2);
			ranges[i].emplace_back(firstDiff, maxNext);
			//cerr << i << ' ' << i+1 << '-' << maxSame-2 << ' ' << firstDiff << '-' << maxNext << '\n';
		}

		vector<char> dp(N);
		vector<char> ndp(N);
		vector<int> pref(N+2);

		dp[0] = (S[0] == T[0]);
		for (int j = 0; j+1 < M; j++) {
			for (int i = 0; i < N; i++) {
				ndp[i] = 0;
			}

			if (T[j] == T[j+1]) {
				for (int i = 0; i+1 < N; i++) {
					if (!dp[i]) continue;
					assert(S[i] == T[j]);
					if (S[i+1] == T[j+1]) {
						ndp[i+1] = true;
					}
				}
			} else {
				for (int i = 0; i <= N+1; i++) pref[i] = 0;
				for (int i = 0; i+1 < N; i++) {
					if (!dp[i])  continue;
					for (auto it : ranges[i]) {
						pref[it.first] ++;
						pref[it.second + 2] --;
					}
				}
				for (int i = 0; i <= N-1; i++) {
					pref[i+2] += pref[i];
				}
				for (int i = 0; i < N; i++) {
					if (pref[i] && S[i] == T[j+1]) {
						ndp[i] = true;
					}
				}
			}
			swap(dp, ndp);
		}
		return dp[N-1];
	}

	string getResult(string S, string T) {
		bool ans = solve(S, T);
		return ans ? "YES" : "NO";
	}
};

void my_tests() {
	// FILL IN TESTS
	cerr << StringTransformation().getResult("RBBRRB", "RB") << '\n';
}

}  // namespace ecnerwala

using ecnerwala::StringTransformation;

// vim:ft=cpp
