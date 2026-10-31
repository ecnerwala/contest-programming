#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		// all equal numbers stay equal forever
		// everything goes to A[i] if cnt[A[i]] == 1 && A[i] < global mex else global mex
		// if permutation, then is fixed
		// this means that first thing with cnt != 1 becomes the new global mex
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		std::vector<int> cnt(N+1);
		for (int i = 0; i < N; i++) {
			cnt[A[i]]++;
		}
		int base = 0;
		while (cnt[base] == 1) base++;
		// the base things are fixed forever
		assert(cnt[base] != 1);
		if (base >= N-1) {
			assert(K >= 1);
			cout << int64_t(N) * int64_t(N-1) / 2 << '\n';
		} else if (cnt[base] >= 2 && K == 1) {
			// do a single step
			int mex = base;
			while (cnt[mex]) mex++;
			int64_t ans = 0;
			for (int i = 0; i < N; i++) {
				ans += (A[i] < mex && cnt[A[i]] == 1 ? mex : A[i]);
			}
			cout << ans << '\n';
		} else {
			int val = (cnt[base] == 0) == (K % 2 == 0) ? base + 1 : base;
			int64_t ans = 0;
			for (int i = 0; i < N; i++) {
				ans += (A[i] < base ? A[i] : (val));
			}
			cout << ans << '\n';
		}
	}

	return 0;
}
