#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	while (true) {
		int64_t N; cin >> N;
		if (N == 0) break;

		int ans = 1;
		int64_t v = N;
		for (int64_t p = 2; v > 1; p++) {
			if (p * p > v) p = v;
			if (v % p == 0) {
				int e = 0;
				while (v % p == 0) { v /= p, e++; }
				ans *= 2 * e + 1;
			}
		}
		ans = (ans+1) / 2;
		cout << N << ' ' << ans << '\n';
	}

	return 0;
}

// N has p^e, then
// (0, e), (1, e), (2, e), ... (e-1, e)
// (e, 0), (e, 1), (e, 2), ... (e, e-1)
// (e, e)
