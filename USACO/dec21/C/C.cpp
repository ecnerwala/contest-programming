#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int MOD = int(1e9)+7;

	int N, X; cin >> N >> X;

	std::vector<int> pref(N+1);
	pref[0] = 1;
	for (int i = 1; i <= N; i++) {
		pref[i] = int(int64_t(pref[i-1]) * int64_t(i) % MOD);
	}

	std::vector<int> suff(N+1);
	suff[N] = 1;
	for (int i = N; i >= 1; i--) {
		suff[i-1] = int(int64_t(suff[i]) * int64_t(i) % MOD);
	}

	int ans = 0;
	for (int a0 = 0, b = X+1; b <= N; b++) {
		// cnt = N! / (b-a0) * (b - a0 - 1)
		int cnt = int(int64_t(suff[b-a0]) * int64_t(pref[b-a0-2]) % MOD);
		ans = int((ans + int64_t(cnt) * int64_t(X - a0)) % MOD);
	}

	for (int l = 2; l < N; l++) {
		// l = b - a0
		// cnt = N! / (b - a0 + 1) * (b-a0) * (b - a0 - 1)
		int cnt = int(int64_t(suff[l + 1]) * int64_t(pref[l-2]) % MOD);
		// 1 <= a0 <= X-1
		// X+1 <= a0 + l <= N
		int lo = std::max(1, X+1-l);
		int hi = std::min(X-1, N - l);
		if (lo <= hi) {
			// add X-a0 for lo <= a0 <= hi
			int mult = int(int64_t(hi - lo + 1) * int64_t((X - hi) + (X - lo)) / 2 % MOD);
			ans = int((ans + int64_t(cnt) * int64_t(mult)) % MOD);
		}
	}
	cout << ans << '\n';

	return 0;
}
