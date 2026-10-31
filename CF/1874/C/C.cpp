#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int MAXN = 6000;
	std::vector<std::vector<double>> dp(MAXN);
	for (int N = 1; N < MAXN; N++) {
		if (N & 1) {
			dp[N].assign(N, double(1) / double(N));
		} else if (N == 0) {
			dp[N].assign(0, 0);
		} else {
			assert(N >= 2);
			dp[N].assign(N, 0);
			auto& prv = dp[N-2];
			auto& cur = dp[N];
			double denom = double(1) / double(N);
			cur[0] += denom;
			for (int i = 1; i < N; i++) {
				if (i-1 > 0) {
					cur[i] += (i-1) * denom * prv[i-2];
				}
				if (N-i-1 > 0) {
					cur[i] += (N-i-1) * denom * prv[i-1];
				}
			}
		}
	}

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		assert(N < MAXN);
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < M; e++) {
			int u, v; cin >> u >> v; u--, v--;
			assert(u < v);
			adj[u].push_back(v);
		}
		std::vector<double> vals(N);
		std::vector<double> buf; buf.reserve(N);
		vals[N-1] = 1;
		for (int i = N-2; i >= 0; i--) {
			buf.clear();
			for (int j : adj[i]) {
				buf.push_back(vals[j]);
			}
			std::sort(buf.begin(), buf.end(), std::greater<double>());
			const auto& cur = dp[buf.size()];
			double v = 0;
			for (int z = int(buf.size())-1; z >= 0; z--) {
				v += buf[z] * cur[z];
			}
			vals[i] = v;
		}
		cout << fixed << setprecision(12) << vals[0] << '\n';
	}

	return 0;
}
