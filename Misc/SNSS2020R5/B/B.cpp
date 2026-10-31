#include<bits/stdc++.h>

std::array<std::map<std::pair<int64_t, std::string>, int64_t>, 6> mem;

// number of things 1 <= v <= M
int64_t solve(int64_t M, int K, std::string suff = "") {
	if (M == 0) return 0;

	assert(int(suff.size()) <= K);
	if (int(suff.size()) == K) suff = suff.substr(1);

	// number of things < M
	if (mem[K].count({M, suff})) {
		return mem[K][{M, suff}];
	}
	int64_t ans = 0;
	for (int d = 0; d <= 9 && d <= M; d++) {
		if (suff.find(char('0'+d)) != std::string::npos) {
			continue;
		}

		int64_t nM = (M - d) / 10;
		if (d) {
			// add nothing
			ans++;
		}
		ans += solve(nM, K, suff + char('0'+d));
	}
	return mem[K][{M, suff}] = ans;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t L, R; int K; cin >> L >> R >> K;
		cout << solve(R, K) - solve(L-1, K) << '\n';
	}

	return 0;
}
