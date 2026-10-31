#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K; K++;
	vector<int64_t> A(N+2);
	for (int i = 1; i <= N; i++) cin >> A[i];

	const int64_t INF = 2e18;

	vector<int64_t> dp(2*N+1, INF);
	dp[N] = 0;
	vector<int64_t> ndp(2*N+1);

	for (int i = 1; i <= N+1; i++) {
		for (int z = 0; z <= 2*N; z++) {
			ndp[z] = INF;
		}
		for (int z = 0; z <= 2*N; z++) {
			if (dp[z] == INF) continue;
			for (int dz = -1; dz <= 1; dz++) {
				int nz = (z + dz);
				if (0 <= nz && nz <= 2 * N) {
					ndp[nz] = min(ndp[nz], dp[z] + max(A[i] + dz * K - A[i-1], int64_t(0)));
				}
			}
		}
		swap(dp, ndp);
		//for (int z = 0; z <= 2 * N; z++) { cerr << dp[z] << ' '; } cerr << '\n';
	}
	cout << dp[N] << '\n';

	return 0;
}
