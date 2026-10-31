#include <bits/stdc++.h>

template <typename T> using min_pq_t = std::priority_queue<T, std::vector<T>, std::greater<T>>;
template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }


int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	int64_t T; cin >> T;
	vector<int64_t> A(N); for (auto& a : A) cin >> a;
	vector<int64_t> prefA(N+1);
	for (int i = 0; i < N; i++) prefA[i+1] = prefA[i] + A[i];

	const int64_t INF = 1e18;
	vector<int64_t> dp(N+1, INF);
	dp[0] = 0;
	int64_t ans = INF;
	min_pq_t<pair<int64_t, int>> kill_val;
	min_pq_t<pair<int64_t, int>> nokill_val;
	vector<vector<pair<int64_t, int>>> nokill_delay(N+1);
	for (int i = 0, j = 0; i < N; i++) {
		while (!kill_val.empty() && kill_val.top().second <= i) kill_val.pop();
		if (!kill_val.empty()) {
			setmin(dp[i], prefA[i] + kill_val.top().first);
		}
		for (auto it : nokill_delay[i]) nokill_val.push(it);
		while (!nokill_val.empty() && nokill_val.top().second <= i) nokill_val.pop();
		if (!nokill_val.empty()) {
			setmin(dp[i], nokill_val.top().first);
		}

		while (j <= N && prefA[j] - prefA[i] <= T) j++;
		assert(j-1 > i);
		assert(prefA[j-1] - prefA[i] <= T);
		assert(j > N || prefA[j] - prefA[i] > T);
		if (j > N) {
			setmin(ans, dp[i]);
		} else {
			// we can explode
			setmin(dp[j], dp[i] + 2 * (prefA[j] - prefA[i]));
		}

		// at j, it expires
		kill_val.push({dp[i] - prefA[i], j});
		if (i+K+1 < j) {
			nokill_delay[i+K+1].push_back({dp[i], j});
		}
	}

	cout << ans << '\n';

	return 0;
}
