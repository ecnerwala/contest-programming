#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false); std::cin.tie(nullptr);

	auto solve_min = [&](std::vector<int> A, int K) -> int64_t {
		int N = int(A.size());
		std::vector<std::pair<int64_t, int>> sums(1 << N);
		sums[0] = {0, 0};
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < (1 << i); j++) {
				sums[(1<<i) + j] = {sums[j].first + A[i], (1<<i) + j};
			}
		}
		std::sort(sums.begin(), sums.end());
		int mi = 0;
		int ma = int(sums.size());
		while (ma - mi > 1) {
			int md = (mi + ma) / 2;
			if ([&]() -> bool {
				int64_t thresh = sums[md].first;
				if (thresh > sums.back().first / K) return false;
				std::vector<std::pair<int, int64_t>> dp(1 << N);
				dp[0] = {0, 0};
				for (int m = 1; m < (1 << N); m++) {
					for (int i = 0; i < N; i++) {
						if (!((m >> i) & 1)) continue;
						auto cnd = dp[m - (1 << i)];
						cnd.second += A[i];
						if (cnd.second >= thresh) cnd.second = 0, cnd.first++;
						dp[m] = std::max(dp[m], cnd);
					}
				}
				return dp.back().first >= K;
			}()) {
				mi = md;
			} else {
				ma = md;
			}
		}
		return sums[mi].first;
	};

	auto solve_max = [&](std::vector<int> A, int K) -> int64_t {
		auto it = std::max_element(A.begin(), A.end());
		int fin = *it; A.erase(it);
		return solve_min(A, K) + fin;
	};

	int T; std::cin >> T;
	while (T--) {
		int N, K; std::cin >> N >> K;
		std::vector<int> A(N);
		for (auto& a : A) std::cin >> a;
		std::cout << solve_max(A, K) << '\n';
	}

	return 0;
}
