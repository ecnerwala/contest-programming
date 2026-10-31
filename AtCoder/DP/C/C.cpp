#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;

	array<int, 3> dp{0,0,0};
	for (int i = 0; i < N; i++) {
		array<int, 3> ndp{0,0,0};
		array<int, 3> A; for (auto& a : A) cin >> a;
		for (int a = 0; a < 3; a++) {
			for (int b = 0; b < 3; b++) {
				if (a==b) continue;
				ndp[b] = max(ndp[b], dp[a] + A[b]);
			}
		}
		dp = ndp;
	}
	cout << *max_element(dp.begin(), dp.end()) << '\n';

	return 0;
}
