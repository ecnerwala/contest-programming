#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int INF = 1e9;

	int N; cin >> N;
	string S; cin >> S;
	std::array<int, 26> dp; dp.fill(INF);
	dp[S[0] - 'a'] = 0;
	for (int i = 0; i < int(S.size()); i++) {
		int best = INF;
		for (int v = 0; v < 26; v++) {
			if (v == S[i] - 'a') continue;
			best = min(best, dp[v] + 1);
		}
		if (i+1 < int(S.size())) {
			dp[S[i+1] - 'a'] = min(dp[S[i+1] - 'a'], best);
		} else {
			if (best > N) best = -1;
			cout << best << '\n';
		}
	}

	return 0;
}
