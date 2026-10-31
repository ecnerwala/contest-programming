#include <bits/stdc++.h>

int mod_inv(int a, int m) {
	if (a == 1) return 1;
	return m - int(int64_t(mod_inv(m % a, a)) * m / a);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<int> problems(N);
	for (int j = 0; j < M; j++) {
		string s; cin >> s;
		for (int i = 0; i < N; i++) {
			if (s[i] == 'H') problems[i] += (1 << j);
		}
	}

	std::vector<int> cnt(1<<M);
	for (int v : problems) cnt[v]++;

	const int MOD = int(1e9)+7;
	auto plus_eq = [&](int& a, int b) {
		a += b;
		a -= (a >= MOD ? MOD : 0);
	};

	std::vector<int> ways(N+1);
	ways[0] = 1;
	for (int i = 1; i <= N; i++) {
		ways[i] = int((1 + int64_t(ways[i-1]) * i) % MOD);
		if (ways[i] == 0) ways[i] = MOD;
	}

	std::vector<int> dp((M+1)<<M);
	dp[0] = 1;
	int ans = 0;
	for (int msk = 0; msk < (1 << M); msk++) {
		for (int cur = 0; cur <= M; cur++) {
			if (cur < M) {
				plus_eq(dp[msk * (M+1) + cur + 1], dp[msk * (M+1) + cur]);
				if (!(msk & (1 << cur))) {
					plus_eq(dp[(msk + (1 <<cur)) * (M+1) + cur + 1], dp[msk * (M+1) + cur]);
				}
			} else {
				// cur == M
				if (msk == (1 << M) - 1) {
					ans = int(int64_t(ways[cnt[msk]]) * dp[msk * (M+1) + cur] % MOD);
					continue;
				} else {
					int w = int(int64_t(ways[cnt[msk]] - 1) * dp[msk * (M+1) + cur] % MOD);
					for (int b = 0; b < M; b++) {
						if (!(msk & (1 << b))) {
							plus_eq(dp[(msk + (1 << b)) * (M+1) + b + 1], w);
						}
					}
				}
			}
		}
	}

	if (ans == 0) ans = MOD;
	ans--;
	cout << ans << '\n';

	return 0;
}
