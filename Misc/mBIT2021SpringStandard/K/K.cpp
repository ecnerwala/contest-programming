#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N; cin >> N;
	int L = 0;
	while ((1 << L) <= N) L++;
	vector<vector<int64_t>> A(L, vector<int64_t>(N));
	for (auto& a : A[0]) cin >> a;
	for (int l = 0; l+1 < L; l++) {
		for (int i = 0; i < N; i++) {
			A[l+1][i] = A[l][i] - A[l][(i+(1<<l))%N];
		}
	}

	int64_t ans = -1e18;
	for (int i = 0; i < N; i++) {
		int sign = +1;
		int cur = i;
		int64_t val = 0;
		for (int l = L-1; l >= 0; l--) {
			if (N & (1 << l)) {
				val += A[l][cur % N] * sign;
				cur += (1 << l);
				sign = -sign;
			}
		}
		ans = max(ans, val);
	}
	cout << ans << '\n';

	return 0;
}
