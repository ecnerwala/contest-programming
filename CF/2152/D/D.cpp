#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, Q; cin >> N >> Q;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;

		std::vector<int64_t> cnt_ops(N+1);
		std::vector<int64_t> cnt_trade(N+1);
		for (int i = 0; i < N; i++) {
			cnt_ops[i+1] = cnt_ops[i] + (32 - __builtin_clz(A[i] - 1));
			cnt_trade[i+1] = cnt_trade[i] + (A[i] == 2 ? 1 : 32 - __builtin_clz(A[i] - 2));
		}

		for (int q = 0; q < Q; q++) {
			int l, r; cin >> l >> r; l--;
			int64_t ops = cnt_ops[r] - cnt_ops[l];
			int64_t trade = cnt_trade[r] - cnt_trade[l];
			cerr << trade << ' ' << ops << '\n';
			cout << (trade + ops) / 2 << '\n';
		}
	}

	return 0;
}
