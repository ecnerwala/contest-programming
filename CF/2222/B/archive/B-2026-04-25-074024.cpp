#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, M; std::cin >> N >> M;
		std::array<std::vector<int>, 2> A;
		A[0].reserve((N+1)/2);
		A[1].reserve((N+0)/2);
		for (int i = 0; i < N; i++) {
			int a; std::cin >> a;
			A[i&1].push_back(a);
		}
		std::array<int, 2> cnt{0, 0};
		for (int j = 0; j < M; j++) {
			int x; std::cin >> x; x--;
			cnt[x&1]++;
		}

		// mark up to cnt[i] elements
		int64_t ans = 0;
		for (int z = 0; z < 2; z++) {
			std::sort(A[z].begin(), A[z].end());
			if (cnt[z] > 0) {
				assert(!A[z].empty());
				cnt[z]--;
				A[z].pop_back();
				while (cnt[z] && !A[z].empty() && A[z].back() > 0) {
					cnt[z]--;
					A[z].pop_back();
				}
			}
			for (auto a : A[z]) ans += a;
		}
		std::cout << ans << '\n';
	}

	return 0;
}
