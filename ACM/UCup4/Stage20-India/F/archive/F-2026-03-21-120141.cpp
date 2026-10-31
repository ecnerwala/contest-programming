#include <bits/stdc++.h>

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, K; std::cin >> N >> K;
		std::vector<int> A(N);
		for (auto& a : A) std::cin >> a;

		K = N-K; // number of 0's
		N++;
		std::vector<int64_t> V(N);
		for (int i = 0; i+1 < N; i++) {
			V[i+1] = V[i] + A[i];
		}
		std::vector<int> prv_bigger(N);
		std::vector<int> nxt_bigger(N);
		{
			std::vector<std::pair<int64_t, int>> stk; stk.reserve(N);
			for (int i = 0; i < N; i++) {
				while (!stk.empty() && V[i] >= stk.back().first) {
					nxt_bigger[stk.back().second] = i;
					stk.pop_back();
				}
				prv_bigger[i] = stk.empty() ? -1 : stk.back().second;
			}
			while (!stk.empty()) {
				nxt_bigger[stk.back().second] = N;
				stk.pop_back();
			}
		}

		for (auto v : prv_bigger) { std::cerr << v << ' '; } std::cerr << '\n';
		for (auto v : nxt_bigger) { std::cerr << v << ' '; } std::cerr << '\n';

		const int64_t INF = 2e18;
		std::vector<int64_t> dp(N, INF);
		std::vector<int64_t> dp2(N, INF);
		dp[N-1] = 0;
		for (int z = 0; z < K; z++) {
			std::fill(dp2.begin(), dp2.end(), INF);
			// must do at least one move of either type
			for (int i = N-1; i >= 0; i--) {
				if (prv_bigger[i] != -1) {
					setmin(dp2[prv_bigger[i]], dp[i]);
					setmin(dp2[prv_bigger[i]], dp2[i]);
				}
			}
			for (int i = N-1; i >= 0; i--) {
				if (dp[i] != INF) dp[i] += V[i];
				if (dp2[i] != INF) dp2[i] += V[i];
			}
			for (int i = N-1; i >= 0; i--) {
				if (nxt_bigger[i] != N) {
					setmin(dp2[i], dp2[nxt_bigger[i]]);
					setmin(dp2[i], dp[nxt_bigger[i]]);
				}
			}
			for (int i = N-1; i >= 0; i--) {
				if (dp2[i] != INF) dp2[i] -= V[i];
			}
			std::swap(dp, dp2);
			for (auto v : dp) { std::cerr << v << ' '; } std::cerr << '\n';
		}
		std::cout << *std::min_element(dp.begin(), dp.end()) << '\n';
	}

	return 0;
}
