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
		if (cnt[base] >= 2 && K == 1) {
			// do a single step
			int mex = base;
			while (cnt[mex]) mex++;
			for (int i = 0; i < N; i++) {
				cout << (A[i] < mex && cnt[A[i]] == 1 ? mex : A[i]) << " \n"[i+1==N];
			}
		} else {
			int val = (cnt[base] == 0) == (K % 2 == 0) ? base + 1 : base;
			for (int i = 0; i < N; i++) {
				cout << (A[i] < base ? A[i] : (val)) << " \n"[i+1==N];
			}
		}
	}

	return 0;
}
