#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, W, K; cin >> N >> W >> K;

	std::vector<std::pair<int, int>> gems(N);
	for (auto& [w, v] : gems) cin >> w >> v;
	std::sort(gems.begin(), gems.end());

	std::vector<int64_t> pref_val(N+1);

	std::vector<int64_t> dp(W+1);
	pref_val[0] = 0;
	for (int i = 0; i < N; i++) {
		auto [w, v] = gems[i];
		for (int z = W; z >= w; z--) {
			dp[z] = std::max(dp[z], dp[z-w] + v);
		}
		pref_val[i+1] = dp[W];
	}

	std::vector<int64_t> suff_val(N+1);
	suff_val[N] = 0;

	int64_t cur_sum = 0;
	std::priority_queue<int, std::vector<int>, std::greater<int>> pq;
	for (int i = N-1; i >= 0; i--) {
		auto [w, v] = gems[i];
		cur_sum += v;
		pq.push(v);
		if (int(pq.size()) > K) {
			cur_sum -= pq.top();
			pq.pop();
		}
		assert(int(pq.size()) <= K);

		suff_val[i] = cur_sum;
	}

	int64_t ans = 0;
	for (int i = 0; i <= N; i++) {
		ans = std::max(ans, pref_val[i] + suff_val[i]);
	}
	cout << ans << '\n';

	return 0;
}
