#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		vector<int> A(N); for (auto& a : A) cin >> a;
		vector<int> pref(N+1);
		for (int i = 0; i < N; i++) pref[i+1] = pref[i] + A[i];

		int min_range = K;
		for (int i = 0; i+K <= N; i++) {
			min_range = min(min_range, pref[i+K] - pref[i]);
		}
		int64_t ans = int64_t(min_range) * int64_t(min_range + 1) / 2 + (pref[N] - min_range);
		cout << ans << '\n';
	}

	return 0;
}
