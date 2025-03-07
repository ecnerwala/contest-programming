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
	const int M = 5;
	// maximum value is 20 or so, definitely nothing of length >= 4
	std::vector<std::array<modint, M+1>> dp(1 << M);
	std::vector<std::array<modint, M+1>> ndp(1 << M);
	for (int msk = 0; msk < (1 << M); msk++) {
		for (int v = 0; v <= M; v++) dp[msk][v] = {0};
	}
	dp[0][0] = {1};
	for (char c : S) {
		int d = c - '0';
		assert(d == 0 || d == 1);
		for (int msk = 0; msk < (1 << M); msk++) {
			for (int v = 0; v <= M; v++) {
				ndp[msk][v] = {dp[msk][v>>1].v * (1 ^ (v & 1) ^ d)};
			}
		}
		for (int msk = (1 << M) - 1; msk >= 0; msk--) {
			for (int v = 1; v <= M; v++) {
				ndp[msk | (1 << (v-1))][0] += ndp[msk][v];
			}
		}
		std::swap(dp, ndp);
	}

	modint ans{0};
	for (int m = 1; m <= M; m++) {
		ans += dp[(1 << m) - 1][0];
	}
	cout << ans.v << '\n';

	return 0;
}
