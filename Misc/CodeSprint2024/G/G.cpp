#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<double> P(N);
	for (auto& p : P) cin >> p;

	std::vector<double> dp(N+1);
	dp[0] = 1;
	std::vector<double> ndp(N+1);
	double ans = 0;
	for (int z = 0; z < N; z++) {
		double p = P[z];
		for (int i = 0; i <= z; i++) {
			double a = dp[i]; dp[i] = 0;
			// face up with probability p
			ans += p * a;
			ndp[i+1] += p * a;
			if (i > 0) {
				ans += (1-p) * a;
				ndp[i-1] += (1-p) * a;
			} else {
				ndp[0] += (1-p) * a;
			}
		}
		std::swap(dp, ndp);
	}
	cout << fixed << setprecision(10) << ans << '\n';


	return 0;
}
