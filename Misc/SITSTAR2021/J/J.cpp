#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int M, N; cin >> M >> N;
	vector<int> A(N+1);
	vector<int> B(N+1);
	for (int i = 0; i < N; i++) {
		cin >> A[i] >> B[i];
	}
	A[N] = 0, B[N] = 0;

	// max balloons with these people last
	int S = 1; for (int z = 0; z < 5; z++) S *= N+1;
	vector<int> dp(S, -1);
	dp[S-1] = 0;
	vector<int> ndp(S);
	for (int t = 1; true; t++) {
		fill(ndp.begin(), ndp.end(), -1);
		for (int s = 0; s < S; s++) {
			if (dp[s] == -1) continue;
			for (int i = 0; i <= N; i++) {
				bool can_pump = true;
				for (int b = B[i], v = s; b > 0; b--, v /= (N+1)) {
					if (v % (N+1) == i) {
						can_pump = false;
						break;
					}
				}
				if (!can_pump) continue;
				int ns = (s * (N+1)) % S + i;
				if (dp[s] + A[i] >= M) {
					cout << t << '\n';
					exit(0);
				}
				ndp[ns] = max(ndp[ns], dp[s] + A[i]);
			}
		}
		swap(dp, ndp);
	}

	return 0;
}
