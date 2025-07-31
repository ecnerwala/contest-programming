#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> P(N);
		for (auto& p : P) cin >> p;
		int64_t ans = 0;
		for (int i = 0; i < N; i++) {
			int cost_lo = 0;
			int cost_hi = 0;
			for (int j = 0; j < N; j++) {
				if (P[j] > P[i]) {
					if (j < i) {
						cost_lo++;
					} else {
						cost_hi++;
					}
				}
			}
			ans += std::min(cost_lo, cost_hi);
		}
		cout << ans << '\n';
	}

	return 0;
}
