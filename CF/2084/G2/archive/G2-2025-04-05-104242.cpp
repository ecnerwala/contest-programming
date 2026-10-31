#include <bits/stdc++.h>

// if it's odd length, i.e. same parity, it's the max of the first and last, otherwise it's the min
// max means we want same side distance to be large, and opposite side distance to be small

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }
int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	//slow(20);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) { cin >> a; a--; }
		// we only care about the evens and the odds
		std::vector<int> invA(N, -1);
		for (int i = 0; i < N; i++) if (A[i] != -1) invA[A[i]] = i & 1;

		const int64_t INF = 1e18;
		int F = N / 2;
		std::vector<int64_t> dp(F+1, -INF);
		dp[0] = 0;
		for (int i = 0; i < N; i++) {
			for (int c = std::min(i, F); c >= 0; c--) {
				int64_t v = std::exchange(dp[c], -INF);
				if (v == -INF) continue;
				v += (2 * c - i) * (2 * (F - c) - (N-i));
				for (int z = 0; z < 2 && c + z <= F; z++) {
					if (invA[i] == -1 || ((invA[i] & 1) == z)) {
						setmax(dp[c+z], v);
					}
				}
			}
		}
		int64_t ans = dp[F];
		ans += int64_t(N+1) * int64_t(N) * int64_t(N+1) / 2;
		assert(!(ans & 1));
		ans /= 2;
		cout << ans << '\n';
	}

	return 0;
}
