#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class OrderlyString {

public:

	int longestLength(string s) {
		int dp[26];
		memset(dp, 0, sizeof(dp));
		for (char c : s) {
			int v = c - 'A';
			dp[v] ++;
			for (int i = v+1; i < 26; i++) {
				dp[i] = max(dp[i], dp[i-1]);
			}
		}
		return dp[25];
	}
};

void my_tests() {
	// FILL IN TESTS
	cerr << OrderlyString().longestLength("ZZZZZA") << '\n';
}

}  // namespace ecnerwala

using ecnerwala::OrderlyString;

// vim:ft=cpp
