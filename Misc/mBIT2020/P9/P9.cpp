#include <bits/stdc++.h>

int minv(int a, int m) {
	if (a <= 1) return a;
	return m - int(minv(m % a, a) * int64_t(m) / a);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int MOD = int(1e9)+7;
	int N, M; cin >> N >> M;

	int64_t ans = M;

	for (int z = 0; z < M; z++) {
		int C; cin >> C;
		// expected value of the max order statistic of C
		// (N+1)! * C / (C+1)
		ans -= minv(C+1, MOD);
		ans = (ans < 0 ? ans + MOD : ans);
	}

	for (int i = 1; i <= N+1; i++) ans = ans * i % MOD;

	cout << ans << '\n';

	return 0;
}
