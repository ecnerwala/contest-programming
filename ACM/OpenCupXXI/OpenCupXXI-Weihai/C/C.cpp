#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	const int V = 30000;
	vector<double> dp(V+1);
	vector<bool> is_prime(V+1, true);

	vector<double> ndp(V+1);
	for (int p = 2; p <= V; p++) {
		if (!is_prime[p]) continue;
		for (int j = p+p; j <= V; j += p) is_prime[j] = false;

		copy(dp.begin(), dp.end(), ndp.begin());
		for (int q = p; q <= V; q *= p) {
			double val = log(q);
			for (int i = q; i <= V; i++) {
				ndp[i] = max(ndp[i], dp[i-q] + val);
			}
		}
		std::swap(dp, ndp);
	}

	int Q; cin >> Q;
	while (Q--) {
		int b; cin >> b;
		cout << fixed << setprecision(9) << dp[b] << '\n';
	}

	return 0;
}
