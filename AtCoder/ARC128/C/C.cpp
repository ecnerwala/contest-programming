#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t N, M, S; cin >> N >> M >> S;
	std::vector<int64_t> pref(N+1);
	for (int i = 0; i < N; i++) {
		int a; cin >> a;
		pref[i+1] = pref[i] + a;
	}

	double ans = 0;
	for (int lo = 0; lo <= N; lo++) {
		if ((N-lo) * M < S) break;
		for (int hi = lo; hi <= N; hi++) {
			if ((N-hi) * M > S) continue;
			ans = max(ans, double(M * (pref[N] - pref[hi])) + double(S - M * (N - hi)) * double(pref[hi] - pref[lo]) / double(hi - lo));
		}
	}
	cout << fixed << setprecision(20) << ans << '\n';

	return 0;
}

// LP duality tells us that we just need some linear combination of the constraints (i.e.
//
// w_0 * (0 <= x_1) and w_1 * (x_1 <= x2) and ... and x_N <= M and k * sum x_i == S to make sum A_i * x_i <= ???
//
// k + (w_i - w_{i-1}) = A_i
// RHS = k * S + w_N * M
//
// w_i = w_0 + sum_{j = 1}^i A_j - k * i >= 0
// k <= (sum_{i=1}^i {A_i} + w_0) / i
// w_N = sum A_i - k * N + w_0
// RHS <= M * w_0 + M * sum A_i - (sum_{i=1}^i A_i + w_0) / i * (N * M - S)
// RHS minimized means k maximal
