#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		std::string S; cin >> S;
		std::vector<int> pref(N+1);
		for (int i = 0; i < N; i++) {
			bool good = true;
			for (int j = 0; j < 7; j++) {
				if (S[(i+j)%N] != "nanjing"[j]) {
					good = false;
					break;
				}
			}
			pref[i+1] = pref[i] + good;
		}
		int ans = 0;
		if (N >= 7) {
			for (int i = 0; i < N && i <= K; i++) {
				// pref from i .. N+i-6
				int hi = N+i-6;
				int lo = i;
				assert(hi > lo);
				assert(hi < 2*N);
				if (hi >= N) {
					ans = std::max(ans, pref[hi - N] + pref[N] - pref[lo]);
				} else {
					ans = std::max(ans, pref[hi] - pref[lo]);
				}
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
