#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<int> A(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		A[u]++;
		A[v]++;
	}

	for (int i = 0; i < N; i++) {
		assert(A[i] >= 1);
		A[i]--;
	}
	assert(std::accumulate(A.begin(), A.end(), 0) == N-2);

	std::sort(A.begin(), A.end());

	std::vector<int> dp(N-1, N+1);
	dp[0] = 0;

	vector<pair<int, int>> q(N);
	for (int z = 0; z < int(A.size()); ) {
		int v = A[z];
		int cnt = 0;
		while (z < int(A.size()) && A[z] == v) z++, cnt++;

		if (v == 0) continue;
		// now we have cnt copies of v

		for (int m = 0; m < v; m++) {
			// solve everything that's m mod v
			int lo = 0, hi = 0;
			for (int i = 0; m + i*v <= N-2; i ++) {
				int cnd = dp[m+i*v] - i;
				while (lo < hi && cnd <= q[hi-1].second) --hi;
				q[hi++] = {i, cnd};

				while (lo < hi && q[lo].first < i - cnt) lo++;
				if (lo < hi) {
					dp[m+i*v] = min(dp[m+i*v], q[lo].second + i);
				}
			}
		}
	}

	int64_t ans = 0;
	for (int y = 0; y <= N-2; y++) {
		int min_x = dp[y];
		if (y - min_x >= -1) {
			ans += (y - min_x) * 2 + 3;
		}
	}
	cout << ans << '\n';

	return 0;
}

// What do we know about this bag of numbers?
// Each number is at least 1, they sum to 2N-2 is necessary and sufficient
// Equivalently (for this), we have N nonnegative integers that sum to N-2
// y-x is a set of N numbers >= -1 with sum -2
// Note that sum(y-x) = -2, so WLOG, sum y-x >= -1 (multiply by 2)
// Consider the min x to attain each sum y, then we can always add 0's until sum y-x == -1
