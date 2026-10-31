#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	const double INF = 1e99;
	std::vector<double> dp(M+1, INF);
	dp[0] = 0;
	std::vector<double> ndp(M+1, INF);
	for (int t = 0; t < N; t++) {
		std::fill(ndp.begin(), ndp.end(), INF);
		for (int s = 0; s < M; s++) {
			if (dp[s] >= INF/2) continue;
			for (int v = (t+1==N ? M-s : 1); v+s <= M && v <= M / (N-t); v++) {
				ndp[s+v] = std::min(ndp[s+v], dp[s]+(double(s)/v));
			}
		}
		std::swap(dp, ndp);
	}
	cout << fixed << setprecision(12) << dp[M] * 2 + N << '\n';

	return 0;
}
