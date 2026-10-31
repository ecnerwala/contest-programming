#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; double P; cin >> N >> P;
		vector<double> dp; dp.reserve(N);
		dp.resize(1); dp[0] = 0;

		auto c2 = [](int a) -> int { return a * (a-1) / 2; };

		vector<double> ndp; ndp.reserve(N);
		for (int sz = 2; sz <= N; sz++) {
			ndp.assign(sz, 0);

			double denom = double(c2(sz));

			for (int i = 0; i < sz; i++) {
				ndp[i] = 1;
				if (i > 0) {
					// i against someone lower
					ndp[i] += (i / denom) * (P * dp[i-1]);
				}
				if (i < sz-1) {
					// i against someone higher
					ndp[i] += (sz-1-i) / denom * ((1-P) * dp[i]);
				}

				if (i >= 2) {
					// 2 people lower
					ndp[i] += c2(i) / denom * dp[i-1];
				}
				if (sz-1-i >= 2) {
					// 2 people higher
					ndp[i] += c2(sz-1-i) / denom * dp[i];
				}

				if (i >= 1 && sz-1-i >= 1) {
					ndp[i] += (i) * (sz-1-i) / denom * (P * dp[i-1] + (1-P) * dp[i]);
				}
			}
			swap(dp, ndp);
		}

		cout << "Case #" << case_num << ":" << '\n';
		for (int i = 0; i < N; i++) {
			cout << fixed << setprecision(10) << dp[i] << '\n';
		}
	}

	return 0;
}
