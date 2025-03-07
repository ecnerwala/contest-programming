#include <bits/stdc++.h>

const int MOD = int(1e9)+7;
struct modint {
	int v;
	modint& operator += (modint o) {
		v += o.v;
		v -= (v >= MOD ? MOD : 0);
		return *this;
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::string S; cin >> S;
	assert(int(S.size()) == N);
	const int M = 20;
	// maximum value is 20 or so, definitely nothing of length >= 4
	std::vector<std::array<modint, M+1>> dp(1 << M);
	for (int msk = 0; msk < (1 << M); msk++) {
		for (int v = 0; v <= M; v++) dp[msk][v] = {0};
	}
	modint ans{0};
	for (char c : S) {
		dp[0][0] += modint{1};
		int d = c - '0';
		assert(d == 0 || d == 1);
		for (int msk = 0; msk < (1 << M); msk++) {
			for (int v = M; v >= 0; v--) {
				dp[msk][v] = {dp[msk][v>>1].v * (1 ^ (v & 1) ^ d)};
			}
		}
		for (int msk = (1 << M) - 1; msk >= 0; msk--) {
			for (int v = 1; v <= M; v++) {
				int a = msk | (1 << (v-1));
				if ((a & (a+1)) == 0) ans += dp[msk][v];
				dp[msk | (1 << (v-1))][0] += dp[msk][v];
			}
		}
	}

	cout << ans.v << '\n';

	return 0;
}
