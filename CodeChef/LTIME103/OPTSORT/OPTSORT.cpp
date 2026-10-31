#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		std::vector<int> pref_max(N+1);
		pref_max[0] = -1;
		for (int i = 0; i < N; i++) {
			pref_max[i+1] = std::max(pref_max[i], A[i]);
		}
		std::vector<int> suff_min(N+1);
		suff_min[N] = int(1e9)+10;
		for (int i = N-1; i >= 0; i--) {
			suff_min[i] = std::min(suff_min[i+1], A[i]);
		}
		assert(pref_max[0] <= suff_min[0]);
		assert(pref_max[N] <= suff_min[N]);
		int64_t ans = 0;
		for (int i = 0, j; i < N; i = j) {
			j = i+1;
			while (!(pref_max[j] <= suff_min[j])) j++;
			ans += pref_max[j] - suff_min[i];
		}
		cout << ans << '\n';
	}

	return 0;
}
