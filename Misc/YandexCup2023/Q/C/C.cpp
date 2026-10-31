#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int MOD = int(1e9)+7;
	int T; cin >> T;
	while (T--) {
		int64_t N; cin >> N;
		int ans = 0;
		int w = 1;
		for (int d = 0; N >> d; d++, w += w, w = (w >= MOD ? w - MOD : w)) {
			int64_t t = (N >> (d+1)) << d;
			if ((N >> d) & 1) {
				t += 1 + (N & ((int64_t(1) << d) - 1));
			}
			t %= MOD;
			ans = int((ans + (t * t) % MOD * w) % MOD);
		}
		cout << ans << '\n';
	}

	return 0;
}
