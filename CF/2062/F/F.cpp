#include <bits/stdc++.h>

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		struct city_t {
			int s, d;
		};
		std::vector<city_t> cities(N);
		for (auto& [s, d] : cities) {
			int a, b; cin >> a >> b;
			s = a + b;
			d = a - b;
		}
		std::sort(cities.begin(), cities.end(), [&](auto x, auto y) -> bool {
			return x.d < y.d;
		});

		const int64_t INF = 1e18;
		std::vector<std::array<int64_t, 5>> dp; dp.reserve(N+1);
		dp.push_back({0, INF, INF, INF, INF});
		for (int i = 0; i < N; i++) {
			dp.push_back({INF, INF, INF, INF, INF});
			auto [s, d] = cities[i];
			for (int k = i; k >= 0; k--) {
				if (dp[k][0] != INF) {
					setmin(dp[k+1][1], dp[k][0] + s - d);
					setmin(dp[k+1][2], dp[k][0] + (s-d)/2);
				}
				if (dp[k][1] != INF) {
					setmin(dp[k+1][1], dp[k][1] + s);
					setmin(dp[k+1][2], dp[k][1] + (s+d)/2);
				}
				if (dp[k][2] != INF) {
					setmin(dp[k+1][2], dp[k][2] + s);
					setmin(dp[k+1][3], dp[k][2] + (s-d)/2);
					setmin(dp[k+1][4], dp[k][2] + (s+d)/2);
				}
				if (dp[k][3] != INF) {
					setmin(dp[k+1][3], dp[k][3] + s);
					setmin(dp[k+1][4], dp[k][3] + s+d);
				}
			}
		}
		for (int k = 2; k <= N; k++) {
			cout << dp[k][4] << " \n"[k==N];
		}
	}

	return 0;
}
