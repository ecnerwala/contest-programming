#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	int K; cin >> K;

	std::vector<int> A(N);
	for (auto& a : A) cin >> a;
	std::vector<int> B(N);
	for (auto& b : B) cin >> b;

	int tot = std::accumulate(A.begin(), A.end(), 0) + std::accumulate(B.begin(), B.end(), 0);
	if (tot < K) {
		cout << "Reject" << '\n';
		exit(0);
	}

	auto check = [&](int fac) -> bool {
		const int64_t INF = 1e18;
		std::vector<int64_t> dp(tot+1, -INF);
		dp[0] = 0;
		for (int i = 0; i < N; i++) {
			int w = A[i] + B[i];
			int v = 10 * A[i] - fac * w;
			for (int a = tot; a >= w; a--) {
				dp[a] = std::max(dp[a], dp[a-w] + v);
			}
		}
		for (int a = K; a <= tot; a++) {
			if (dp[a] >= 0) {
				return true;
			}
		}
		return false;
	};

	if (check(7)) {
		cout << "Outstanding" << '\n';
	} else if (check(5)) {
		cout << "Good Quality" << '\n';
	} else {
		cout << "Low Quality" << '\n';
	}

	return 0;
}
