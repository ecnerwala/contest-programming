#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N; cin >> N;
	vector<int> A(N); for (auto& a : A) cin >> a;
	vector<int> B(N); for (auto& b : B) cin >> b;

	int64_t tot = 0;

	array<vector<int>, 2> C{vector<int>(N/2), vector<int>(N/2)};
	for (int i = 0; i < N; i++) {
		tot += A[i];
		C[i%2][i/2] = B[i] - A[i];
	}

	sort(C[0].begin(), C[0].end());
	sort(C[1].begin(), C[1].end());

	int64_t ans = tot;
	for (int i = N/2-1; i >= 0; i--) {
		tot += int64_t(C[0][i]) + int64_t(C[1][i]);
		ans = max(ans, tot);
	}
	cout << ans << '\n';

	return 0;
}
