#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<int64_t> A(N);
	for (auto& a : A) cin >> a;
	A.insert(A.begin(), int64_t(-1e9));
	A.push_back(int64_t(2e9));

	vector<int64_t> lrad; lrad.reserve(1e4);
	vector<int64_t> rrad; rrad.reserve(1e4);
	vector<int64_t> lpref; lpref.reserve(1e4);
	vector<int64_t> rpref; rpref.reserve(1e4);
	vector<int64_t> dp(N+2);
	for (int i = 1; i <= N+1; i++) {
		for (int j = max(0, i-11); j < i; j++) {
			lrad.assign(i-j+1, 0);
			rrad.assign(i-j+1, 0);
			lpref.assign(i-j+1, -1);
			rpref.assign(i-j+1, -1);
			lpref[j-j] = 0;
			rpref[i-j] = 0;
			for (int k = j+1; k < i; k++) {
				lrad[k-j] = (A[k] - A[k-1]) - lrad[k-1-j];
				if (lrad[k-j] < 0) break;
				lpref[k-j] = lpref[k-1-j] + lrad[k-j] * lrad[k-j];
			}
			for (int k = i-1; k > j; k--) {
				rrad[k-j] = (A[k+1] - A[k]) - rrad[k+1-j];
				if (rrad[k-j] < 0) break;
				rpref[k-j] = rpref[k+1-j] + rrad[k-j] * rrad[k-j];
			}
			for (int k = j; k < i; k++) {
				if (lpref[k-j] == -1) continue;
				if (rpref[k+1-j] == -1) continue;
				if (lrad[k-j] + rrad[k+1-j] > A[k+1] - A[k]) continue;
				dp[i] = max(dp[i], dp[j] + lpref[k-j] + rpref[k+1-j]);
			}
		}
	}
	cout << dp[N+1] << '\n';

	return 0;
}
