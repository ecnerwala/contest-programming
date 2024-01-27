#include <bits/stdc++.h>

int solve(int N) {
	int R = 1;
	for (int p = 2; N > 1; p++) {
		if (int64_t(p) * p > N) p = N;
		if (N % p) continue;
		int e = 0;
		while (N % p == 0) { N /= p; e++; }
		if (p % 4 == 1) continue;
		if (p % 4 == 3 && e % 2 == 1) return 0;
		for (int z = 0; z < e; z++) R *= p;
	}
	return R;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int ans = solve(N);
		if (ans) cout << ans << '\n';
		else cout << "inf" << '\n';
	}

	return 0;
}
