#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;
	std::vector<uint64_t> A(N);
	for (auto& a : A) cin >> a;

	std::vector<std::array<int, 2>> queries(Q);
	for (int q = 0; q < Q; q++) {
		int l, r; cin >> l >> r; l--;
		queries[q] = {l, r};
	}
	std::vector<uint64_t> ans(Q);

	std::vector<std::vector<std::pair<int, int>>> by_left(N);
	for (int q = 0; q < Q; q++) {
		auto [l, r] = queries[q];
		by_left[l].push_back({r, q});
	}

	std::vector<uint64_t> dp(N+1);
	for (int i = N-1; i >= 0; i--) {
		uint64_t cur_xor = 0;
		for (int j = i+1; j <= N; j++) {
			cur_xor ^= (A[i] + A[j-1]);
			dp[j] ^= cur_xor;
		}
		for (auto [r, q] : by_left[i]) {
			ans[q] = dp[r];
		}
	}

	for (int q = 0; q < Q; q++) {
		cout << ans[q] << '\n';
	}
	return 0;
}
