#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t N, S, Q; cin >> N >> S >> Q;
	int64_t ans = 0;
	for (int64_t d = 0; d < N; d++) {
		int64_t r = 1;
		int64_t e = Q, a = d;
		while (e) {
			if (e & 1) r *= a, r %= N;
			a *= a, a %= N;
			e >>= 1;
		}
		if ((r + Q * d) % N == S) {
			ans += d;
		}
	}
	ans %= Q;
	cout << ans << '\n';

	return 0;
}
