#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<int> A(N); for (auto& a : A) { cin >> a; a %= 3; }

	array<array<int, 3>, 3> dp{array<int, 3>{0,0,0}, array<int, 3>{0,0,0}, array<int, 3>{0,0,0}};

	int64_t ans = 0;
	for (int i = 0; i < N; i++) {
		array<array<int, 3>, 3> ndp{array<int, 3>{0,0,0}, array<int, 3>{0,0,0}, array<int, 3>{0,0,0}};
		dp[0][0] ++;
		for (int a = 0; a < 3; a++) {
			for (int b = 0; b < 3; b++) {
				ndp[(a+b*A[i])%3][(b+A[i])%3] += dp[a][b];
			}
		}
		dp = ndp;
		for (int b = 0; b < 3; b++) {
			ans += dp[0][b];
		}
	}
	cout << ans << '\n';

	return 0;
}
