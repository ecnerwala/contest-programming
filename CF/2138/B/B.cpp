#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, Q; cin >> N >> Q;
		std::vector<int> A(N);
		for (auto& a : A) { cin >> a; a--; }
		std::array<std::vector<int>, 2> prvs;
		for (int z = 0; z < 2; z++) {
			prvs[z].resize(N);
			std::vector<std::pair<int, int>> stk; stk.reserve(N+1);
			stk.push_back({N, -1});
			for (int i = 0; i < N; i++) {
				while (stk.back().first < A[i]) stk.pop_back();
				prvs[z][i] = stk.back().second;
				stk.push_back({A[i], i});
			}
			std::ranges::reverse(A);
			for (auto& a : A) { a = N - 1 - a; }
		}
		std::ranges::reverse(prvs[1]);
		for (auto& a : prvs[1]) a = N-1-a;

		std::vector<int> bad(N, N);
		for (int i = 0; i < N; i++) {
			int l = prvs[0][i], r = prvs[1][i];
			assert(l <= i && i <= r);
			if (0 <= l && r <= N-1) {
				// l..r is bad
				bad[l] = std::min(bad[l], r);
			}
		}
		for (int i = N-2; i >= 0; i--) bad[i] = std::min(bad[i], bad[i+1]);
		for (int q = 0; q < Q; q++) {
			int l, r; cin >> l >> r; l--, r--;
			cout << (r >= bad[l] ? "NO" : "YES") << '\n';
		}
	}

	return 0;
}
