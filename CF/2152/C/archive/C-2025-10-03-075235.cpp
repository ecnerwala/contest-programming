#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, Q; cin >> N >> Q;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;

		std::vector<int> cnt_A(N+1);
		std::vector<int> cnt_checker(N+1);
		for (int i = 0; i < N; i++) {
			cnt_A[i+1] = cnt_A[i] + A[i];
			cnt_checker[i+1] = cnt_checker[i] + (A[i] ^ (i & 1));
		}
		for (int q = 0; q < Q; q++) {
			int l, r; cin >> l >> r; l--;
			if ((r - l) % 3 || (cnt_A[r] - cnt_A[l]) % 3) {
				cout << -1 << '\n';
			} else {
				int v = cnt_checker[r] - cnt_checker[l];
				v = std::min(v, (r - l) - v);
				cout << ((r - l) / 3 + !v) << '\n';
			}
		}
	}

	return 0;
}
